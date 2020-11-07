/* 
 * File:            TemplateProject/dspic33_time.h
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is a custom config header file for timing related routines
 * 
 * Revision history:
 *      11/07/2020  --> reverted to delay_5us() to reduce jitter and error
 *      ************************************************************************
 *      11/04/2020  --> added SCCP1 timer routines based on MCC generated code
 *                  --> added milliseconds() and seconds() function definitions
 *                  --> delay_5us() changed to delay_us()
 *                  --> TIMER1 here is at 1us resolution with constant 1.2us error
 *                  --> This 1.2us error is totally insignificant when duration > 100us
 *      ************************************************************************
 *      11/02/2020  --> added delay_5us() function definition
 *                  --> TIMER1 here is at 5us resolution
 *      ************************************************************************
 *      11/01/2020  --> Initial creation based on TIMER1 MCC generated code
 *                  --> simplified for basic functional use
 *                  --> timer1 is set for 1ms resolution
 *                  --> added custom delay_ms() function definition
 */

#ifndef _DSPIC33_TIME_H
#define _DSPIC33_TIME_H


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>


#define TMR1_INTERRUPT_TICKER_FACTOR    1
#define COUNT_LIMIT                     0x00C7                                  // 199


// *****************************************************************************
// TIMER1 Function Call Definitions
// *****************************************************************************

/**
  @Description
    Initializes the TIMER1 Module

  @Param
    None.

  @Returns
    None
*/
void TIMER1_Initialize (void);

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
void delay_ms(uint16_t duration);

/**
  @Description
    Uses TIMER1. Delay for specified duration in 5us resolution

  @Param
    uint8_t duration - duration * 5us

  @Returns
    None
*/
void delay_5us(uint8_t duration);


// *****************************************************************************
// SCCP1 Function Call Definitions
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
    Returns elapsed time in milliseconds 

  @Param
    None.

  @Returns
    unsigned long long int
*/
int milliseconds (void);

/**
  @Description
    Returns elapsed time in seconds

  @Param
    None.

  @Returns
    unsigned long long int
*/
int seconds (void);
        

#endif