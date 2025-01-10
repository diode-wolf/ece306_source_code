//------------------------------------------------------------------------------
//  Description: This file contains Initalization functions
//  Author:     James Sorber
//  Created:    1/27/24
//  Modified:   1/27/24
//  Last built with Code Composer Studio v12.4.0
//------------------------------------------------------------------------------

// Includes
#include "msp430.h"
#include "macros.h"
#include "port_macros.h"
#include "functions.h"

// Extern Variables for Process Serial
extern volatile char IOT_msg_rcvd[PROCESS_BUFF_ROWS];
extern volatile char USB_msg_rcvd[PROCESS_BUFF_ROWS];
extern volatile char IOT_Rx_row_idx;
extern volatile char USB_Rx_row_idx;
extern volatile char IOT_Rx_Buff[PROCESS_BUFF_ROWS][PROCESS_BUFF_SIZE];
extern volatile char USB_Rx_Buff[PROCESS_BUFF_ROWS][PROCESS_BUFF_SIZE];
extern unsigned char x_value_ring_buf[ACCEL_AVG_BUF_SIZE];
extern unsigned char y_value_ring_buf[ACCEL_AVG_BUF_SIZE];
extern unsigned int avg_x_val;
extern unsigned int avg_y_val;
extern char rxd_ssid[PROCESS_BUFF_SIZE];
extern char rxd_ip[PROCESS_BUFF_SIZE];

// Extern Variables for Update Display
extern volatile unsigned char display_changed;
extern char display_line[4][11];

// Extern Variables for Update Menu
extern volatile unsigned int ADC_thumb_changed;
extern volatile unsigned int ADC_thumb;
extern volatile unsigned char switch1_press;
extern volatile unsigned char switch2_press;
extern MENU_ITEM all_menus[NUM_MENUS];

// Extern Variables for Calculate Movement
extern volatile RTCS_TASK_ENTRY RTCS_Task_Table[RTCS_NUM_TASKS];


// Local Variables
unsigned char current_menu;
unsigned char pending_menu_update;
unsigned char backlight_timeout;



// Process Serial
// This function is called 20x per second and checks for new messages from either serial port
// If data has been recieved the function does something with it
void Process_Serial(void){
    // For Debug
    P3OUT |= SMCLK;

    unsigned int i;
    unsigned int copy_idx;
    // Check if there are messages on the iot port to handle
    if(IOT_msg_rcvd[0] || IOT_msg_rcvd[1] || IOT_msg_rcvd[2] || IOT_msg_rcvd[3]){
        for(i = 0; i < PROCESS_BUFF_ROWS; i++){
            if(IOT_msg_rcvd[i]){
                IOT_msg_rcvd[i] = FALSE;
                
                // Search string for data, run various task based on data recieved
                switch (Search_String(IOT_Rx_Buff[i])){
                    case GOT_SSID:
                        // Wipe screen
                        strcpy(display_line[0], "          ");
                        strcpy(display_line[1], "          ");
                        strcpy(display_line[2], "          ");
                        strcpy(display_line[3], "          ");

                        // Copy ssid to screen
                        copy_idx = 0;
                        while(copy_idx < 10){
                            if(rxd_ssid[copy_idx] == ASCII_NULL){
                                break;
                            }
                            display_line[1][copy_idx] = rxd_ssid[copy_idx];
                            copy_idx++;
                        }
                        display_changed = TRUE;
                    break;

                    case GOT_IP:
                        // Wipe screen
                        strcpy(display_line[0], "          ");
                        strcpy(display_line[1], "          ");
                        strcpy(display_line[2], "          ");
                        strcpy(display_line[3], "          ");

                        // Copy first 8 digits of ip address to screen
                        copy_idx = 0;
                        while(copy_idx < 8){
                            display_line[1][copy_idx] = rxd_ip[copy_idx];
                            copy_idx++;
                        }
                        // Copy last up to 7 characters
                        while(copy_idx < 15){
                            if(rxd_ip[copy_idx] == ASCII_NULL){
                                break;
                            }
                            display_line[2][copy_idx - 8] = rxd_ip[copy_idx];
                            copy_idx++;
                        }
                        display_changed = TRUE;
                    break;

                    case GOT_Y_VAL:
                        // Set motor speeds
                        if(RTCS_Task_Table[MOVEMENT_TASK].Enabled){
                            avg_x_val = RESET;
                            avg_y_val = RESET;

                            // Calculate average acceleration values
                            for(i = 0; i < ACCEL_AVG_BUF_SIZE; i++){
                                avg_x_val += x_value_ring_buf[i];
                                avg_y_val += y_value_ring_buf[i];
                            }
                            avg_x_val = avg_x_val / ACCEL_AVG_BUF_SIZE;
                            avg_y_val = avg_y_val / ACCEL_AVG_BUF_SIZE;
                        }
                    break;

                    case START_STOP:
                        if(RTCS_Task_Table[MOVEMENT_TASK].Enabled){
                            P6OUT |= GRN_LED;
                        }
                        else{
                            P6OUT &= ~GRN_LED;
                            LEFT_FORWARD_SPEED = OFF;
                            LEFT_REVERSE_SPEED = OFF;
                            RIGHT_FORWARD_SPEED = OFF;
                            RIGHT_REVERSE_SPEED = OFF;
                        }
                    break;
                    
                    default:
                    break;
                }
            }
        }
    }

    // Check if there are messages on the usb port to handle
    if(USB_msg_rcvd[0] || USB_msg_rcvd[1] || USB_msg_rcvd[2] || USB_msg_rcvd[3]){
        for(i = 0; i < PROCESS_BUFF_ROWS; i++){
            if(USB_msg_rcvd[i]){
                USB_msg_rcvd[i] = FALSE;
                Transmit(0, USB_Rx_Buff[i]);
            }
        }
    }
    P3OUT &= ~SMCLK;
}

