/* 
 * File:            TemplateProject/dspic33_sys.h
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is a custom config header file for device initialization.
 * 
 * Revision history:
 *      11/04/2020  --> Added GPIO and System Clock initialization function definitions
 *      ************************************************************************
 *      11/02/2020  --> Modified clock setting to Fosc = 100MHz
 *      ************************************************************************
 *      11/01/2020  --> Initial creation of sys_config.c
 *                  --> Created SYS_INIT function
 *                  --> Added custom bit config settings
 */
  
#ifndef _DSPIC33_SYS_H
#define	_DSPIC33_SYS_H

#include <xc.h>
#include <stdbool.h>

// CUSTOM USER MACROS
#define DEBUG                               false


// SYSTEM MACROS
#define _XTAL_FREQ                          100000000UL
#define WDT_CLR_KEY                         0x5743

#define CLOCK_SystemFrequencyGet()          (100000000UL)
#define CLOCK_PeripheralFrequencyGet()      (CLOCK_SystemFrequencyGet() / 2)
#define CLOCK_InstructionFrequencyGet()     (CLOCK_SystemFrequencyGet() / 2)


// CUSTOM GPIO PIN MACROS
#define RA0                                 0x0001
#define RA1                                 0x0002
#define RA2                                 0x0004
#define RA3                                 0x0008
#define RA4                                 0x0010
#define RB0                                 0x0001
#define RB1                                 0x0002
#define RB2                                 0x0004
#define RB3                                 0x0008
#define RB4                                 0x0010
#define RB5                                 0x0020
#define RB6                                 0x0040
#define RB7                                 0x0080
#define RB8                                 0x0100
#define RB9                                 0x0200
#define RB10                                0x0400
#define RB11                                0x0800
#define RB12                                0x1000
#define RB13                                0x2000
#define RB14                                0x4000
#define RB15                                0x8000


// CUSTOM FUNCTION DEFINITIONS

/**
  @Description
    Initialize system

  @Param
    None

  @Returns
    None
*/
void SYS_INIT (void);

/**
  @Description
    Initialize default GPIO settings at startup

  @Param
    None

  @Returns
    None
*/
void GPIO_Initialize (void);

/**
  @Description
    Initialize system clock at startup

  @Param
    None

  @Returns
    None
*/
void SYSCLOCK_Initialize (void);


#endif	/* _DSPIC33_SYS_H */

