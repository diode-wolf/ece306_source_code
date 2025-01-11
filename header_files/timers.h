//------------------------------------------------------------------------------
// Description: This file contains the macros mainly used by timer_isrs.c
// Author: James Sorber
// Created  01/11/2025
// Modified 01/11/2025
// Built with Code Composer Studio v12.4.0
//------------------------------------------------------------------------------

typedef struct{
    unsigned int ccr0_period;
    unsigned char tick_ready;
} Timer_ISR;

// Timers
#define TB0_0_INTERVAL      (1250)  // RTCS Tick Rate = 10ms
#define TB1_0_INTERVAL      (65000)  // Defaul
#define TB2_0_INTERVAL      (65000)  // Default