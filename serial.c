//------------------------------------------------------------------------------
//
//  Description: This file contains the interupt service routines for the serial ports
//
//  Author:     James Sorber
//  Created:    01/08/2024
//  Modified:   01/08/2024
//  Last built with Code Composer Studio (12.4.0)
//------------------------------------------------------------------------------

#include "msp430.h"
#include "header_files/serial.h"
#include "header_files/rtcs.h"
#include "header_files/basic_macros.h"
#include "header_files/movement.h"

// global variables defined externally
extern volatile RTCS_TASK_ENTRY RTCS_Task_Table[RTCS_NUM_TASKS];


// global variables defined in this file
volatile char IOT_Rx_Buff[PROCESS_BUFF_ROWS][PROCESS_BUFF_SIZE];
volatile char IOT_Rx_char_idx;
volatile char IOT_Rx_row_idx;
volatile char IOT_msg_rcvd[PROCESS_BUFF_ROWS];
volatile char IOT_Tx_Buff[PROCESS_BUFF_SIZE];
volatile char IOT_Tx_idx = 0x01;

volatile char USB_Rx_Buff[PROCESS_BUFF_ROWS][PROCESS_BUFF_SIZE];
volatile char USB_Rx_char_idx;
volatile char USB_Rx_row_idx;
volatile char USB_msg_rcvd[PROCESS_BUFF_ROWS];
volatile char USB_Tx_Buff[PROCESS_BUFF_SIZE];
volatile char USB_Tx_idx = 0x01;

unsigned char x_value_ring_buf[ACCEL_AVG_BUF_SIZE];
unsigned char y_value_ring_buf[ACCEL_AVG_BUF_SIZE];
unsigned char x_value_buf_idx;
unsigned char y_value_buf_idx;
unsigned int avg_x_val;
unsigned int avg_y_val;

unsigned char motors_enabled;
char rxd_ssid[PROCESS_BUFF_SIZE];
char rxd_ip[PROCESS_BUFF_SIZE];

// Serial Port 0 IOT
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    switch(__even_in_range(UCA0IV,0x08)){
        case 0: // Vector 0 - no interrupt
        break;

        case 2: // Vector 2 - RXIFG. Copy received char into ring buffer
            IOT_Rx_Buff[IOT_Rx_row_idx][IOT_Rx_char_idx] = UCA0RXBUF;

            // Re transmit character
            UCA1TXBUF = IOT_Rx_Buff[IOT_Rx_row_idx][IOT_Rx_char_idx];

            IOT_Rx_char_idx++;

            // Go to new line if either linefeed recieved or if reached end of current row
            // Only go to new row at line feed if the linefeed is not the first character in the array
            // This is to prevent single linefeeds using all the rows in my Rx_Buf character arrays
            // Will result in message with this format occasionally: /n[data]
            if((IOT_Rx_char_idx >= PROCESS_BUFF_SIZE) || ((IOT_Rx_Buff[IOT_Rx_row_idx][IOT_Rx_char_idx - 1] == '\n') && IOT_Rx_char_idx > 1)){
                IOT_Rx_char_idx = RESET;

                // Set flag when message received
                IOT_msg_rcvd[IOT_Rx_row_idx] = TRUE;
                IOT_Rx_row_idx++;

                // Clear next row to be written to
                int i;
                for(i = 0; i < PROCESS_BUFF_SIZE; i++){
                    IOT_Rx_Buff[IOT_Rx_row_idx % PROCESS_BUFF_ROWS][i] = RESET;
                }

            }

            // Wrap row index when needed
            if(IOT_Rx_row_idx >= PROCESS_BUFF_ROWS){
                IOT_Rx_row_idx = RESET;
            }
        break;

        case 4: // Vector 4 - TXIFG
            if((IOT_Tx_Buff[IOT_Tx_idx] == ASCII_NULL) || (IOT_Tx_idx >= (PROCESS_BUFF_SIZE - 1))){
                UCA0IE &= ~UCTXIE;                                  // Disable interupt if done transmitting
                IOT_Tx_Buff[IOT_Tx_idx] = ASCII_NULL;               // Null written character from transmit buffer
                IOT_Tx_idx = 1;                                 // Reset index to zero
            }
            else{
                UCA0TXBUF = IOT_Tx_Buff[IOT_Tx_idx];                    // Transmit character
                IOT_Tx_Buff[IOT_Tx_idx] = ASCII_NULL;               // Null written character from transmit buffer
                IOT_Tx_idx++;                                       // Increment index
            }
        break;

        default:
        break;
    }
}


