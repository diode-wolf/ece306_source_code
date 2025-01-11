//------------------------------------------------------------------------------
//  Description: This file contains Initalization functions
//  Author: James Sorber
//  Created 
//  Modified 
//  Built with 
//------------------------------------------------------------------------------

#include "msp430.h"
#include "header_files/basic_macros.h"
#include "header_files/init.h"
#include "header_files/timers.h"
#include "header_files/tasks.h"
#include "header_files/functions.h"
#include "header_files/movement.h"

volatile unsigned int DAC_data;                         // Value written to the DAC
Timer_ISR timer_B0;

// //DAC Initialization
// //------------------------------------------------------------------------------
void Init_DAC(void){
    // Init reference voltage to 2.5 V
    PMMCTL0_H = PMMPW_H;                                // Unlock the PMM registers
    PMMCTL2 = INTREFEN;                                 // Enable internal reference
    PMMCTL2 |= REFVSEL_2;                               // Select 2.5V reference
    while(!(PMMCTL2 & REFGENRDY));                      // Poll till internal reference settles

    // Init DAC
    DAC_data = INITAL_DAC_VAL;                          // Value between 0x000 and 0x0FFF
    SAC3DAT = DAC_data;                                 // Initial DAC data
    SAC3DAC = DACSREF_1;                                // Select int Vref as DAC reference
    SAC3DAC |= DACLSEL_0;                               // DAC latch loads when DACDAT written
    //SAC3DAC |= DACIE;                                 // generate an interrupt
    SAC3DAC |= DACEN;                                   // Enable DAC
    SAC3OA = NMUXEN;                                    // SAC Negative input MUX controL
    SAC3OA |= PMUXEN;                                   // SAC Positive input MUX control
    SAC3OA |= PSEL_1;                                   // 12-bit reference DAC source selected
    SAC3OA |= NSEL_1;                                   // Select negative pin input
    SAC3OA |= OAPM;                                     // Select low speed and low power mode
    SAC3PGA = MSEL_1;                                   // Set OA as buffer mode
    SAC3OA |= SACEN;                                    // Enable SAC
    SAC3OA |= OAEN;                                     // Enable OA
}


// Init_Serial
// This function call initializations for the UART ports 0 and 1
void Init_Serial(void){
    Init_Serial_UCA0();
    Init_Serial_UCA1();
}

// Initialize Serial port 0
//------------------------------------------------------------------------------
void Init_Serial_UCA0(void){
    // Init serial for IOT Module
    UCA0CTLW0  =  0;
    UCA0CTLW0 |=  UCSWRST ;                             // Put eUSCI in reset
    UCA0CTLW0 |=  UCSSEL__SMCLK;                        // Set SMCLK as fBRCLK
    UCA0CTLW0 &= ~UCMSB;                                // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB;                                // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA0CTLW0 &= ~UCPEN;                                // No Parity
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |=  UCMODE_0;

    // Default is 115200 BAUD
    UCA0BRW    =  4;
    UCA0MCTLW  =  0x5551;

    UCA0CTLW0 &= ~UCSWRST;                              // release from reset
    UCA0TXBUF  =  0x00;                                 // Prime the Pump
    UCA0IE    |=  UCRXIE;                               // Enable RX interrupt
//------------------------------------------------------------------------------
}

// Initialize Serial port 1
//------------------------------------------------------------------------------
void Init_Serial_UCA1(void){   
    // Init serial for USB Backchannel
    UCA1CTLW0  =  0;
    UCA1CTLW0 |=  UCSWRST ;                             // Put eUSCI in reset
    UCA1CTLW0 |=  UCSSEL__SMCLK;                        // Set SMCLK as fBRCLK
    UCA1CTLW0 &= ~UCMSB;                                // MSB, LSB select
    UCA1CTLW0 &= ~UCSPB;                                // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA1CTLW0 &= ~UCPEN;                                // No Parity
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |=  UCMODE_0;

    // Default is 115200 BAUD
    UCA1BRW    =  4;
    UCA1MCTLW  =  0x5551;

    UCA1CTLW0 &= ~UCSWRST;                              // release from reset
    UCA1TXBUF  =  0x00;                                 // Prime the Pump
    UCA1IE    |=  UCRXIE;                               // Enable RX interrupt
//------------------------------------------------------------------------------
}


//------------------------------------------------------------------------------
// These functions initalize the timers
// This fuction calls initialization for all the timers
void Init_Timers(void){
    Init_Timer_B0();
    Init_Timer_B1();
    Init_Timer_B2();
    Init_Timer_B3();

    timer_B0.ccr0_period = TB0_0_INTERVAL;
    timer_B0.tick_ready = FALSE;
}

// Initalize timer B0
// This function starts timer B0 counting up in continuous mode from 0x0 to 0xFFFF
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK;             // SMCLK source = 8MHz
    TB0CTL |= TBCLR;                    // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS;            // Continuous up
    TB0CTL |= ID__8;                    // Divide clock by 8
    TB0EX0 = TBIDEX__8;                 // Divide clock by an additional 8
    TB0CCR0 = TB0_0_INTERVAL;           // CCR0
    TB0CCTL0 &= ~CCIE;                   // CCR0 enable interrupt
    TB0CTL |=  TBIE;                    // Enable Overflow Interrupt. For PU
    TB0CTL &= ~TBIFG;                   // Clear Overflow Interrupt flag
}

