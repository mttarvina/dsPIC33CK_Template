/* 
 * File:            dspic33ck_analog.c
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     This is the source code for DAC and DAC related routines
 */


#include <xc.h>
#include "dspic33ck_analog.h"


// *****************************************************************************
// SCCP2 Routines
//
// - can used by adc as trigger at constant rate
//
// *****************************************************************************

static SCCP2_TMR_OBJ sccp2_obj;


void SCCP2_Initialize(void) {
    
    //    CCPI: CCP2 Capture/Compare Event
    //    Priority: 1
    IPC5bits.CCP2IP = 1;
    //    CCTI: CCP2 Timer Event
    //    Priority: 1
    IPC6bits.CCT2IP = 1;
        
    // CCPON disabled; MOD 16-Bit/32-Bit Timer;
    // CCSEL disabled; CCPSIDL disabled; T32 16 Bit;
    // CCPSLP disabled; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled;
        
    CCP2CON1L = (0x00 & 0x7FFF);                                                //Disabling CCPON bit
    CCP2CON1H = 0x00;                                                           //RTRGEN disabled; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; OPS Each Time Base Period Match; SYNC None; OPSSRC Timer Interrupt Event; 
    CCP2CON2L = 0x00;                                                           //ASDGM disabled; SSDG disabled; ASDG 0; PWMRSEN disabled;
    CCP2CON2H = 0x00;                                                           //ICGSM Level-Sensitive mode; ICSEL IC2; AUXOUT Disabled; OCAEN disabled; OENSYNC disabled;
    CCP2CON3H = 0x00;                                                           //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state;
    CCP2STATL = 0x00;                                                           //ICDIS disabled; SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; TRCLR disabled;
    CCP2TMRL = 0x00;                                                            //TMR 0;
    CCP2TMRH = 0x00;                                                            //TMR 0;
    
    if (CLOCK_SystemFrequencyGet() == _8MHZ) {
        CCP2PRL = 0x4F;
    }
    else if (CLOCK_SystemFrequencyGet() == _16MHZ) {
        CCP2PRL = 0x9F;
    }
    else if (CLOCK_SystemFrequencyGet() == _20MHZ) {
        CCP2PRL = 0xC7;
    }
    else if (CLOCK_SystemFrequencyGet() == _50MHZ) {
        CCP2PRL = 0x1F3;
    }
    else if (CLOCK_SystemFrequencyGet() == _100MHZ) {
        CCP2PRL = 0x3E7;
    }    
    
    
    CCP2PRH = 0x00;                                                             //PR 0;
    CCP2RA = 0x00;                                                              //CMP 0;
    CCP2RB = 0x00;                                                              //CMP 0;
    CCP2BUFL = 0x00;                                                            //BUF 0;
    CCP2BUFH = 0x00;                                                            //BUF 0
         
    SCCP2_Start();
}


void SCCP2_Start( void ) {
    sccp2_obj.state = false;

    IFS1bits.CCP2IF = false;
    IFS1bits.CCT2IF = false;
    
    IEC1bits.CCP2IE = true;                                                     // Enabling SCCP2 interrupt.
    IEC1bits.CCT2IE = true;                                                     // Enabling SCCP2 interrupt.
    
    CCP2CON1Lbits.CCPON = true;
}


void SCCP2_Stop( void ) {
    CCP2CON1Lbits.CCPON = false;
}


void SCCP2_SetSamplingPeriod( uint16_t _period_us ) {
    
    if (CLOCK_SystemFrequencyGet() == _8MHZ) {
        sccp2_obj.pr = _period_us * 4;
    }
    else if (CLOCK_SystemFrequencyGet() == _16MHZ) {
        sccp2_obj.pr = _period_us * 8;
    }
    else if (CLOCK_SystemFrequencyGet() == _20MHZ) {
        sccp2_obj.pr = _period_us * 10;
    }
    else if (CLOCK_SystemFrequencyGet() == _50MHZ) {
        sccp2_obj.pr = _period_us * 25;
    }
    else if (CLOCK_SystemFrequencyGet() == _100MHZ) {
        sccp2_obj.pr = _period_us * 50;
    }    

    SCCP2_Stop();
    CCP2PRL = sccp2_obj.pr - 1;
    SCCP2_Start();
}

