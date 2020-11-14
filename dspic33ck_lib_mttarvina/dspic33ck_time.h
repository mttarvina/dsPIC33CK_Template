/* 
 * File:            dspic33ck_time.h
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is a custom header file for timing/delay related routines
 */

#ifndef _DSPIC33CK_TIME_H
#define _DSPIC33CK_TIME_H


#include <xc.h>
#include "dspic33ck_core.h"


#define TMR1_INTERRUPT_TICKER_FACTOR    1


typedef struct _TMR_OBJ_STRUCT {
    volatile uint16_t       count;
    volatile uint16_t       count_by_10;
} TMR_OBJ;


typedef struct _SCCP1_TMR_OBJ_STRUCT {
    volatile uint16_t                   count_buf;
    volatile unsigned long long int     count;
    volatile unsigned long long int     count_by_1k;
} SCCP1_TMR_OBJ;


// *****************************************************************************
// TIMER1 Function Call Definitions
// TIMER1 timer period = 5us
// *****************************************************************************

/**
  @Description
    Initializes the TIMER1 Module

  @Param
    None.

  @Returns
    None
*/
void TIMER1_Initialize( void );

/**
  @Description
    Starts the TIMER1 Module

  @Param
    None.

  @Returns
    None
*/
void TIMER1_Start( void );

/**
  @Description
    Stops the TIMER Module

  @Param
    None.

  @Returns
    None
*/
void TIMER1_Stop( void );

/**
  @Description
    Clear the counter parameters of Timer1 object

  @Param
    None.

  @Returns
    None
*/
void TIMER1_CountReset( void );

/**
  @Description
    Uses TIMER1. Delay for specified duration in 1ms resolution
        the 1.2us error is very insignificant here

  @Param
    uint16_t duration - duration in ms

  @Returns
    None
*/
void delay_ms( uint16_t _duration );


// *****************************************************************************
// SCCP1 Function Call Definitions
// SCCP2 timer period = 1ms
// *****************************************************************************

/**
  @Description
    Initialize SCCP1 as Timer

  @Param
    None.

  @Returns
    None
*/
void SCCP1_Initialize (void);

/**
  @Description
    This routine starts the SCCP1 Timer

  @Param
    None.

  @Returns
    None
*/
void SCCP1_Start( void );

/**
  @Description
    This routine stops the SCCP1 Timer

  @Param
    None.

  @Returns
    None
*/
void SCCP1_Stop( void );

/**
  @Description
    Reset counter parameters of SCCP1

  @Param
    None.

  @Returns
    None
*/
void SCCP1_CountReset( void );
/**
  @Description
    Returns elapsed time in milliseconds 

  @Param
    None.

  @Returns
    unsigned long long int
*/
unsigned long long milliseconds( void );

/**
  @Description
    Returns elapsed time in seconds

  @Param
    None.

  @Returns
    unsigned long long int
*/
unsigned long long seconds( void );     

#endif