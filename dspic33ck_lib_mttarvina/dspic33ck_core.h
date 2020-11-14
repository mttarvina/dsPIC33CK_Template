/* 
 * File:            dspic33ck_core.h
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is a custom header file for device initialization and basic pin settings
 */


#ifndef _DSPIC33CK_CORE_H
#define	_DSPIC33CK_CORE_H


#include <xc.h>
#include <stdbool.h>
#include "dspic33ck_time.h"
#include "dspic33ck_analog.h"
#include "dspic33ck_pwm.h"


// SYSTEM MACROS
#define WDT_CLR_KEY                         0x5743

#define _8MHZ                               8000000UL
#define _16MHZ                              16000000UL
#define _20MHZ                              20000000UL
#define _50MHZ                              50000000UL
#define _100MHZ                             100000000UL

//#define CLOCK_SystemFrequencyGet()          (8000000UL)       // 8 MHz
//#define CLOCK_SystemFrequencyGet()          (16000000UL)      // 16 MHz
//#define CLOCK_SystemFrequencyGet()          (20000000UL)      // 20 MHz
#define CLOCK_SystemFrequencyGet()          (50000000UL)      // 50 MHz
//#define CLOCK_SystemFrequencyGet()          (100000000UL)     // 100 MHz

#define CLOCK_PeripheralFrequencyGet()      (CLOCK_SystemFrequencyGet() / 2)
#define CLOCK_InstructionFrequencyGet()     (CLOCK_SystemFrequencyGet() / 2)


// CUSTOM USER MACROS
#define CLKOUT_EN                           false                                // display CLKOUT at RB1 pin
#define TIMER1_EN                           true                                // enables usage of delay_ms()
#define SCCP1_EN                            true                                // enables usage of milliseconds() and seconds()
#define SCCP2_EN                            true                                // enables usage of sccp2 as sampling trigger for ADC


#define INPUT                               true                                // used in Digital_SetPin()
#define OUTPUT                              false                               // used in Digital_SetPin()
#define LOW                                 false                               // used in Digital_DrvPin()
#define HIGH                                true                                // used in Digital_DrvPin()


// GPIO PIN MACROS
#define PORT_A                              0x1
#define PORT_B                              0x2

#define PA0                                 0x01
#define PA1                                 0x02
#define PA2                                 0x03
#define PA3                                 0x04
#define PA4                                 0x05
#define PB0                                 0x06
#define PB1                                 0x07
#define PB2                                 0x08
#define PB3                                 0x09
#define PB4                                 0x0A
#define PB5                                 0x0B
#define PB6                                 0x0C
#define PB7                                 0x0D
#define PB8                                 0x0E
#define PB9                                 0x0F
#define PB10                                0x10
#define PB11                                0x11
#define PB12                                0x12
#define PB13                                0x13
#define PB14                                0x14
#define PB15                                0x15


/**
  @Description
    Initialize system

  @Param
    None

  @Returns
    None
*/
void SYS_INIT( void );

/**
  @Description
    Initialize system clock at startup

  @Param
    None

  @Returns
    None
*/
void SYSCLOCK_Initialize( void );

/**
  @Description
    Initialize default GPIO settings at startup

  @Param
    None

  @Returns
    None
*/
void GPIO_Initialize( void );

/**
  @Description
    Sets the pin direction to either OUTPUT or INPUT

  @Param
    port = [PORT_A, PORT_B, ...]
    pin = [RA0-RAx, RB0-RBx, ...]
    dir = OUTPUT/INPUT

  @Returns
    None
*/
void Digital_SetPin( uint8_t _pin, bool _dir );

/**
  @Description
    Drive the output pin to either HIGH or LOW

  @Param
    port = [PORT_A, PORT_B, ...]
    pin = [RA0-RAx, RB0-RBx, ...]
    dir = HIGH/LOW

  @Returns
    None
*/
void Digital_DrvPin( uint8_t _pin, bool _state );

/**
  @Description
    Reads the value of digital input pin as either HIGH or LOW

  @Param
    pin = [RA0-RAx, RB0-RBx, ...]

  @Returns
    digital pin state as HIGH or LOW
*/
bool Digital_ReadPin( uint8_t _pin );

#endif	/* _DSPIC33_CORE_H */

