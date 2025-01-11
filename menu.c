//------------------------------------------------------------------------------
//  Description: This file contains the code to create a menu system
//  Author:     James Sorber
//  Created:    1/28/24
//  Modified:   1/28/24
//  Last built with Code Composer Studio v12.4.0
//------------------------------------------------------------------------------

#include "msp430.h"
#include "header_files/functions.h"
#include "header_files/menu.h"
#include "header_files/basic_macros.h"

// Variables
//------------------------------------------------------------------------------
// Create an array to hold all the all_menus
// Menu 0: Main
// Menu 1: Network
// Menu 2: Power
// Menu 3: Status
MENU_ITEM all_menus[NUM_MENUS];

extern unsigned char current_menu;
extern unsigned char pending_menu_update;

// Menu Change functions
//------------------------------------------------------------------------------
void Enter_NM(void){
    current_menu = NET_MENU;
    pending_menu_update = TRUE;
}

void Enter_PM(void){
    current_menu = PWR_MENU;
    pending_menu_update = TRUE;
}

void Enter_SM(void){
    current_menu = STAT_MENU;
    pending_menu_update = TRUE;
}

// Action functions
//------------------------------------------------------------------------------
void Get_SSID(void){
    // Add content Here
    Transmit(0, "AT+CWJAP?\r\n");
}

void Get_IP(void){
    // Add content Here
    Transmit(0, "AT+CIFSR\r\n");
}

void Mux_EN(void){
    // Add content Here
    Transmit(0, "AT+CIPMUX=1\r\n");
}

void Start_TCP(void){
    // Add content Here
    Transmit(0, "AT+CIPSERVER=1,7983\r\n");
}



// Init_Menus
// This function initalizes the Menus
// Assumes text arrays filled with zeros
//------------------------------------------------------------------------------
void Init_Menus(void){
    // Main Menu
    all_menus[MAIN_MENU].actions[0] = Enter_NM;
    all_menus[MAIN_MENU].actions[1] = Enter_PM;
    all_menus[MAIN_MENU].actions[2] = Enter_SM;
    all_menus[MAIN_MENU].actions[3] = (void *) 0;
    strcpy(all_menus[MAIN_MENU].text[0], " NETWORK  ");
    strcpy(all_menus[MAIN_MENU].text[1], " TEMP 0.1 ");
    strcpy(all_menus[MAIN_MENU].text[2], " TEMP 0.2 ");
    strcpy(all_menus[MAIN_MENU].text[3], " TEMP 0.3 ");

    // Network Menu
    all_menus[NET_MENU].actions[0] = Get_SSID;
    all_menus[NET_MENU].actions[1] = Get_IP;
    all_menus[NET_MENU].actions[2] = Mux_EN;
    all_menus[NET_MENU].actions[3] = Start_TCP;
    strcpy(all_menus[NET_MENU].text[0], " GET_SSID ");
    strcpy(all_menus[NET_MENU].text[1], "  GET_IP  ");
    strcpy(all_menus[NET_MENU].text[2], "  MUX_EN  ");
    strcpy(all_menus[NET_MENU].text[3], " START_TCP");

    // Power Menu
    all_menus[PWR_MENU].actions[0] = (void *) 0;
    all_menus[PWR_MENU].actions[1] = (void *) 0;
    all_menus[PWR_MENU].actions[2] = (void *) 0;
    all_menus[PWR_MENU].actions[3] = (void *) 0;
    strcpy(all_menus[PWR_MENU].text[0], " TEMP 2.0 ");
    strcpy(all_menus[PWR_MENU].text[1], " TEMP 2.1 ");
    strcpy(all_menus[PWR_MENU].text[2], " TEMP 2.2 ");
    strcpy(all_menus[PWR_MENU].text[3], " TEMP 2.3 ");

    // Status Menu
    all_menus[STAT_MENU].actions[0] = (void *) 0;
    all_menus[STAT_MENU].actions[1] = (void *) 0;
    all_menus[STAT_MENU].actions[2] = (void *) 0;
    all_menus[STAT_MENU].actions[3] = (void *) 0;
    strcpy(all_menus[STAT_MENU].text[0], " TEMP 3.0 ");
    strcpy(all_menus[STAT_MENU].text[1], " TEMP 3.1 ");
    strcpy(all_menus[STAT_MENU].text[2], " TEMP 3.2 ");
    strcpy(all_menus[STAT_MENU].text[3], " TEMP 3.3 ");
}