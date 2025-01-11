//------------------------------------------------------------------------------
// Description: This file contains the macros used movement.c
// Author: James Sorber
// Created  01/11/2025
// Modified 01/11/2025
// Built with Code Composer Studio v12.4.0
//------------------------------------------------------------------------------

#define WHEEL_PERIOD        (60000)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_FORWARD_SPEED  (TB3CCR4)
#define LEFT_REVERSE_SPEED  (TB3CCR5)

#define OFF                 (0x00)
#define ON_100_PRESCALE     (117)
#define SENSITIVITY         (2)
#define SCALE_FACTOR        (254)
#define CALC_THRES          (256)
#define ACCEL_AVG_BUF_SIZE  (3) 
#define MAX_PWM             (60000)
#define MIN_PWM             (9000)
#define LEFT_F              (0)
#define LEFT_R              (1)
#define RIGHT_F             (2)
#define RIGHT_R             (3)
#define HYPOT               (181)