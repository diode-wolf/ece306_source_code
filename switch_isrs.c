//------------------------------------------------------------------------------
//
//  Description: This file contains all the switch related ISRs
//  File includes:
//      Switch1_ISR(void)
//      Switch2_ISR(void)
//
//  Author:     James Sorber
//  Created:    2/20/2023
//  Modified:   2/24/2023
//  Built with IAR Embedded Workbench Version: (8.10.2)
//------------------------------------------------------------------------------

#include "msp430.h"
#include "macros.h"
#include "port_macros.h"
#include "functions.h"

volatile unsigned char switch1_press;
volatile unsigned char switch2_press;

//------------------------------------------------------------------------------
// Port 4 interrupt for switch 1, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT4_VECTOR
__interrupt void switch1_interrupt(void) {
    if(P4IFG & SW1){
        P4IE &= ~SW1;                               // Disable interupts
        P4IFG &= ~SW1;
        unsigned int new_comp_value;
        new_comp_value = (DEBOUNCE + TB0R);         // Do some janky stuff bc compiler dosn't like two volatiles in one line
        TB0CCR1 = new_comp_value;                   // Set delay for switch debounce
        TB0CCTL1 |=  CCIE;                          // Enable Interupts from timer B0_1
        switch1_press = TRUE;
    }
}


//------------------------------------------------------------------------------
// Port 4 interrupt for switch 1, it is disabled for the duration
// of the debounce time. Debounce time is set for 1 second
#pragma vector=PORT2_VECTOR
__interrupt void switch2_interrupt(void) {
    if(P2IFG & SW2){
        P2IE &= ~SW2;                               // Disable interupts
        P2IFG &= ~SW2;
        unsigned int new_comp_value;
        new_comp_value = (DEBOUNCE + TB0R);         // Do some janky stuff bc compiler dosn't like two volatiles in one line
        TB0CCR2 = new_comp_value;                   // Set delay for switch debounce
        TB0CCTL2 |=  CCIE;                          // Enable Interupts from timer B0_2
        switch2_press = TRUE;
    }
}
