//------------------------------------------------------------------------------
//  Description: 
//
//  Author:     James Sorber
//  Created:    
//  Modified:   
//  Built with 
//------------------------------------------------------------------------------

#include "header_files/ports.h"
#include "header_files/basic_macros.h"
#include "header_files/init.h"
#include "header_files/timers.h"
#include "msp430.h"
#include "header_files/functions.h"

// Variables
//------------------------------------------------------------------------------
extern volatile unsigned int DAC_data;                         // Value written to the DAC

extern Timer_ISR timer_B0;

unsigned int startup_sequence;
char startup_complete;


// This ISR handles interupts from TB0CCR0
//----------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
    // Content goes here
    TB0CCR0 += timer_B0.ccr0_period;
    timer_B0.tick_ready = TRUE;
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    switch(__even_in_range(TB0IV, 14)){
        case 0: break; // No interrupt

        case 2: // CCR1 Used for SW1 Debounce
            TB0CCTL1 &= ~TBIE;          // Disable interupts from timer B0_1
            P4IFG &= ~SW1;              // Clear interupts from SW1
            P4IE |=  SW1;               // Enable interupts from SW1
        break;

        case 4: // CCR2 Used for SW2 Debounce           
            TB0CCTL2 &= ~TBIE;          // Disable interupts from timer B0_2
            P2IFG &= ~SW2;              // Clear interupts from SW2
            P2IE |=  SW2;               // Enable interupts from SW2
        break;

        case 6:
        break;

        case 8:
        break;

        case 10:
        break;

        case 12:
        break;

        case 14: // overflow interrupt
            startup_sequence++;
            // Start up state machine
            if(DAC_data > DESIRED_DAC){
                DAC_data -= DAC_STEP;
                SAC3DAT = DAC_data;
            }
            else{
                startup_complete = TRUE;
                TB0CTL &=  ~TBIE;
            }

            switch (startup_sequence){
                case 3:
                    P3OUT |=  IOT_EN_CPU;       // Enable IOT
                break;
            
                default:
                    break;
            }
        break;

        default: 
        break;
    }
}


// Timer B1 0-2
#pragma vector = TIMER1_B0_VECTOR           // get ADC values every 1/20 seconds
__interrupt void Timer1_B0_ISR(void){
    // Content here
}

#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
    switch(__even_in_range(TB1IV, 14)){
        case 0: break; // No interrupt

        case 2:
            // Content here
        break;

        case 4:         
        break;

        case 6:
        break;

        case 8:
        break;

        case 10:
        break;

        case 12:
        break;

        case 14:  
        break;
        
        default: 
        break;
    }
}


// Timer B2 0-2
#pragma vector = TIMER2_B0_VECTOR
__interrupt void Timer2_B0_ISR(void){
    // Code goes here
}

#pragma vector=TIMER2_B1_VECTOR
__interrupt void TIMER2_B1_ISR(void){
    switch(__even_in_range(TB2IV, 14)){
        case 0: break; // No interrupt

        case 2:
        break;

        case 4:           
        break;

        case 6:
        break;

        case 8:
        break;

        case 10:
        break;

        case 12:
        break;

        case 14:
        break;

        default:
        break;
    }
}