// Update Display
void Update_Display(void){
    if(display_changed){
        display_changed = FALSE;
        Display_Update(0,0,0,0);
        //P3OUT ^= TEST_PROBE;
    }
}

// Check ADC runs a conversion on ADC channels 2, 3, and 5
void Check_ADC(void){
    ADCCTL0 |=  ADCENC;   // Enable ADC
    ADCCTL0 |=  ADCSC;    // Start conversion
}

// Update Menu
// This function updates the LCD to reflect the correct menu
// Also processes any interactions with the menu
void Update_Menu(void){
    // Redraw menu if thumbwheel has been rotated or if menu changed
    if(ADC_thumb_changed || pending_menu_update){
        ADC_thumb_changed = FALSE;
        pending_menu_update = FALSE;
        LCD_BRIGHTNESS = PERCENT_60;
        backlight_timeout = 50;
        // Redraw all lines of current menu and add pointer indicator
        strcpy(display_line[0], all_menus[current_menu].text[0]);
        strcpy(display_line[1], all_menus[current_menu].text[1]);
        strcpy(display_line[2], all_menus[current_menu].text[2]);
        strcpy(display_line[3], all_menus[current_menu].text[3]);
        display_line[ADC_thumb >> 6][0] = '>';
        display_changed = TRUE;
    }

    // Run action item on menu
    if(switch1_press){
        switch1_press = FALSE;
        LCD_BRIGHTNESS = PERCENT_60;
        backlight_timeout = 50;
        if(all_menus[current_menu].actions[ADC_thumb >> 6] != 0){
            all_menus[current_menu].actions[ADC_thumb >> 6]();
        }
    }

    // Return to main menu
    if(switch2_press){
        switch2_press = FALSE;
        LCD_BRIGHTNESS = PERCENT_60;
        backlight_timeout = 50;
        current_menu = MAIN_MENU;
        pending_menu_update = TRUE;
    }

    // Turn off back light if inactive for > 5 sec
    if(LCD_BRIGHTNESS){
        if(backlight_timeout > 1){
            backlight_timeout--;
        }
        else{
            LCD_BRIGHTNESS = OFF;
        }
    }
}

