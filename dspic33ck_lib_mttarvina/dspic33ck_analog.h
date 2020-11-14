/* 
 * File:            dspic33ck_analog.h
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is a custom header file for DAC and DAC related routines
 */


#ifndef _DSPIC33CK_ANALOG_H
#define _DSPIC33CK_ANALOG_H


#include <xc.h>
#include "dspic33ck_core.h"


#define ADC1_IS_SOFTWARE_TRIGGERED    true
#define ADC1_USES_INTERRUPT           false

#define AN0                           0x1   // pin RA0 - analog channel dedicated to Core0
#define AN1                           0x2   // pin RB2 - analog channel dedicated to Core1
#define AN2                           0x3   // pin RB7 - analog chared channel
#define AN3                           0x4   // pin RA3 - analog shared channel                  
#define AN4                           0x5   // pin RA4 - analog shared channel
#define AN5                           0x6   // pin RB0 - analog shared channel
#define AN6                           0x7   // pin RB1 - analog shared channel
#define AN7                           0x8   // pin RB2 - analog shared channel
#define AN8                           0x9   // pin RB3 - analog shared channel
#define AN9                           0xA   // pin RA2 - analog shared channel                   
#define AN10                          0xB   // pin RB8 - analog shared channel
#define AN11                          0xC   // pin RB9 - analog shared channel


typedef struct _SCCP2_TMR_OBJ_STRUCT    {
    volatile bool           state;
    volatile uint16_t       pr;
} SCCP2_TMR_OBJ;


typedef struct _ADC1_OBJ_STRUCT    {
    volatile bool           ADC1Initialized;
    volatile uint8_t        counter;
} ADC1_OBJ;


// *****************************************************************************
// SCCP2 Function Call Definitions
// SCCP2 default timer period = 20us
// *****************************************************************************

/**
  @Description
    Initialize SCCP2 as Timer

  @Param
    None.

  @Returns
    None
*/
void SCCP2_Initialize (void);

/**
  @Description
    This routine starts the SCCP2 Timer

  @Param
    None.

  @Returns
    None
*/
void SCCP2_Start( void );

/**
  @Description
    This routine stops the SCCP2 Timer

  @Param
    None.

  @Returns
    None
*/
void SCCP2_Stop( void );

/**
  @Description
    Set priority level of SCCP2
    Default = 1

  @Param
    None.

  @Returns
    None
*/
inline static void SCCP2_InterruptPrioritySet( uint8_t _priorityValue ) {
    IPC5bits.CCP2IP = 0x7 & _priorityValue;
    IPC6bits.CCT2IP = 0x7 & _priorityValue;
}

/**
  @Description
    Change the sampling rate or trigger rate of SCCP2
    Default = 50kHz
    
    Maximum PWM period range per Fosc:
    8MHz => 16ms
    16MHz => 8ms
    20MHz => 6.5ms
    50MHz => 2.6ms
    100MHz => 1.3ms

  @Param
    _period_us = period of desired sampling rate in us

  @Returns
    none
*/
void SCCP2_SetSamplingPeriod( uint16_t _period_us );

/**
  @Description
    Checks if SCCP2 is triggered

  @Param
    period_ns = period of desired sampling rate in ns

  @Returns
    none
*/
bool SCCP2_Triggered( void );


// *****************************************************************************
// AN0 - ADC Function Call Definitions
//
// *****************************************************************************


/**
  @Description
    Initialize the ADC Peripheral

  @Param
    None.

  @Returns
    None
*/
void ADC1_Initialize( void );

/**
  @Description
    Enable ADC Peripheral

  @Param
    None.

  @Returns
    None
*/
inline static void ADC1_Enable( void ) {
   ADCON1Lbits.ADON = 1;
}

/**
  @Description
    Disable ADC Peripheral

  @Param
    None.

  @Returns
    None
*/
inline static void ADC1_Disable( void ) {
   ADCON1Lbits.ADON = 0;
}

/**
  @Description
    Turn on ADC1 Core0 Power

  @Param
    None.

  @Returns
    None
*/
void ADC1_Core0PowerEnable( void );

/**
  @Description
    Turn off ADC1 Core0 Power

  @Param
    None.

  @Returns
    None
*/
void ADC1_Core0PowerDisable( void );

/**
  @Description
    Turn on ADC1 Core1 Power

  @Param
    None.

  @Returns
    None
*/
void ADC1_Core1PowerEnable( void );

/**
  @Description
    Turn off ADC1 Core1 Power

  @Param
    None.

  @Returns
    None
*/
void ADC1_Core1PowerDisable( void );

/**
  @Description
    Turn on ADC1 Shared Core Power

  @Param
    None.

  @Returns
    None
*/
void ADC1_SharedCorePowerEnable( void );

/**
  @Description
    Turn off ADC1 Shared Core Power

  @Param
    None.

  @Returns
    None
*/
void ADC1_SharedCorePowerDisable( void );

/**
  @Description
    Enable Software Trigger

  @Param
    None.

  @Returns
    None
*/
inline static void ADC1_SoftwareTriggerEnable( void ) {
   ADCON3Lbits.SWCTRG = 1;
}

/**
  @Description
    Disable Software Trigger

  @Param
    None.

  @Returns
    None
*/
inline static void ADC1_SoftwareTriggerDisable( void ) {
   ADCON3Lbits.SWCTRG = 0;
}

/**
  @Description
    Initializes the ADC core of the corresponding ADC channel
    Sets the pin to analog input

  @Param
    channel = ADC channel

  @Returns
    None
*/
void Analog_SetPin( uint8_t _channel );

/**
  @Description
    Read ADC value on specified channel

  @Param
    channel = ADC channel

  @Returns
    ADC value
*/
uint16_t ADC1_Read( uint8_t _channel );

/**
  @Description
    Gets ADC value from ADCBUFx register

  @Param
    channel = ADC channel

  @Returns
    ADCBUFx value
*/
uint16_t ADC1_GetConversionResult( uint8_t _channel );

/**
  @Description
    Returns true if ADC finished conversion 

  @Param
    channel = ADC channel

  @Returns
    Return true of ADC finished conversion
*/
bool ADC1_IsConversionComplete( uint8_t _channel);

/**
  @Description
    Set ADC interrupt priority
    Default = 1

  @Param
    None.

  @Returns
    None
*/
inline static void ADC1_InterruptPrioritySet( uint8_t _priorityValue ) {
    IPC22bits.ADCIP = 0x7 & _priorityValue;
}


// *****************************************************************************
// CMP1/DAC Function Call Definitions
//
// *****************************************************************************

/**
  @Description
    Initialize Comparator1 for DAC

  @Param
    None.

  @Returns
    None
*/
void CMP1_Initialize( void );

/**
  @Description
    Enable Comparator1/DAC

  @Param
    None.

  @Returns
    None
*/
void CMP1_Enable( void );

/**
  @Description
    Disable Comparator1/DAC

  @Param
    None.

  @Returns
    None
*/
void CMP1_Disable( void ) ;

/**
  @Description
    Set DAC value

  @Param
    uint16_t dacVal

  @Returns
    None
*/
void DAC_SetValue( uint16_t _dacVal );

/**
  @Description
    Enable DAC output

  @Param
    None.

  @Returns
    None
*/
void CMP1_EnableDACOutput( void );

/**
  @Description
    Disable DAC output

  @Param
    None.

  @Returns
    None
*/
void CMP1_DisableDACOutput( void );

/**
  @Description
    Initialize CMP1 and enable DAC output at pin RA3

  @Param
    None.

  @Returns
    None
*/
void DAC_Enable( void );

#endif

