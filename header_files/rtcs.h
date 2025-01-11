//------------------------------------------------------------------------------
// Description: This file contains the macros used by rtcs.c
// Author: James Sorber
// Created  01/11/2025
// Modified 01/11/2025
// Built with Code Composer Studio v12.4.0
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