// Serial Port 1 USB
//------------------------------------------------------------------------------
#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
    switch(__even_in_range(UCA1IV,0x08)){
        case 0: // Vector 0 - no interrupt
        break;

        case 2: // Vector 2 - RXIFG. Copy received char into ring buffer
            USB_Rx_Buff[USB_Rx_row_idx][USB_Rx_char_idx] = UCA1RXBUF;
            USB_Rx_char_idx++;

            // Go to new line if either linefeed recieved or if reached end of current row
            if((USB_Rx_char_idx >= PROCESS_BUFF_SIZE) || (USB_Rx_Buff[USB_Rx_row_idx][USB_Rx_char_idx - 1] == '\n')){
                USB_Rx_char_idx = RESET;

                // Set flag when message received
                USB_msg_rcvd[USB_Rx_row_idx] = TRUE;
                USB_Rx_row_idx++;

                // Clear next row to be written to
                int i;
                for(i = 0; i < PROCESS_BUFF_SIZE; i++){
                    USB_Rx_Buff[USB_Rx_row_idx % PROCESS_BUFF_ROWS][i] = RESET;
                }               
            }

            // Wrap row index when needed
            if(USB_Rx_row_idx >= PROCESS_BUFF_ROWS){
                USB_Rx_row_idx = RESET;
            }
        break;

        case 4: // Vector 4 - TXIFG
            if((USB_Tx_Buff[USB_Tx_idx] == ASCII_NULL) || (USB_Tx_idx >= (PROCESS_BUFF_SIZE - 1))){
                UCA1IE &= ~UCTXIE;                                  // Disable interupt if done transmitting
                USB_Tx_Buff[USB_Tx_idx] = ASCII_NULL;               // Null written character from transmit buffer
                USB_Tx_idx = 1;                                 // Reset index to zero
            }
            else{
                UCA1TXBUF = USB_Tx_Buff[USB_Tx_idx];                    // Transmit character
                USB_Tx_Buff[USB_Tx_idx] = ASCII_NULL;               // Null written character from transmit buffer
                USB_Tx_idx++;                                       // Increment index
            }
        break;

        default:
        break;
    }
}


// Transmit
// This function transmits a string out of a serial Port
// Port specifies the port, 0 for IOT, 1 for USB
// string is a pointer to the string to transmit
void Transmit(char port, volatile char* target){
    if(port == 0){
        strcpy(IOT_Tx_Buff, target);
        UCA0IE |= UCTXIE;
        UCA0TXBUF = IOT_Tx_Buff[0];
    }
    else if(port == 1){
        strcpy(USB_Tx_Buff, target);
        UCA1IE |= UCTXIE;
        UCA1TXBUF = USB_Tx_Buff[0];
    }
}