bool SCCP2_Triggered( void ) {
    if(sccp2_obj.state) {
        sccp2_obj.state = false;
        return true;
    }
    else {
        return false;
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT2Interrupt ( void ) {
    sccp2_obj.state = true;
    IFS1bits.CCT2IF = false;
}



// *****************************************************************************
// ADC1 Routines
//
//
//
// *****************************************************************************

static ADC1_OBJ adc1_obj;


void ADC1_Initialize ( void ) {

    if (ADC1_USES_INTERRUPT) {
        //    ADCI: ADC Global Interrupt
        //    Priority: 2
        IPC22bits.ADCIP = 1;
        //    ADCAN0: ADC AN0 Convert Done
        //    Priority: 2
        IPC22bits.ADCAN0IP = 1;
    }

    // ADSIDL disabled; ADON enabled; 
    ADCON1L = 0x0000;                                                           // Disabling ADON bit
    ADCON1H = 0x60;                                                             // FORM Integer; SHRRES 12-bit resolution;

    ADCON2L = 0x00;                                                             // Shared ADC Core Input CLK Divider, SHRADCS = 2;
                                                                                // 1 TADCORE = 2 Fosc CLK period
                                                                                // PTGEN disabled; REFCIE disabled; SHREISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; REFERCIE disabled; EIEN disabled;
    
    ADCON2H = 0x00;                                                             // Shared ADC Core Sample Time, SHRSAMC = 2 TADCORE;
                                                                                // Shared ADC Core Sample Time = 4 Fosc CLK period

    ADCON3L = 0x00;                                                             // SWCTRG disabled; SHRSAMP disabled; SUSPEND disabled; SWLCTRG disabled; SUSPCIE disabled; CNVCHSEL AN0; REFSEL disabled; 
    
    // SHREN disabled; C1EN disabled; C0EN enabled; CLKDIV 1; CLKSEL FOSC; 
    ADCON3H = 0x4000;                                                           // Fosc = CLK Source, Disabling C0EN, C1EN, C2EN, C3EN and SHREN bits   
    ADCON4L = 0x00;                                                             // SAMC0EN disabled; SAMC1EN disabled;
    ADCON4H = 0x00;                                                             // C0CHS AN0; C1CHS AN1; 
    ADMOD0L = 0x00;                                                             // SIGN0 disabled; SIGN4 disabled; SIGN3 disabled; SIGN2 disabled; SIGN1 disabled; SIGN7 disabled; SIGN6 disabled; DIFF0 disabled; SIGN5 disabled; DIFF1 disabled; DIFF2 disabled; DIFF3 disabled; DIFF4 disabled; DIFF5 disabled; DIFF6 disabled; DIFF7 disabled;
    ADMOD0H = 0x00;                                                             // DIFF8 disabled; DIFF9 disabled; SIGN10 disabled; SIGN11 disabled; SIGN8 disabled; DIFF11 disabled; DIFF10 disabled; SIGN9 disabled; 
    ADMOD1H = 0x00;                                                             // DIFF25 disabled; SIGN25 disabled; 
    ADIEL = 0xFFFF;                                                             // Enable common and individual interrupts
    ADIEH = 0xFFFF;                                                             // Enable common and individual interrupts
    ADFL0CON = 0x000;                                                           // OVRSAM 4x; MODE Oversampling Mode; FLCHSEL AN0; IE disabled; FLEN disabled;
    ADFL1CON = 0x000;                                                           // OVRSAM 4x; MODE Oversampling Mode; FLCHSEL AN0; IE disabled; FLEN disabled; 
    ADFL2CON = 0x000;                                                           // OVRSAM 4x; MODE Oversampling Mode; FLCHSEL AN0; IE disabled; FLEN disabled; 
    ADFL3CON = 0x000;                                                           // OVRSAM 4x; MODE Oversampling Mode; FLCHSEL AN0; IE disabled; FLEN disabled; 
    ADLVLTRGL = 0x00;                                                           // LVLEN9 disabled; LVLEN8 disabled; LVLEN11 disabled; LVLEN7 disabled; LVLEN10 disabled; LVLEN6 disabled; LVLEN5 disabled; LVLEN4 disabled; LVLEN3 disabled; LVLEN2 disabled; LVLEN1 disabled; LVLEN0 disabled; 
    ADLVLTRGH = 0x00;                                                           // LVLEN24 disabled; LVLEN25 disabled; 
    ADCORE0L = 0x00;                                                            // SAMC 0; 
    ADCORE1L = 0x00;                                                            // SAMC 0; 
    ADCORE0H = 0x300;                                                           // RES 12-bit resolution; EISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; ADCS 2;
    ADCORE1H = 0x300;                                                           // RES 12-bit resolution; EISEL Early interrupt is generated 1 TADCORE clock prior to data being ready; ADCS 2; 
    ADEIEL = 0x00;                                                              // EIEN9 disabled; EIEN7 disabled; EIEN8 disabled; EIEN5 disabled; EIEN6 disabled; EIEN3 disabled; EIEN4 disabled; EIEN1 disabled; EIEN2 disabled; EIEN0 disabled; EIEN11 disabled; EIEN10 disabled;
    ADEIEH = 0x00;                                                              // EIEN25 disabled; EIEN24 disabled; 
    // C0CIE disabled; C1CIE disabled; SHRCIE disabled; WARMTIME 32768 Source Clock Periods; 
    ADCON5H = (0xF00 & 0xF0FF);                                                 //Disabling WARMTIME bit

    ADCON5Hbits.WARMTIME = 0xF;                                                 // Setting WARMTIME bit
    
    if (ADC1_IS_SOFTWARE_TRIGGERED) {
        //TRGSRC0 Common Software Trigger; TRGSRC1 Common Software Trigger; 
        ADTRIG0L = 0x101;
        //TRGSRC3 Common Software Trigger; TRGSRC2 Common Software Trigger; 
        ADTRIG0H = 0x101;
        //TRGSRC4 Common Software Trigger; TRGSRC5 Common Software Trigger; 
        ADTRIG1L = 0x101;
        //TRGSRC6 Common Software Trigger; TRGSRC7 Common Software Trigger; 
        ADTRIG1H = 0x0101;
        //TRGSRC8 Common Software Trigger; TRGSRC9 Common Software Trigger; 
        ADTRIG2L = 0x101;
        //TRGSRC11 Common Software Trigger; TRGSRC10 Common Software Trigger; 
        ADTRIG2H = 0x101;
        //TRGSRC24 None; TRGSRC25 None; 
        ADTRIG6L = 0x00;
    }

    adc1_obj.ADC1Initialized = true;
}


void ADC1_Core0PowerEnable( void ) {
    ADCON5Lbits.C0PWR = true; 
    while(!ADCON5Lbits.C0RDY);
    ADCON3Hbits.C0EN = true;   
}


void ADC1_Core0PowerDisable( void ) {
    ADCON5Lbits.C0PWR = false;
    ADCON3Hbits.C0EN = false;   
}


void ADC1_Core1PowerEnable( void )  {
    ADCON5Lbits.C1PWR = true; 
    while(!ADCON5Lbits.C1RDY);
    ADCON3Hbits.C1EN = true;     
}


void ADC1_Core1PowerDisable( void )  {
    ADCON5Lbits.C1PWR = false;
    ADCON3Hbits.C1EN = false;     
}


void ADC1_SharedCorePowerEnable( void ) {
    ADCON5Lbits.SHRPWR = true;   
    while(!ADCON5Lbits.SHRRDY);
    ADCON3Hbits.SHREN = true;
}


void ADC1_SharedCorePowerDisable( void ) {
    ADCON5Lbits.SHRPWR = false;
    ADCON3Hbits.SHREN = false;
}


uint16_t ADC1_GetConversionResult( uint8_t _channel ) {
    switch(_channel) {
        case AN0:
            return ADCBUF0;
        case AN1:
            return ADCBUF1;
        case AN2:
            return ADCBUF2;
        case AN3:
            if (!DAC1CONLbits.DACOEN) {
                return ADCBUF3;
            }
            break;
        case AN4:
            return ADCBUF4;
        case AN5:
            return ADCBUF5;
        case AN6:
            return ADCBUF6;
        case AN7:
            return ADCBUF7;
        case AN8:
            return ADCBUF8;
        case AN9:
            return ADCBUF9;
        case AN10:
            return ADCBUF10;
        case AN11:
            return ADCBUF11;
        default:
                break;
    }
    return 0;
}


bool ADC1_IsConversionComplete( uint8_t _channel) {
    switch(_channel) {
        case AN0:
            return ADSTATLbits.AN0RDY;
        case AN1:
            return ADSTATLbits.AN1RDY;
        case AN2:
            return ADSTATLbits.AN2RDY;
        case AN3:
            if (!DAC1CONLbits.DACOEN) {
                return ADSTATLbits.AN3RDY;
            }
            break;
        case AN4:
            return ADSTATLbits.AN4RDY;
        case AN5:
            return ADSTATLbits.AN5RDY;
        case AN6:
            return ADSTATLbits.AN6RDY;
        case AN7:
            return ADSTATLbits.AN7RDY;
        case AN8:
            return ADSTATLbits.AN8RDY;
        case AN9:
            return ADSTATLbits.AN9RDY;
        case AN10:
            return ADSTATLbits.AN10RDY;
        case AN11:
            return ADSTATLbits.AN11RDY;
        default:
            break;
    }
    return false;
}


void ADC1_SetChannelInterrupt( uint8_t _channel, bool _state) {
    switch(_channel) {
        case AN0:
            IFS5bits.ADCAN0IF = false;
            IEC5bits.ADCAN0IE = _state;
            break;
        case AN1:
            IFS5bits.ADCAN1IF = false;
            IEC5bits.ADCAN1IE = _state;
            break;         
        case AN2:
            IFS5bits.ADCAN2IF = false;
            IEC5bits.ADCAN2IE = _state;
            break;
        case AN3:
            if (!DAC1CONLbits.DACOEN) {
                IFS5bits.ADCAN3IF = false;
                IEC5bits.ADCAN3IE = _state;
            }
            break;
        case AN4:
            IFS5bits.ADCAN4IF = false;
            IEC5bits.ADCAN4IE = _state;
            break;
        case AN5:
            IFS6bits.ADCAN5IF = false;
            IEC6bits.ADCAN5IE = _state;
            break;
        case AN6:
            IFS6bits.ADCAN6IF = false;
            IEC6bits.ADCAN6IE = _state;
            break;
        case AN7:
            IFS6bits.ADCAN7IF = false;
            IEC6bits.ADCAN7IE = _state;
            break;
        case AN8:
            IFS6bits.ADCAN8IF = false;
            IEC6bits.ADCAN8IE = _state;
            break;
        case AN9:
            IFS6bits.ADCAN9IF = false;
            IEC6bits.ADCAN9IE = _state;
            break;
        case AN10:
            IFS6bits.ADCAN10IF = false;
            IEC6bits.ADCAN10IE = _state;
            break;
        case AN11:
            IFS6bits.ADCAN11IF = false;
            IEC6bits.ADCAN11IE = _state;
            break;
        default:
                break;
    }
}


void Analog_SetPin( uint8_t _channel ) {
    switch (_channel) {

        case AN0:
            _TRISA0 = true;                                                     // set this pin as input
            _ANSELA0 = true;                                                    // set this pin as input            
            ADC1_Core0PowerDisable();                                           // turn off core0 power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_Core0PowerEnable();                                            // power up Core 0
            break;      
        case AN1:
            _TRISB2 = true;                                                     // set this pin as input
            _ANSELB2 = true;                                                    // set this pin as input
            ADC1_Core1PowerDisable();                                           // turn off core1 power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_Core1PowerEnable();                                            // power up Core 1
            break;
        case AN2:
            _TRISB7 = true;                                                     // set this pin as input
            _ANSELB7 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN3:
            if (!DAC1CONLbits.DACOEN) {
                _TRISA3 = true;                                                 // set this pin as input
                _ANSELA3 = true;                                                // set this pin as input
                ADC1_SharedCorePowerDisable();                                  // turn off shared core power
                ADC1_Disable();                                                 // disable ADC1 first
                if (!adc1_obj.ADC1Initialized)
                    ADC1_Initialize();                                          // initialize ADC1 if not yet initialized
                // ***
                // insert channel specific config here
                    ADCON3Lbits.SHRSAMP = true;
                // ***
                ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);        // set the interrupt behavior of this channel
                ADC1_Enable();
                ADC1_SharedCorePowerEnable();                                   // power up shared core
            }
            break;
        case AN4:
            _TRISA4 = true;                                                     // set this pin as input
            _ANSELA4 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN5:
            _TRISB0 = true;                                                     // set this pin as input
            _ANSELB0 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN6:
            _TRISB1 = true;                                                     // set this pin as input
            _ANSELB1 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN7:
            _TRISB2 = true;                                                     // set this pin as input
            _ANSELB2 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN8:
            _TRISB3 = true;                                                     // set this pin as input
            _ANSELB3 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN9:
            _TRISA2 = true;                                                     // set this pin as input
            _ANSELA2 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN10:
            _TRISB8 = true;                                                     // set this pin as input
            _ANSELB8 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        case AN11:
            _TRISB9 = true;                                                     // set this pin as input
            _ANSELB9 = true;                                                    // set this pin as input
            ADC1_SharedCorePowerDisable();                                      // turn off shared core power
            ADC1_Disable();                                                     // disable ADC1 first
            if (!adc1_obj.ADC1Initialized)
                ADC1_Initialize();                                              // initialize ADC1 if not yet initialized
            // ***
            // insert channel specific config here
                ADCON3Lbits.SHRSAMP = true;
            // ***
            ADC1_SetChannelInterrupt(_channel, ADC1_USES_INTERRUPT);            // set the interrupt behavior of this channel
            ADC1_Enable();
            ADC1_SharedCorePowerEnable();                                       // power up shared core
            break;
        default:
            break;
    }
}


uint16_t ADC1_Read( uint8_t _channel ) {
    // setup channel in shared core first before software triggering

    switch (_channel) {
        case AN0:
            ADCON3Lbits.SWCTRG = 1;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN0RDY);
            return ADCBUF0;
        case AN1:
            ADCON3Lbits.SWCTRG = 1;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN1RDY);
            return ADCBUF1;
        case AN2:
            ADCON3Lbits.CNVCHSEL = 0x2;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN2RDY);
            return ADCBUF2;
        case AN3:
            if (!DAC1CONLbits.DACOEN) {
                ADCON3Lbits.CNVCHSEL = 0x3;
                ADCON3Lbits.CNVRTCH = true;
                for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
                while(!ADSTATLbits.AN3RDY);
                return ADCBUF3;
            }
        case AN4:
            ADCON3Lbits.CNVCHSEL = 0x4;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN4RDY);
            return ADCBUF4;
        case AN5:
            ADCON3Lbits.CNVCHSEL = 0x5;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN5RDY);
            return ADCBUF5;
        case AN6:
            ADCON3Lbits.CNVCHSEL = 0x6;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN6RDY);
            return ADCBUF6;
        case AN7:
            ADCON3Lbits.CNVCHSEL = 0x7;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN7RDY);
            return ADCBUF7;
        case AN8:
            ADCON3Lbits.CNVCHSEL = 0x8;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN8RDY);
            return ADCBUF8;
        case AN9:
            ADCON3Lbits.CNVCHSEL = 0x9;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN9RDY);
            return ADCBUF9;
        case AN10:
            ADCON3Lbits.CNVCHSEL = 0xA;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN10RDY);
            return ADCBUF10;
        case AN11:
            ADCON3Lbits.CNVCHSEL = 0xB;
            ADCON3Lbits.CNVRTCH = true;
            for(adc1_obj.counter = 0; adc1_obj.counter < 10; adc1_obj.counter++);
            while(!ADSTATLbits.AN11RDY);
            return ADCBUF11;
    }

    return 0;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN0Interrupt ( void ) {
    //clear the channel_AN0 interrupt flag
    IFS5bits.ADCAN0IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN1Interrupt ( void ) {
    //clear the channel_AN1 interrupt flag
    IFS5bits.ADCAN1IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN2Interrupt ( void ) {
    //clear the channel_AN2 interrupt flag
    IFS5bits.ADCAN2IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN3Interrupt ( void ) {
    //clear the channel_AN3 interrupt flag
    IFS5bits.ADCAN3IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN4Interrupt ( void ) {
    //clear the channel_AN4 interrupt flag
    IFS5bits.ADCAN4IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN5Interrupt ( void ) {
    //clear the channel_AN5 interrupt flag
    IFS6bits.ADCAN5IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN6Interrupt ( void ) {
    //clear the channel_AN6 interrupt flag
    IFS6bits.ADCAN6IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN7Interrupt ( void ) {
    //clear the channel_AN6 interrupt flag
    IFS6bits.ADCAN7IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN8Interrupt ( void ) {
    //clear the channel_AN8 interrupt flag
    IFS6bits.ADCAN8IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN9Interrupt ( void ) {
    //clear the channel_AN9 interrupt flag
    IFS6bits.ADCAN9IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN10Interrupt ( void ) {
    //clear the channel_AN10 interrupt flag
    IFS6bits.ADCAN10IF = false;
}


void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCAN11Interrupt ( void ) {
    //clear the channel_AN11 interrupt flag
    IFS6bits.ADCAN11IF = false;
}


// *****************************************************************************
// CMP1/DAC Routines
//
//
//
// *****************************************************************************


void CMP1_Initialize( void ) {           

    // Disable the CMP module before the initialization
    CMP1_Disable();
	
	// Comparator Register settings
	DACCTRL1L = 0x40;                                                           //CLKDIV 1:1; DACON disabled; DACSIDL disabled; FCLKDIV 1:1; CLKSEL FVCO/2 - System VCO Clock; 
	DACCTRL2L = 0x55;                                                           //TMODTIME 85; 
	DACCTRL2H = 0x8A;                                                           //SSTIME 138; 
	DAC1CONH = 0x00;                                                            //TMCB 0; 
	DAC1CONL = 0x8200;                                                          //CMPPOL Non Inverted; HYSPOL Rising Edge; HYSSEL None; DACEN enabled; FLTREN disabled; CBE disabled; IRQM Interrupts are disabled; INSEL CMP1A; DACOEN enabled; 

	//Slope Settings
	SLP1CONH = 0x00;                                                            //HME disabled; PSE Negative; SLOPEN disabled; TWME disabled; 
	SLP1CONL = 0x00;                                                            //HCFSEL None; SLPSTRT None; SLPSTOPB None; SLPSTOPA None; 
	SLP1DAT = 0x00;                                                             //SLPDAT 0; 
	DAC1DATL = 0x00;                                                            //DACDATL 0; 
	DAC1DATH = 0x00;                                                            //DACDATH 0; 
    
    CMP1_Enable();
}


void CMP1_Enable( void ) {
    DACCTRL1Lbits.DACON = 1;
}


void CMP1_Disable( void ) {
    DACCTRL1Lbits.DACON = 0;
}


void DAC_SetValue( uint16_t _dacVal ) {
    DAC1DATH = _dacVal;
}


void CMP1_EnableDACOutput( void ) {
    DAC1CONLbits.DACOEN = 1;
}


void CMP1_DisableDACOutput( void ) {
    DAC1CONLbits.DACOEN = 0;
}


void DAC_Enable( void ) {
    _TRISA3 = false;                                                            // set RA3 as output
    CMP1_Initialize();
    CMP1_EnableDACOutput();
}