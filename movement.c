//------------------------------------------------------------------------------

//  Description: This file contains the code to translate accelerometer values into movements
//  Author:     James Sorber
//  Created:    1/28/24
//  Modified:   1/28/24
//  Last built with Code Composer Studio v12.4.0
//------------------------------------------------------------------------------

#include "msp430.h"
#include "functions.h"
#include "macros.h"
#include "port_macros.h"

// Variables
//------------------------------------------------------------------------------
extern unsigned char motors_enabled;
extern unsigned int avg_x_val;
extern unsigned int avg_y_val;

char pwm_2_usb[7];


// Calculate_Movement
// This function takes two unsigned int values, x and y
// x and y repersent acceleration vectors from a phone
// x and y are essentially mapped to an xy plane and a (very) crude arctan
// operation is used to calculate angle of resulting vector
// relative speeds of motors are than set based on this angle
//------------------------------------------------------------------------------
void Calculate_Movement(void){
    P3OUT |= TEST_PROBE;
    unsigned int prescale_values[4];
    int accel_vector[2];
    float linear_velocity = 0;
    float angular_velocity = 0;
    float w_left = 0;
    float w_right = 0;
    long int motor_speeds[2];
    // Order is
    // unsigned int left_forward_prescale;
    // unsigned int left_reverse_prescale;
    // unsigned int right_forward_prescale;
    // unsigned int right_reverse_prescale;

    // Clip to stay below maximums
    if(avg_x_val > 255) avg_x_val = 255;
    if(avg_y_val > 255) avg_y_val = 255;

    // Scale to be centered at origin, flip so +x +y is in NE quadrent
    accel_vector[0] = 128 - avg_x_val;
    accel_vector[1] = 128 - avg_y_val;

    // Calculate linear and angular velocity
    linear_velocity = (float)accel_vector[1] / HYPOT;
    angular_velocity = (float)accel_vector[0] / HYPOT;

    // Calculate left and right wheel speeds
    w_left = linear_velocity + angular_velocity;
    w_right = linear_velocity - angular_velocity;

    // Convert to int for motors
    motor_speeds[0] = (int)(w_left * 32000);
    motor_speeds[1] = (int)(w_right * 32000);

    // Convert LEFT from +/- to 2 positive values
    if(motor_speeds[0] >= 0){
        prescale_values[LEFT_F] = motor_speeds[0] * 2;
        prescale_values[LEFT_R] = OFF;
    }
    else{
        motor_speeds[0] = motor_speeds[0] * -1;
        prescale_values[LEFT_F] = OFF;
        prescale_values[LEFT_R] = motor_speeds[0] * 2;
    }
    // Convert RIGHT from +/- to 2 positive values
    if(motor_speeds[1] >= 0){
        prescale_values[RIGHT_F] = motor_speeds[1] * 2;
        prescale_values[RIGHT_R] = OFF;
    }
    else{
        motor_speeds[1] = motor_speeds[1] * -1;
        prescale_values[RIGHT_F] = OFF;
        prescale_values[RIGHT_R] = motor_speeds[1] * 2;
    }

    // Clip values to add dead spot and to ensure stay below 60k
    char i;
    for(i = 0; i<4; i++){
        if(prescale_values[i] > MAX_PWM){
            prescale_values[i] = MAX_PWM;
        }
        else if((prescale_values[i] < MIN_PWM) && (prescale_values[i] > 0)){
            prescale_values[i] = MIN_PWM;
        }
    }

    // Run an idiot check to prevent magic smoke release
    if((prescale_values[LEFT_F] && prescale_values[LEFT_R]) || (prescale_values[RIGHT_F] && prescale_values[RIGHT_R])){
        LEFT_FORWARD_SPEED = OFF;
        LEFT_REVERSE_SPEED = OFF;
        RIGHT_FORWARD_SPEED = OFF;
        RIGHT_REVERSE_SPEED = OFF;
        P1OUT |= RED_LED;
        motors_enabled = FALSE;
        return;
    }

    // Set motor PWM registers
    LEFT_FORWARD_SPEED = prescale_values[LEFT_F];
    LEFT_REVERSE_SPEED = prescale_values[LEFT_R];
    RIGHT_FORWARD_SPEED = prescale_values[RIGHT_F];
    RIGHT_REVERSE_SPEED = prescale_values[RIGHT_R];

    P3OUT &= ~TEST_PROBE;
}

