//------------------------------------------------------------------------------
//  Description: This file contains all the ADC related interupt service routines
//  Author:     James Sorber
//  Created:    3/3/2023
//  Modified:   4/27/2023
//  Built with IAR Embedded Workbench Version: (8.10.2)
//------------------------------------------------------------------------------

// Include header libraries
#include "msp430.h"
#include "header_files/ports.h"
#include "header_files/basic_macros.h"
#include "header_files/functions.h"

// Global variables defined in this file
volatile unsigned int ADC_channel;
volatile unsigned int ADC_left_ir;
volatile unsigned int ADC_right_ir;
volatile unsigned int ADC_thumb;
volatile unsigned int last_ADC_thumb;
volatile unsigned char ADC_thumb_changed;


// Check ADC values
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV__NONE:
        break;
        case ADCIV__ADCOVIFG: // When a conversion result is written to the ADCMEM0 before its previous conversion result was read.
        break;
        case ADCIV__ADCTOVIFG:               // ADC conversion-time overflow
        break;
        case ADCIV__ADCHIIFG:                // Window comparator interrupt flags
        break;
        case ADCIV__ADCLOIFG:                // Window comparator interrupt flag
        break;
        case ADCIV__ADCINIFG:                // Window comparator interrupt flag
        break;

        case ADCIV__ADCIFG0:                 // ADCMEM0 memory register with the conversion result
            ADCCTL0 &= ~ADCENC;              // Disable ENC bit.
            switch (ADC_channel++){
                case 0x00:                   // Channel A2 Interrupt
                    ADC_left_ir = ADCMEM0; 
                    ADC_left_ir = ADC_left_ir >> 2;
                    ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
                    ADCMCTL0 |= ADCINCH_3;  // Enable Next channel A3
                    ADCCTL0 |= ADCENC;      // Enable Conversions
                    ADCCTL0 |= ADCSC;       // Start next sample
                break;
                case 0x01:
                    ADC_right_ir = ADCMEM0; 
                    ADC_right_ir = ADC_right_ir >> 2;
                    ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A3
                    ADCMCTL0 |= ADCINCH_5;  // Enable Next channel A5
                    ADCCTL0 |= ADCENC;      // Enable Conversions
                    ADCCTL0 |= ADCSC;       // Start next sample
                break;
                case 0x02:
                    ADC_thumb = ADCMEM0; 
                    ADC_thumb = ADC_thumb >> 4;
                    // Give a bit of buffer so it isn't constantly saying that the value has changed
                    if((ADC_thumb >> 3) != (last_ADC_thumb >> 3)){
                        last_ADC_thumb = ADC_thumb;
                        ADC_thumb_changed = TRUE;
                    }
                    ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A5
                    ADCMCTL0 |= ADCINCH_2;  // Enable Next channel A2
                    ADC_channel = 0;

                break;
                default:
                break;
            }
        break;

        default:
        break;
    }
}

