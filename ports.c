//------------------------------------------------------------------------------
//
//  Description: This file contains the ports initalization functions
//
//  James Sorber
//  1/27/2023
//  Modified 4/12/2023
//  Built with Code Composer Studio (12.4.0)
//------------------------------------------------------------------------------

// Include macros
#include "header_files/ports.h"
#include  "msp430.h"

// Function Declarations
void Init_Ports(void);
void Init_Port_1(void);
void Init_Port_2(void);
void Init_Port_3(void);
void Init_Port_4(void);
void Init_Port_5(void);
void Init_Port_6(void);


// Function Definitions
//------------------------------------------------------------------------------
// Initalize all ports
void Init_Ports(void){
    Init_Port_1();
    Init_Port_2();
    Init_Port_3();
    Init_Port_4();
    Init_Port_5();
    Init_Port_6();

}
//------------------------------------------------------------------------------
// Initalize port 1
void Init_Port_1(void){
    P1OUT = 0x00;              // Set output reg to zeros
    P1DIR = 0x00;              // Set direction reg to inputs

    P1SEL0 &= ~RED_LED;
    P1SEL1 &= ~RED_LED;
    P1DIR  |=  RED_LED;
    P1OUT  &= ~RED_LED;

    P1SELC |=  A1_SEEED;

    P1SELC |=  V_DETECT_L;

    P1SELC |=  V_DTECT_R;

    P1SELC |=  A4_SEEED;
 
    P1SELC |=  V_THUMB;

    P1SEL0 |=  UCA0RXD;
    P1SEL1 &= ~UCA0RXD;

    P1SEL0 |=  UCA0TXD;
    P1SEL1 &= ~UCA0TXD;
}
//------------------------------------------------------------------------------
// Initalize port 2
void Init_Port_2(void){
    P2OUT = 0x00;
    P2DIR = 0x00;

    P2SEL0 &= ~RESET_LCD;
    P2SEL1 &= ~RESET_LCD;
    P2DIR  |=  RESET_LCD;
    P2OUT  &= ~RESET_LCD;

    P2SEL0 &= ~CHECK_BATT;
    P2SEL1 &= ~CHECK_BATT;
    P2DIR  |=  CHECK_BATT;
    P2OUT  &= ~CHECK_BATT;

    P2SEL0 &= ~IR_LED;
    P2SEL1 &= ~IR_LED;
    P2DIR  |=  IR_LED;
    P2OUT  &= ~IR_LED;    

    P2SEL0 &= ~SW2;
    P2SEL1 &= ~SW2;
    P2DIR  &= ~SW2;
    P2REN  |=  SW2;
    P2OUT  |=  SW2;
    P2IES  |=  SW2;       // SW1 Hi/Lo edge interrupt
    P2IFG  &= ~SW2;       // IFG SW1 cleared
    P2IE   |=  SW2;       // SW1 interrupt Enabled

    P2SEL0 &= ~IOT_RUN_CPU;
    P2SEL1 &= ~IOT_RUN_CPU;
    P2DIR  |=  IOT_RUN_CPU;
    P2OUT  &= ~IOT_RUN_CPU;

    P2SEL0 &= ~DAC_EN;
    P2SEL1 &= ~DAC_EN;
    P2DIR  |=  DAC_EN;
    P2OUT  |=  DAC_EN;

    P2SEL0 &= ~XOUT;
    P2SEL1 |=  XOUT;

    P2SEL0 &= ~XIN;
    P2SEL1 |=  XIN;    
}
//------------------------------------------------------------------------------
// Initalize port 3
void Init_Port_3(void){
    P3DIR = 0x00;
    P3OUT = 0x00;

    P3SEL0 &= ~TEST_PROBE;
    P3SEL1 &= ~TEST_PROBE;
    P3DIR  |=  TEST_PROBE;
    P3OUT  &= ~TEST_PROBE;

    P3SEL0 &= ~OA2O;
    P3SEL1 &= ~OA2O;
    P3DIR  &= ~OA2O;

    P3SEL0 &= ~OA2N;
    P3SEL1 &= ~OA2N;
    P3DIR  &= ~OA2O;

    P3SEL0 &= ~OA2P;
    P3SEL1 &= ~OA2P;
    P3DIR  &= ~OA2P;

    P3SEL0 &= ~SMCLK;
    P3SEL1 &= ~SMCLK;
    P3DIR  |=  SMCLK;
    P3OUT  &= ~SMCLK;

    P3SEL0 &= ~DAC_CNTL;
    P3SEL1 &= ~DAC_CNTL;
    P3DIR  &= ~DAC_CNTL;

    P3SEL0 &= ~IOT_LINK_CPU;
    P3SEL1 &= ~IOT_LINK_CPU;
    P3DIR  |=  IOT_LINK_CPU;
    P3OUT  &= ~IOT_LINK_CPU;

    P3SEL0 &= ~IOT_EN_CPU;
    P3SEL1 &= ~IOT_EN_CPU;
    P3DIR  |=  IOT_EN_CPU;
    P3OUT  &= ~IOT_EN_CPU;
}
//------------------------------------------------------------------------------
// Initalize port 4
void Init_Port_4(void){
    P4DIR = 0x00;
    P4OUT = 0x00;

    P4SEL0 &= ~DAC_CONT1;
    P4SEL1 &= ~DAC_CONT1;
    P4DIR  &= ~DAC_CONT1;

    P4SEL0 &= ~SW1;
    P4SEL1 &= ~SW1;
    P4DIR  &= ~SW1;
    P4REN  |=  SW1;
    P4OUT  |=  SW1;
    P4IES  |=  SW1;       // SW1 Hi/Lo edge interrupt
    P4IFG  &=  ~SW1;      // IFG SW1 cleared
    P4IE   |=  SW1;       // SW1 interrupt Enabled

    P4SEL0 |=  BCL_RXD;
    P4SEL1 &= ~BCL_RXD;

    P4SEL0 |=  BCL_TXD;
    P4SEL1 &= ~BCL_TXD;

    P4SEL0 &= ~UCB1_CS_LCD;
    P4SEL1 &= ~UCB1_CS_LCD;
    P4DIR  |=  UCB1_CS_LCD;
    P4OUT  |=  UCB1_CS_LCD;

    P4SEL0 |=  UCB1SCK;
    P4SEL1 &= ~UCB1SCK;

    P4SEL0 |=  UCB1SIMO;
    P4SEL1 &= ~UCB1SIMO;

    P4SEL0 |=  UCB1SOMI;
    P4SEL1 &=  UCB1SOMI;
    }
