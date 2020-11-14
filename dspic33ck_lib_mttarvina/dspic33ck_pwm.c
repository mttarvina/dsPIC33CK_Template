
/* 
 * File:            dspic33ck_pwm.c
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is the source code for PWM related routines
 */


#include <xc.h>
#include "dspic33ck_pwm.h"


uint16_t PWMx_GetPinRegister( uint8_t _pin ) {
    switch (_pin) {
        case PA0:
            return 0x0001;
        case PA1:
            return 0x0002;
        case PA2:
            return 0x0004;
        case PA3:
            return 0x0008;
        case PA4:
            return 0x0010;
        case PB0:
            return 0x0001;
        case PB1:
            return 0x0002;
        case PB2:
            return 0x0004;
        case PB3:
            return 0x0008;
        case PB4:
            return 0x0010;
        case PB5:
            return 0x0020;
        case PB6:
            return 0x0040;
        case PB7:
            return 0x0080;
        case PB8:
            return 0x0100;
        case PB9:
            return 0x0200;
        case PB10:
            return 0x0400;
        case PB11:
            return 0x0800;
        case PB12:
            return 0x1000;
        case PB13:
            return 0x2000;
        case PB14:
            return 0x4000;
        case PB15:
            return 0x8000;
    }
    return 0;
}


// *****************************************************************************
// SCCP3 Routines - used as PWMA
//
//
//
// *****************************************************************************

PWM_OBJ pwma;                                                                   // create PWMA object

void PWMA_Attach( uint8_t _pin, uint16_t _period_us ) {
    
    if ( _pin <= PA4 && _pin >= PA0) {
        pwma.port = PORT_A;
    }
    else if (_pin <= PB15 && _pin >= PB0) {
        pwma.port = PORT_B;
    }

    pwma.pin = PWMx_GetPinRegister(_pin);

    if (CLOCK_SystemFrequencyGet() == _8MHZ) {
        pwma.period = _period_us * 4;
    }
    else if (CLOCK_SystemFrequencyGet() == _16MHZ) {
        pwma.period = _period_us * 8;
    }
    else if (CLOCK_SystemFrequencyGet() == _20MHZ) {
        pwma.period = _period_us * 10;
    }
    else if (CLOCK_SystemFrequencyGet() == _50MHZ) {
        pwma.period = _period_us * 25;
    }
    else if (CLOCK_SystemFrequencyGet() == _100MHZ) {
        pwma.period = _period_us * 50;
    }

    pwma.duty_factor = pwma.period * 0.000244;                                  // period/4096

    //    CCPI: CCP3 Capture/Compare Event
    //    Priority: 1
    IPC8bits.CCP3IP = 1;
    //    CCTI: CCP3 Timer Event
    //    Priority: 1
    IPC9bits.CCT3IP = 1;
        
    // CCPON disabled; MOD Dual Edge Compare, Buffered(PWM);
    // CCSEL disabled; CCPSIDL disabled; T32 16 Bit; CCPSLP disabled;
    // TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled; 
    CCP3CON1L = (0x05 & 0x7FFF);                                                //Disabling CCPON bit 
    CCP3CON1H = 0x00;                                                           //RTRGEN disabled; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; OPS Each Time Base Period Match; SYNC None; OPSSRC Timer Interrupt Event;
    CCP3CON2L = 0x00;                                                           //ASDGM disabled; SSDG disabled; ASDG 0; PWMRSEN disabled;
    CCP3CON2H = 0x00;                                                           //ICGSM Level-Sensitive mode; ICSEL IC3; AUXOUT Disabled; OCAEN disabled; OENSYNC disabled; 
    CCP3CON3H = 0x00;                                                           //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state;
    CCP3STATL = 0x00;                                                           //ICDIS disabled; SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; TRCLR disabled; 
    CCP3TMRL = 0x00;                                                            //TMR 0;
    CCP3TMRH = 0x00;                                                            //TMR 0; 
    CCP3PRL = pwma.period;                                                      //PERIOD; 
    CCP3PRH = 0x00;                                                             //PR 0;
    CCP3RA = 0x00;                                                              //CMP 0; 
    CCP3RB = 0x00;                                                              //DUTY CYCLE; 
    CCP3BUFL = 0x00;                                                            //BUF 0; 
    CCP3BUFH = 0x00;                                                            //BUF 0; 

    // Clearing IF flag before enabling the interrupt.
    IFS2bits.CCP3IF = 0;
    // Enabling SCCP3 interrupt.
    IEC2bits.CCP3IE = 1;

    // Clearing IF flag before enabling the interrupt.
    IFS2bits.CCT3IF = 0;
    // Enabling SCCP3 interrupt.
    IEC2bits.CCT3IE = 1;
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCP3Interrupt ( void ) {
    switch (pwma.port) {
        case PORT_A:
            LATA = LATA & ~(pwma.pin);
            break;
        case PORT_B:
            LATB = LATB & ~(pwma.pin);
            break;
    }
    IFS2bits.CCP3IF = 0;
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT3Interrupt ( void ) {
    if (CCP3RB > 0) {
        switch (pwma.port) {
            case PORT_A:
                LATA = LATA | pwma.pin;
                break;
            case PORT_B:
                LATB = LATB | pwma.pin;
                break;
        }
    }
    IFS2bits.CCT3IF = 0;
}

void PWMA_Start( void ) {
    CCP3CON1Lbits.CCPON = true;
}

void PWMA_Stop( void ) {
    CCP3CON1Lbits.CCPON = false;
}


void PWMA_SetDuty( uint16_t _duty ) {
    if (_duty < 4096) {
        CCP3RB = (int) (_duty * pwma.duty_factor);
    }
}


/**
 End of File
*/
