//------------------------------------------------------------------------------
// Description: This file contains the macros used by other files
// Author: James Sorber
// Created  02/07/2023
// Modified 04/27/2023
// Built with IAR Embedded Workbench Version: (8.10.2)
//------------------------------------------------------------------------------


// General Macros
//------------------------------------------------------------------------------
#define TRUE                (0x01)
#define FALSE               (0x00)
#define RESET               (0x00)
#define ASCII_NULL          (0x00)
#define ASCII_QUOTE         (0x22)
#define ASCII_OFFSET        (0x30)
#define ALWAYS              (1)
#define RESET_STATE         (0)

// RTCS Stuff
//------------------------------------------------------------------------------
typedef struct {
    void (*Task)(void);
    int Period;
    int TicksUntilRelease;
    int ReleasesPending;
    char Enabled;
} RTCS_TASK_ENTRY;

#define RTCS_NUM_TASKS      (10)            // Number of tasks the RTCS can run. Can increase as needed
#define MOVEMENT_TASK       (2)



// Movement Related Macros
//------------------------------------------------------------------------------
#define WHEEL_PERIOD        (60000)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_FORWARD_SPEED  (TB3CCR4)
#define LEFT_REVERSE_SPEED  (TB3CCR5)

#define OFF                 (0x00)
#define ON_100_PRESCALE     (117)
#define SENSITIVITY         (2)
#define SCALE_FACTOR        (254)
#define CALC_THRES          (256)
#define ACCEL_AVG_BUF_SIZE  (3) 
#define MAX_PWM             (60000)
#define MIN_PWM             (9000)
#define LEFT_F              (0)
#define LEFT_R              (1)
#define RIGHT_F             (2)
#define RIGHT_R             (3)
#define HYPOT               (181)


// Timer Related
//------------------------------------------------------------------------------              
#define TB0_0_INTERVAL      (1250)  // RTCS Tick Rate = 10ms
#define TB1_0_INTERVAL      (65000)  // Defaul
#define TB2_0_INTERVAL      (65000)  // Default
#define DEBOUNCE            (65000) // About 0.5 seconds

typedef struct{
    unsigned int ccr0_period;
    unsigned char tick_ready;
} Timer_ISR;


// Line following related macros
//------------------------------------------------------------------------------



// LCD Related Macros
//------------------------------------------------------------------------------
#define LCD_BRIGHTNESS      (TB3CCR1)
#define PERCENT_60          (30000)
#define LCD_TIMEOUT         (50)        // Number of instances of update_display to turn backlight off


// Clock display related macros


// IOT Related Macros
//------------------------------------------------------------------------------


// Menue Related Macros
//------------------------------------------------------------------------------
// Create a struct to contain menu item
// Menu has attributes text and actions
// Both are 4 element arrays. Text has all the text for that menu
// Actions is an array of function pointers to the corrisponding function
typedef struct{
    void (*actions[4])(void);
    char text[4][11];
} MENU_ITEM;

#define NUM_MENUS           (4)
#define MAIN_MENU           (0)
#define NET_MENU            (1)
#define PWR_MENU            (2)
#define STAT_MENU           (3)

// DAC Related Macros
#define INITAL_DAC_VAL      (4000)
#define DESIRED_DAC         (1100)
#define DAC_STEP            (50)


// Serial Related Macros
//------------------------------------------------------------------------------
#define PROCESS_BUFF_SIZE   (64)
#define PROCESS_BUFF_ROWS   (4)

#define X_VAL               (0)
#define Y_VAL               (1)
#define START_STOP          (11)
#define GOT_X_VAL           (12)
#define GOT_Y_VAL           (13)
#define GOT_SSID            (14)
#define GOT_IP              (15)
#define NO_STR_FOUND        (3)