//------------------------------------------------------------------------------
// Initalize port 5
void Init_Port_5(void){
    P5DIR = 0x00;
    P5OUT = 0x00;

    P5SELC |=  V_BATT;

    P5SELC |=  V5;

    P5SELC |=  V_DAC;

    P5SELC |=  V_3_3;

    P5SEL0 &= ~IOT_BOOT_CPU;
    P5SEL1 &= ~IOT_BOOT_CPU;
    P5DIR  |=  IOT_BOOT_CPU;
    P5OUT  |=  IOT_BOOT_CPU;
}
//------------------------------------------------------------------------------
// Initalize port 6
void Init_Port_6(void){
    P6DIR = 0x00;
    P6OUT = 0x00;

    P6SEL0 |=  LED_BACKLITE;
    P6SEL1 &= ~LED_BACKLITE;
    P6DIR  |=  LED_BACKLITE;

    P6SEL0 |=  R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6DIR  |=  R_FORWARD;

    P6SEL0 |=  R_REVERSE;
    P6SEL1 &= ~R_REVERSE;
    P6DIR  |=  R_REVERSE;
 
    P6SEL0 |=  L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6DIR  |=  L_FORWARD;

    P6SEL0 |=  L_REVERSE;
    P6SEL1 &= ~L_REVERSE;
    P6DIR  |=  L_REVERSE;

    P6SEL0 &= ~P6_5;        // Unused pin
    P6SEL1 &= ~P6_5;
    P6DIR  &= ~P6_5;

    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6DIR  |=  GRN_LED;
    P6OUT  &= ~GRN_LED;
}