// Initalize timer B1
// This function starts timer B0 counting up in continuous mode from 0x0 to 0xFFFF
void Init_Timer_B1(void) {
    TB1CTL = TBSSEL__SMCLK;             // SMCLK source
    TB1CTL |= TBCLR;                    // Resets TB0R, clock divider, count direction
    TB1CTL |= MC__CONTINOUS;            // Continuous up
    TB1CTL |= ID__8;                    // Divide clock by 8
    TB1EX0 = TBIDEX__8;                 // Divide clock by an additional 8
    TB1CCR0 = TB1_0_INTERVAL;           // CCR0
    TB1CCTL0 &= ~CCIE;                  // CCR0 enable interrupt
    TB1CTL &= ~TBIE;                    // Disable overflow interupt
    TB1CTL &= ~TBIFG;                   // Clear Overflow Interrupt flag
}

// Initalize timer B2
// This function starts timer B0 counting up in continuous mode from 0x0 to 0xFFFF
void Init_Timer_B2(void) {
    TB2CTL = TBSSEL__SMCLK;             // SMCLK source
    TB2CTL |= TBCLR;                    // Resets TB0R, clock divider, count direction
    TB2CTL |= MC__CONTINOUS;            // Continuous up
    TB2CTL |= ID__8;                    // Divide clock by 8
    TB2EX0 = TBIDEX__8;                 // Divide clock by an additional 8
    TB2CCR0 = TB2_0_INTERVAL;           // CCR0
    TB2CCTL0 &= ~CCIE;                  // CCR0 disable interrupt
    TB2CTL &= ~TBIE;                    // Disable Overflow Interrupt
    TB2CTL &= ~TBIFG;                   // Clear Overflow Interrupt flag
}

// Initalize timer B3
// This function starts timer B3 generating PWM
void Init_Timer_B3(void) {
    // TB3.2 P6.1 R_FORWARD
    // TB3.3 P6.2 R_REVERSE
    // TB3.4 P6.3 L_FORWARD
    // TB3.5 P6.4 L_REVERSE
    TB3CTL = TBSSEL__SMCLK;              // Set clock source to SMCLK (8MHz)
    TB3CTL |= MC__UP;                    // Up Mode
    TB3CTL |= TBCLR;                     // Clear Timer
    TB3CCR0 = WHEEL_PERIOD;              // Set PWM Period (50,000)
    TB3CCTL1 = OUTMOD_7;                 // CCR1 Reset/set
    LCD_BRIGHTNESS = OFF;          // Full brightness is WHEEL_PERIOD, off is 0
    TB3CCTL2 = OUTMOD_7;                 // CCR2 reset/set
    RIGHT_FORWARD_SPEED = OFF;     // P6.0 Right Forward PWM duty cycle
    TB3CCTL3 = OUTMOD_7;                 // CCR3 reset/set
    RIGHT_REVERSE_SPEED = OFF;     // P6.1 Right Reverse PWM duty cycle
    TB3CCTL4 = OUTMOD_7;                 // CCR4 reset/set
    LEFT_FORWARD_SPEED = OFF;      // P6.2 Left Forward PWM duty cycle
    TB3CCTL5 = OUTMOD_7;                 // CCR5 reset/set
    LEFT_REVERSE_SPEED = OFF;      // P6.3 Left Reverse PWM duty cycle
}



//------------------------------------------------------------------------------
// Initalize the ADC
//------------------------------------------------------------------------------
void Init_ADC(void){
    // ADCCTL0 Register
    ADCCTL0 = 0;                // Reset
    ADCCTL0 |= ADCSHT_2;        // 16 ADC clocks
    ADCCTL0 |= ADCMSC;          // MSC
    ADCCTL0 |= ADCON;           // ADC ON

    // ADCCTL1 Register
    ADCCTL2 = 0; // Reset
    ADCCTL1 |= ADCSHS_0;        // 00b = ADCSC bit
    ADCCTL1 |= ADCSHP;          // ADC sample-and-hold SAMPCON signal from sampling timer.
    ADCCTL1 &= ~ADCISSH;        // ADC invert signal sample-and-hold.
    ADCCTL1 |= ADCDIV_0;        // ADC clock divider - 000b = Divide by 1
    ADCCTL1 |= ADCSSEL_0;       // ADC clock MODCLK
    ADCCTL1 |= ADCCONSEQ_0;     // ADC conversion sequence 00b = Single-channel single-conversion
    // ADCCTL1 & ADCBUSY identifies a conversion is in process

    // ADCCTL2 Register
    ADCCTL2 = 0; // Reset
    ADCCTL2 |= ADCPDIV0;        // ADC pre-divider 00b = Pre-divide by 1
    ADCCTL2 |= ADCRES_2;        // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
    ADCCTL2 &= ~ADCDF;          // ADC data read-back format 0b = Binary unsigned.
    ADCCTL2 &= ~ADCSR;          // ADC sampling rate 0b = ADC buffer supports up to 200 ksps

    // ADCMCTL0 Register
    ADCMCTL0 |= ADCSREF_0;      // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
    ADCMCTL0 |= ADCINCH_2;      // Left IR
    ADCIE    |=  ADCIE0;        // disable ADC conv complete interrupt
    ADCCTL0  &= ~ADCENC;        // ADC disable conversion.
//    ADCCTL0 |= ADCSC;         // ADC start conversion.
}