// Search String
// This function takes a pointer to a string of length PROCESS_BUFF_SIZE
// This fuction then scans the string for expressions of intrest
// Saves data found in variables, returns char type to indicate type of data found
char Search_String(char* target){
    unsigned int i;
    unsigned int j;
    unsigned char temp_x;
    unsigned char temp_y;
    // Iterate through process buffer checking for string matches
    for(i = 0; i < PROCESS_BUFF_SIZE; i++){
        // Check for null -> reached end of message
        if(target[i] == ASCII_NULL){
            return NO_STR_FOUND;
        }

        // Check for SSID 
        if((target[i] == 'A') && (target[i+1] == 'P') && (target[i+2] == ':')){
            // If got ssid, clear ssid array, copy in new data
            for(j = 0; j < PROCESS_BUFF_SIZE; j++){
                rxd_ssid[j] = ASCII_NULL;
            }

            for(j = 0; (j + i + 4) < PROCESS_BUFF_SIZE; j++){
                if(target[j + i + 4] == ASCII_QUOTE){
                    return GOT_SSID;
                }

                rxd_ssid[j] = target[j + i + 4];
            }
        }

        // Check for IP 
        if((target[i] == 'I') && (target[i+1] == 'P') && (target[i+2] == ',')){
            // If got ip, clear ip array, copy in new data
            for(j = 0; j < PROCESS_BUFF_SIZE; j++){
                rxd_ip[j] = ASCII_NULL;
            }
            
            for(j = 0; (j + i + 4) < PROCESS_BUFF_SIZE; j++){
                if(target[j + i + 4] == ASCII_QUOTE){
                    return GOT_IP;
                }

                rxd_ip[j] = target[j + i + 4];
            }
        }

        // Check for start/stop command
        if((target[i] == '%') && (target[i+1] == '~') && (target[i+2] == 'S')){
            // Toggle motors being enabled
            RTCS_Task_Table[MOVEMENT_TASK].Enabled = !RTCS_Task_Table[MOVEMENT_TASK].Enabled;
            return START_STOP;
        }

        // Check for X accel value
        if((target[i] == 'x') && (target[i+1] == '%') && (target[i+2] == '~')){
            for(j = 0; (j + i + 4) < PROCESS_BUFF_SIZE; j++){
                if(target[j + i + 4] == '\r'){
                    break;
                }

                // convert from ascii string to number
                temp_x *= 10;
                temp_x += (target[j + i + 4] - ASCII_OFFSET);
            }

            // Add to ring buffer
            x_value_ring_buf[x_value_buf_idx] = temp_x;
            x_value_buf_idx++;
            if(x_value_buf_idx >= ACCEL_AVG_BUF_SIZE){
                x_value_buf_idx = RESET;
            }

            return GOT_X_VAL;
        }

        // Check for Y accel value
        if((target[i] == 'y') && (target[i+1] == '%') && (target[i+2] == '~')){
            for(j = 0; (j + i + 4) < PROCESS_BUFF_SIZE; j++){
                if(target[j + i + 4] == '\r'){
                    break;
                }

                // convert from ascii string to number
                temp_y *= 10;
                temp_y += (target[j + i + 4] - ASCII_OFFSET);
            }

            // Add to ring buffer
            y_value_ring_buf[y_value_buf_idx] = temp_y;
            y_value_buf_idx++;
            if(y_value_buf_idx >= ACCEL_AVG_BUF_SIZE){
                y_value_buf_idx = RESET;
            }

            return GOT_Y_VAL;
        }
    }

    // If code gets to this point there has been some error
    return NO_STR_FOUND;
}


// BCD OUT
// This function is given an unsigned int and converts to string
// Must specify int, pointer to output char array, length of output array
void BCD_Out(unsigned int input, char* output, char len){
    char ten_k;
    char thousands;
    char hundreds;
    char tens;

    char i;
    for(i = 0; i < len; i++){
        output[i] = 0x00;
    }
    while(input > 10000){
        input = input - 10000;
        ten_k++; 
    }
    while(input > 1000){
        input = input - 1000;
        thousands++; 
    }
    while(input > 100){
        input = input - 100;
        hundreds++; 
    }
    while(input > 10){
        input = input - 10;
        tens++; 
    }
    output[len - 6] = ten_k + ASCII_OFFSET;
    output[len - 5] = thousands + ASCII_OFFSET;
    output[len - 4] = hundreds + ASCII_OFFSET;
    output[len - 3] = tens + ASCII_OFFSET;
    output[len - 2] = input + ASCII_OFFSET;
}
