/* 
 * File:            dspic33ck_pwm.h
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is a custom header file for PWM related routines
 */


#ifndef _DSPIC33CK_PWM_H
#define _DSPIC33CK_PWM_H


#include <xc.h>
#include "dspic33ck_core.h"


typedef struct _PWM_OBJ_STRUCT {
    volatile uint8_t        port;
    volatile uint16_t       pin;
    volatile uint16_t       period;
    volatile float          duty_factor;
} PWM_OBJ;


/**
  @Description
    Assigns a 16bit number for LATA/LATB corresponding to the input pin
  @Param
    _pin = [PA0-PA4, PB0-PB15]

  @Returns
    16bit number to be assigned to LATA/LATB
*/
uint16_t PWMx_GetPinRegister( uint8_t _pin );


// *****************************************************************************
// SCCP3 Routines - used as PWMA
//
// *****************************************************************************

/**
  @Description
    Initializes SCCP3 as PWMA Module
    Make sure to set the pin as OUTPUT first
    Assign the output pin and period count, sets initial duty cycle = 0

    Maximum PWM period range per Fosc:
    8MHz => 16ms
    16MHz => 8ms
    20MHz => 6.5ms
    50MHz => 2.6ms
    100MHz => 1.3ms

  @Param
    _pin = [PA0-PA4, PB0-PB15]
    _period_us = PWM period in us

  @Returns
    None
*/
void PWMA_Attach( uint8_t _pin, uint16_t _period_us );

/**
  @Description
    Start SCCP3/PWMA Module

  @Param
    None.

  @Returns
    None
*/
void PWMA_Start( void );

/**
  @Description
    Stop SCCP3/PWMA Module

  @Param
    None.

  @Returns
    None
*/
void PWMA_Stop( void );

/**
  @Description
    Set the duty cycle count of PWMA
    Should be less than the period count

  @Param
    _duty = [0 - 4096] which translates to 0 - 100% duty cycle

  @Returns
    None
*/
void PWMA_SetDuty( uint16_t _duty );


#endif //_DSPIC33CK_PWM_H

