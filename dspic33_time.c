/* 
 * File:            TemplateProject/dspic33_time.c
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     Source code for timing related routines
 * 
 * Revision history:
 *      11/07/2020  --> reverted to delay_5us with TIMER1 resolution = 5us
 *                  --> this is to minimize jitter when using 1us resolution and to reduce error
 *      ************************************************************************
 *      11/04/2020  --> reduced TIMER1 resolution to 1us with 1.2us error added to the specified duration
 *                  --> this 1.2us error will be totally insignificant to if duration > 100us
 *                  --> changed 'count_by_200' to 'count_by_1k'
 *                  --> changed delay_5us() to delay_us()
 *                  --> changed COUNT_LIMIT from 199 to 999
 *                  --> added SCCP1 timer routines
 *                  --> created milliseconds() routine similar to millis() of Arduino
 *                  --> created seconds() routine as added functionality
 *      ************************************************************************
 *      11/02/2020  --> TIMER1 is set for 5us resoultion
 *                  --> added count_by_200 parameter in timer1 object
 *                  --> modified delay_ms() to accomodate change to timer1 object
 *                  --> created delay_5us() function
 *      ************************************************************************
 *      11/01/2020  --> Initial creation
 *                  --> derived from MCC generated code and was simplified for basic functional use
 *                  --> timer1 is set for 1ms resolution
 *                  --> created custom delay_ms() function
 */


#include "dspic33_time.h"


// *****************************************************************************
// TIMER1 Routines
//
// - used by delay_ms()
// - used by delay_5us()
// *****************************************************************************

typedef struct _TMR_OBJ_STRUCT {
    volatile uint16_t       count;
    volatile uint16_t       count_by_1k;
} TMR_OBJ;
static TMR_OBJ timer1_obj;                                                      // Create default timer1 object


void TIMER1_Initialize (void) {
    
    IPC0bits.T1IP = 1;                                                          // Set TIMER1 Interrupt Priority: 1
         
    TMR1 = 0x00;                                                                // TMR 1; 
    PR1 = 0x1F3;                                                                // Period = 0.000005 s; Frequency = 100000000 Hz; PR 499;                        
    T1CON = 0x0202;                                                             // TCKPS 1:1; PRWIP Write complete; TMWIP Write complete; TON disabled; TSIDL disabled; TCS External; TECS FOSC; TSYNC disabled; TMWDIS disabled; TGATE disabled; 

    IFS0bits.T1IF = false;                                                      // reset timer1 interrupt flag
    IEC0bits.T1IE = true;                                                       // enable timer1 interrupt
    
    TIMER1_CountReset();
    TIMER1_Start();
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt() {
    IFS0bits.T1IF = false;                                                      
    timer1_obj.count++;
    
    if (timer1_obj.count > 0xFFFE) {
        timer1_obj.count = 0;
    }
}


void TIMER1_Start( void ) {
    IFS0bits.T1IF = false;                                                      // reset timer1 interrupt flag
    IEC0bits.T1IE = true;
    T1CONbits.TON = true;
}


void TIMER1_Stop( void ) {
    T1CONbits.TON = false;
    IEC0bits.T1IE = false;
}


void TIMER1_CountReset( void ) {
    timer1_obj.count = 0;                                                       // reset counter parameters
    timer1_obj.count_by_1k = 0;                                                 // reset counter parameters
}


void delay_ms(uint16_t duration){
    TIMER1_CountReset();
    while (timer1_obj.count_by_1k < duration) {
        if (timer1_obj.count > COUNT_LIMIT) {
            timer1_obj.count = 0;
            timer1_obj.count_by_1k++;
        }
    }
    TIMER1_CountReset();
}


void delay_5us(uint8_t duration){
    TIMER1_CountReset();
    while (timer1_obj.count < duration) {
        // do nothing
    }
    TIMER1_CountReset();
}




// *****************************************************************************
// SCCP1 Routines
//
// - used by milliseconds()
// - used by seconds()
// *****************************************************************************

typedef struct _SCCP1_TMR_OBJ_STRUCT {
    volatile uint16_t                   count_buf;
    volatile unsigned long long int     count;
    volatile unsigned long long int     count_by_1k;
} SCCP1_TMR_OBJ;
static SCCP1_TMR_OBJ sccp1_obj;


void SCCP1_Initialize(void) {
    
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
    CCP1PRL = 0xC350;                                                           //PR 50000;
    CCP1PRH = 0x00;                                                             //PR 0;
    CCP1RA = 0x00;                                                              //CMP 0;
    CCP1RB = 0x00;                                                              //CMP 0;
    CCP1BUFL = 0x00;                                                            //BUF 0;
    CCP1BUFH = 0x00;                                                            //BUF 0; 

    IFS0bits.CCP1IF = false;
    IFS0bits.CCT1IF = false;
      
    IEC0bits.CCP1IE = true;                                                     // Enabling SCCP1 interrupt.
    IEC0bits.CCT1IE = true;                                                     // Enabling SCCP1 interrupt.
    
    SCCP1_Start();
}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT1Interrupt ( void ) {         
    IFS0bits.CCT1IF = false;
    sccp1_obj.count++;
    sccp1_obj.count_buf++;
            
    if (sccp1_obj.count_buf > COUNT_LIMIT){
        sccp1_obj.count_by_1k++;
        sccp1_obj.count_buf = 0;
    }
}


void SCCP1_Start( void ) {
    sccp1_obj.count_buf = 0;                                                    // reset counter
    sccp1_obj.count = 0;                                                        // reset counter
    sccp1_obj.count_by_1k = 0;                                                  // reset counter

    CCP1CON1Lbits.CCPON = true;
}


void SCCP1_Stop( void ) {
    CCP1CON1Lbits.CCPON = false;
}


int milliseconds (void) {
    return sccp1_obj.count;
}


int seconds (void) {
    return sccp1_obj.count_by_1k; 
}