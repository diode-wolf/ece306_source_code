//------------------------------------------------------------------------------
// Description: This file contains the macros used menu.c
// Author: James Sorber
// Created  01/11/2025
// Modified 01/11/2025
// Built with Code Composer Studio v12.4.0
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