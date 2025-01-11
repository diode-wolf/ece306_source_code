//------------------------------------------------------------------------------
//  Description: This file contains the Main Routine - "While" Operating System
//  Author:  James Sorber
//  Created  12/19/2023
//  Modified 12/19/2023
//  Last built with Code Composer Studio (12.4.0)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include all function prototypes                              
#include "msp430.h"
#include "header_files/basic_macros.h"
#include "header_files/ports.h"
#include "header_files/functions.h"


// Global Variables defined externally
//------------------------------------------------------------------------------
extern unsigned int startup_complete;
extern char display_line[4][11];


void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is RTCS
//------------------------------------------------------------------------------
PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

// Add Init functions here
// Init_Ports enables the XT1 Crystal and must be called before Init_Clocks
Init_Ports();
Init_Clocks();
Init_Timers();
Init_ADC();
Init_Serial();
Init_LCD();
Init_DAC();
Init_Menus();
RTCS_Init();
enable_interrupts();

// TASKS
//#0: Update Display
//#1: Process Serial
//#2: Check ADC
//#3: Update Menu
//#4: 
//#5:
//#6:
//#7:
//#8:
//#9:

// Syntax is (linked function, priority, period, ticks till 1st release)
// One tick happens every 10ms
RTCS_Add_Task(Update_Display, 0, 20, 20);
RTCS_Enable_Task(Update_Display, TRUE);

RTCS_Add_Task(Process_Serial, 1, 2, 5);
RTCS_Enable_Task(Process_Serial, TRUE);

RTCS_Add_Task(Calculate_Movement, 2, 5, 7);
RTCS_Enable_Task(Calculate_Movement, FALSE);

RTCS_Add_Task(Check_ADC, 3, 10, 10);
RTCS_Enable_Task(Check_ADC, TRUE);

RTCS_Add_Task(Update_Menu, 4, 10, 5);
RTCS_Enable_Task(Update_Menu, TRUE);



// RTCS Scheduler Call
// Contains while loop and never exists
Enter_RTCS_Scheduler();

}
