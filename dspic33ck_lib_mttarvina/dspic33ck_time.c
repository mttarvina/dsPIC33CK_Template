/* 
 * File:            dspic33ck_time.c
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     Source code for timing/delay related routines
 */


#include <xc.h>
#include "dspic33ck_time.h"


// *****************************************************************************
// TIMER1 Routines
// TIMER1 resolution = 5us
// - used by delay_ms()
// *****************************************************************************

static TMR_OBJ timer1_obj;                                                      // Create default timer1 object


void TIMER1_Initialize (void) {
    
    IPC0bits.T1IP = 3;                                                          // Set TIMER1 Interrupt Priority: 7
         
    TMR1 = 0x00;                                                                // TMR 1; 

    if (CLOCK_SystemFrequencyGet() == _8MHZ) {
        PR1 = 0x18F;
    }
    else if (CLOCK_SystemFrequencyGet() == _16MHZ) {
        PR1 = 0x31F;
    }
    else if (CLOCK_SystemFrequencyGet() == _20MHZ) {
        PR1 = 0x3E7;
    }
    else if (CLOCK_SystemFrequencyGet() == _50MHZ) {
        PR1 = 0x9C3;
    }
    else if (CLOCK_SystemFrequencyGet() == _100MHZ) {
        PR1 = 0x1387;
    }
                        
    T1CON = 0x0000;                                                             // TCKPS 1:1; PRWIP Write complete; TMWIP Write complete; TON disabled; TSIDL disabled; TCS External; TECS FOSC; TSYNC disabled; TMWDIS disabled; TGATE disabled; 

    TIMER1_Start();
}


void TIMER1_Start( void ) {
    TIMER1_CountReset();
    IFS0bits.T1IF = false;                                                      // reset timer1 interrupt flag
    IEC0bits.T1IE = true;                                                       // enable timer1 interrupt
    T1CONbits.TON = true;
}


void TIMER1_Stop( void ) {
    T1CONbits.TON = false;
    IEC0bits.T1IE = false;
}


void TIMER1_CountReset( void ) {
    timer1_obj.count = 0;                                                       // reset counter parameters
    timer1_obj.count_by_10 = 0;                                                 // reset counter parameters
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt() {                                                      
    timer1_obj.count++;
    
    if (timer1_obj.count > 0xFFFE) {
        timer1_obj.count = 0;
    }

    IFS0bits.T1IF = false;
}


void delay_ms( uint16_t _duration ){
    TIMER1_CountReset();
    while (timer1_obj.count_by_10 < _duration) {
        if (timer1_obj.count > 0x9) {                                           // if > 9
            timer1_obj.count = 0;
            timer1_obj.count_by_10++;
        }
    }
}




// *****************************************************************************
// SCCP1 Routines
// SCCP1 resolution = 1ms
// - used by milliseconds()
// - used by seconds()
// *****************************************************************************

static SCCP1_TMR_OBJ sccp1_obj;


void SCCP1_Initialize( void ) {
    
    //    CCPI: CCP1 Capture/Compare Event
    //    Priority: 2
    IPC1bits.CCP1IP = 2;
    //    CCTI: CCP1 Timer Event
    //    Priority: 2
    IPC1bits.CCT1IP = 2;
        
    // CCPON disabled; MOD 16-Bit/32-Bit Timer;
    // CCSEL disabled; CCPSIDL disabled; T32 32 Bit;
    // CCPSLP disabled; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled;
    
    CCP1CON1L = (0x20 & 0x7FFF);                                                //Disabling CCPON bit,
    CCP1CON1H = 0x00;                                                           //RTRGEN disabled; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; OPS Each Time Base Period Match; SYNC None; OPSSRC Timer Interrupt Event;
    CCP1CON2L = 0x00;                                                           //ASDGM disabled; SSDG disabled; ASDG 0; PWMRSEN disabled; 
    CCP1CON2H = 0x00;                                                           //ICGSM Level-Sensitive mode; ICSEL IC1; AUXOUT Disabled; OCAEN disabled; OENSYNC disabled; 
    CCP1CON3H = 0x00;                                                           //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state;
    CCP1STATL = 0x00;                                                           //ICDIS disabled; SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; TRCLR disabled; 
    CCP1TMRL = 0x00;                                                            //TMR 0;
    CCP1TMRH = 0x00;                                                            //TMR 0;
    
    if (CLOCK_SystemFrequencyGet() == _8MHZ) {
        CCP1PRL = 0xF9F;
    }
    else if (CLOCK_SystemFrequencyGet() == _16MHZ) {
        CCP1PRL = 0x1F3F;
    }
    else if (CLOCK_SystemFrequencyGet() == _20MHZ) {
        CCP1PRL = 0x270F;
    }
    else if (CLOCK_SystemFrequencyGet() == _50MHZ) {
        CCP1PRL = 0x61A7;
    }
    else if (CLOCK_SystemFrequencyGet() == _100MHZ) {
        CCP1PRL = 0xC34F;
    }

    CCP1PRH = 0x00;                                                             //PR 0;
    CCP1RA = 0x00;                                                              //CMP 0;
    CCP1RB = 0x00;                                                              //CMP 0;
    CCP1BUFL = 0x00;                                                            //BUF 0;
    CCP1BUFH = 0x00;                                                            //BUF 0; 

    SCCP1_Start();
}


void SCCP1_Start( void ) {
    SCCP1_CountReset();
    IFS0bits.CCP1IF = false;
    IFS0bits.CCT1IF = false;
      
    IEC0bits.CCP1IE = true;                                                     // Enabling SCCP1 interrupt.
    IEC0bits.CCT1IE = true;                                                     // Enabling SCCP1 interrupt.
    
    CCP1CON1Lbits.CCPON = true;
}


void SCCP1_Stop( void ) {
    CCP1CON1Lbits.CCPON = false;
}

void SCCP1_CountReset( void ) {
    sccp1_obj.count_buf = 0;                                                    // reset counter
    sccp1_obj.count = 0;                                                        // reset counter
    sccp1_obj.count_by_1k = 0;                                                  // reset counter
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT1Interrupt ( void ) {         
    sccp1_obj.count++;
    sccp1_obj.count_buf++;
            
    if (sccp1_obj.count_buf > 0x03E7){                                          // if > 999
        sccp1_obj.count_by_1k++;
        sccp1_obj.count_buf = 0;
    }
    IFS0bits.CCT1IF = false;
}


unsigned long long milliseconds( void ) {
    return sccp1_obj.count;
}


unsigned long long seconds( void ) {
    return sccp1_obj.count_by_1k; 
}