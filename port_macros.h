//------------------------------------------------------------------------------
//
//  Description: This file contains the macros used for the ports.c source file
//               The following macros are all bitmasks for setting values in
//               the port control registers
//
//  James Sorber
//  1/27/2023
//  Built with Code Composer Studio (12.4.0)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Port 1
#define RED_LED         (0x01)
#define A1_SEEED        (0x02)
#define V_DETECT_L      (0x04)
#define V_DTECT_R       (0x08)
#define A4_SEEED        (0x10)
#define V_THUMB         (0x20)
#define UCA0RXD         (0x40)
#define UCA0TXD         (0x80)

// Port 2
#define RESET_LCD       (0x01)
#define CHECK_BATT      (0x02)
#define IR_LED          (0x04)
#define SW2             (0x08)
#define IOT_RUN_CPU     (0x10)
#define DAC_EN          (0x20)
#define XOUT            (0x40)
#define XIN             (0x80)

// Port 3
#define TEST_PROBE      (0x01)
#define OA2O            (0x02)
#define OA2N            (0x04)
#define OA2P            (0x08)
#define SMCLK           (0x10)
#define DAC_CNTL        (0x20)
#define IOT_LINK_CPU    (0x40)
#define IOT_EN_CPU      (0x80)

// Port 4
#define DAC_CONT1       (0x01)
#define SW1             (0x02)
#define BCL_RXD         (0x04)
#define BCL_TXD         (0x08)
#define UCB1_CS_LCD     (0x10)
#define UCB1SCK         (0x20)
#define UCB1SIMO        (0x40)
#define UCB1SOMI        (0x80)

// Port 5
#define V_BATT          (0x01)
#define V5              (0x02)
#define V_DAC           (0x04)
#define V_3_3           (0x08)
#define IOT_BOOT_CPU    (0x10)

// Port 6
#define LED_BACKLITE    (0x01)
#define R_FORWARD       (0x02)
#define R_REVERSE       (0x04)
#define L_FORWARD       (0x08)
#define L_REVERSE       (0x10)
#define P6_5            (0x20)
#define GRN_LED         (0x40)
