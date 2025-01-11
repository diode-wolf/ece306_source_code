//------------------------------------------------------------------------------
//  Description: This file contains the source code for the RTCS scheduler
//
//  James Sorber
//  Created:    1/27/2024
//  Modified:   1/27/2024
//  Last built with Code Composer Studio (12.4.0)
//------------------------------------------------------------------------------

#include "msp430.h"
#include "header_files/rtcs.h"
#include "header_files/timers.h"
#include "header_files/basic_macros.h"
#include "header_files/ports.h"
#include "header_files/functions.h"


// Create a structure to hold each task
// Each task entry has a pointer to the task function
// A number of timer ticks until the next release of the task
// A number of releases ReleasesPending
// A true/false value for enabled/disabled

volatile RTCS_TASK_ENTRY RTCS_Task_Table[RTCS_NUM_TASKS];
extern Timer_ISR timer_B0;


// RTCS_Init
// This function initalizes the RTCS scheduler
//------------------------------------------------------------------------------
void RTCS_Init(void){
    // Initialize task table
    int i;
    for(i = 0; i < RTCS_NUM_TASKS; i++){
        RTCS_Task_Table[i].Task = (void *) 0;
        RTCS_Task_Table[i].Period = 0;
        RTCS_Task_Table[i].TicksUntilRelease = 0;
        RTCS_Task_Table[i].ReleasesPending = 0;
        RTCS_Task_Table[i].Enabled = FALSE;
    }

    TB0CCTL0 |= CCIE;                   // Enable TB0 interupt used for timer tick
}

// RTCS_Add_Task
// This function adds a task to the RTCS task table
// Must provide pointer to task function, task period in 10s of ms, priority, ticks til next release
// Priority sets location in table. 0 is highest, RTCS_NUM_TASKS is lowest
// Will overwrite existing task with same priority
int RTCS_Add_Task(void (*task)(void), int priority, int period, int ttnr){
    if(priority < RTCS_NUM_TASKS){
        RTCS_Task_Table[priority].Task = task;
        RTCS_Task_Table[priority].Period = period;
        RTCS_Task_Table[priority].TicksUntilRelease = ttnr;
        RTCS_Task_Table[priority].ReleasesPending = 0;
        RTCS_Task_Table[priority].Enabled = FALSE;
        return TRUE;
    }

    return FALSE;
}


// RTCS_Timer_Tick
// This function updates all tasks after a timer tick
void RTCS_Timer_Tick(void){
    unsigned int i;
    for(i = 0; i < RTCS_NUM_TASKS; i++){
        if((RTCS_Task_Table[i].Task != 0) && (RTCS_Task_Table[i].Enabled) && (RTCS_Task_Table[i].TicksUntilRelease > 0)){
            RTCS_Task_Table[i].TicksUntilRelease--;
            if(RTCS_Task_Table[i].TicksUntilRelease == 0){
                RTCS_Task_Table[i].ReleasesPending++;
                RTCS_Task_Table[i].TicksUntilRelease = RTCS_Task_Table[i].Period;
            }
        }
    }
}


// RTCS_Get_Index
// This function is a helper function for other RTCS functions
// Converts task function name to table entry index
int RTCS_Get_Index(void (*task)(void)){
    int i;
    for(i = 0; i < RTCS_NUM_TASKS; i++){
        if(RTCS_Task_Table[i].Task == task){
            return i;
        }
    }

    return -1;
}

// RTCS_Enable_Task
// This function takes the name of a task function and sets/clears the enabled flag
void RTCS_Enable_Task(void (*task)(void), char enable) {
	int i = RTCS_Get_Index(task);

	if ((i >= 0) && (i < RTCS_NUM_TASKS)) {
		RTCS_Task_Table[i].Enabled = enable;
	}
}


// RTCS_Set_Period
// This function takes the name of a task function and sets the period
// If passed true, optional parameter release now increments the release pending
void RTCS_Set_Task_Period(void (*task)(void), int period, char release_now){
	int i = RTCS_Get_Index(task);

	if ((i >= 0) && i < RTCS_NUM_TASKS) {
        RTCS_Task_Table[i].Period = period;
        if(release_now){
            RTCS_Task_Table[i].ReleasesPending++;
        }
	}
}

// RTCS_Release_Task
// This function takes the name of a task function and increments the relases pending
void RTCS_Release_Task(void (*task)(void)) {
	int i = RTCS_Get_Index(task);

	if ((i >= 0) && (i<RTCS_NUM_TASKS)) {
		RTCS_Task_Table[i].ReleasesPending++;
	}
}


// Enter_RTCS_Scheduler
// This function enters the scheduler loop. Does not return
void Enter_RTCS_Scheduler(void){
    while(TRUE){
        // P3OUT ^= TEST_PROBE;
        // Process timer ticks
        if(timer_B0.tick_ready){
            timer_B0.tick_ready = FALSE;
            RTCS_Timer_Tick();
        }
        // Iterate through table to find highest priority ready task
        int i;
        for(i = 0; i < RTCS_NUM_TASKS; i++){
            // Run task if it exists, is enabled, and has releases pending
            if((RTCS_Task_Table[i].Task != 0) && (RTCS_Task_Table[i].ReleasesPending > 0) && (RTCS_Task_Table[i].Enabled)){
                RTCS_Task_Table[i].ReleasesPending--;
                RTCS_Task_Table[i].Task();
                break;
            }
        }
    }
}



