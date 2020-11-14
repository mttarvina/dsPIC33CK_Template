/* 
 * File:            dspic33ck_core.c
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     Source code for device initialization routines
 */


#include <xc.h>
#include "dspic33ck_core.h"


// Configuration bits

// FSEC
#pragma config BWRP = OFF               //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED           //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF               //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF               //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED           //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF               //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED           //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF            //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191             //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = FRC              //Oscillator Source Selection->FRC
#pragma config IESO = OFF               //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE            //Primary Oscillator Mode Select bits->Primary Oscillator disabled

#if DEBUG
#pragma config OSCIOFNC = OFF           //OSC2 Pin Function bit->OSC2 is CLKO output
#else
#pragma config OSCIOFNC = ON            //OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#endif


#pragma config FCKSM = CSECME           //Clock Switching Mode bits->Both Clock switching and Fail-safe Clock Monitor are enabled
#pragma config PLLKEN = ON              //PLL Lock Status Control->PLL lock signal will be used to disable PLL clock output if lock is lost
#pragma config XTCFG = G3               //XT Config->24-32 MHz crystals
#pragma config XTBST = ENABLE           //XT Boost->Boost the kick-start

// FWDT
#pragma config RWDTPS = PS1             //Run Mode Watchdog Timer Post Scaler select bits->1:1
#pragma config RCLKSEL = LPRC           //Watchdog Timer Clock Select bits->Always use LPRC
#pragma config WINDIS = OFF             //Watchdog Timer Window Enable bit->Watchdog Timer in Window mode
#pragma config WDTWIN = WIN25           //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
#pragma config SWDTPS = PS1             //Sleep Mode Watchdog Timer Post Scaler select bits->1:1
#pragma config FWDTEN = ON_SW           //Watchdog Timer Enable bit->WDT controlled via SW, use WDTCON.ON bit

// FPOR
#pragma config BISTDIS = DISABLED       //Memory BIST Feature Disable->mBIST on reset feature disabled

// FICD
#pragma config ICS = PGD2               //ICD Communication Channel Select bits->Communicate on PGC2 and PGD2
#pragma config JTAGEN = OFF             //JTAG Enable bit->JTAG is disabled
#pragma config NOBTSWP = DISABLED       //BOOTSWP instruction disable bit->BOOTSWP instruction is disabled

// FDMTIVTL
#pragma config DMTIVTL = 0              //Dead Man Timer Interval low word->0

// FDMTIVTH
#pragma config DMTIVTH = 0              //Dead Man Timer Interval high word->0

// FDMTCNTL
#pragma config DMTCNTL = 0              //Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF)->0

// FDMTCNTH
#pragma config DMTCNTH = 0              //Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF)->0

// FDMT
#pragma config DMTDIS = OFF             //Dead Man Timer Disable bit->Dead Man Timer is Disabled and can be enabled by software

// FDEVOPT
#pragma config ALTI2C1 = OFF            //Alternate I2C1 Pin bit->I2C1 mapped to SDA1/SCL1 pins
#pragma config ALTI2C2 = OFF            //Alternate I2C2 Pin bit->I2C2 mapped to SDA2/SCL2 pins
#pragma config ALTI2C3 = OFF            //Alternate I2C3 Pin bit->I2C3 mapped to SDA3/SCL3 pins
#pragma config SMBEN = SMBUS            //SM Bus Enable->SMBus input threshold is enabled
#pragma config SPI2PIN = PPS            //SPI2 Pin Select bit->SPI2 uses I/O remap (PPS) pins

// FALTREG
#pragma config CTXT1 = OFF              //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits->Not Assigned
#pragma config CTXT2 = OFF              //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits->Not Assigned
#pragma config CTXT3 = OFF              //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits->Not Assigned
#pragma config CTXT4 = OFF              //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits->Not Assigned

// FBTSEQ
#pragma config BSEQ = 4095              //Relative value defining which partition will be active after device Reset; the partition containing a lower boot number will be active->4095
#pragma config IBSEQ = 4095             //The one's complement of BSEQ; must be calculated by the user and written during device programming.->4095

// FBOOT
#pragma config BTMODE = SINGLE          //Device Boot Mode Configuration->Device is in Single Boot (legacy) mode


void SYS_INIT( void ) {
    
    GPIO_Initialize();
    SYSCLOCK_Initialize();
    
    if (TIMER1_EN) {
        TIMER1_Initialize();
    }
    if (SCCP1_EN) {
        SCCP1_Initialize();
    }
    if (SCCP2_EN){
        SCCP2_Initialize();
    }
}


void SYSCLOCK_Initialize( void ) {

    CLKDIV = 0x3001;                    // FRCDIV FRC/1; PLLPRE 1; DOZE 1:8; DOZEN disabled; ROI disabled;
    PLLFBD = 0x64;                      // PLLFBDIV 100;
    OSCTUN = 0x00;                      // TUN Center frequency;
    ACLKCON1 = 0x101;                   // APLLEN disabled; FRCSEL FRC; APLLPRE 1:1; 
    APLLFBD1 = 0x64;                    // APLLFBDIV 100;

    // Fosc = 8MHz
    if (CLOCK_SystemFrequencyGet() == 8000000UL) {
        // This does not affect on 8 MHz though since its not using the PLL
        PLLDIV = 0x241;                     // POST1DIV 1:4; VCODIV FVCO/2; POST2DIV 1:1; 
        APLLDIV1 = 0x241;                   // APOST1DIV 1:4; APOST2DIV 1:1; AVCODIV FVCO/2;        
    }

    // Fosc = 16MHz
    else if (CLOCK_SystemFrequencyGet() == 16000000UL) {
        PLLDIV = 0x255;                     // POST1DIV 1:5; VCODIV FVCO/2; POST2DIV 1:5;
        APLLDIV1 = 0x255;                   // APOST1DIV 1:5; APOST2DIV 1:5; AVCODIV FVCO/2; 
    }

    // Fosc = 20MHz
    else if (CLOCK_SystemFrequencyGet() == 20000000UL) {
        PLLDIV = 0x254;                     // POST1DIV 1:5; VCODIV FVCO/2; POST2DIV 1:4;
        APLLDIV1 = 0x254;                   // APOST1DIV 1:5; APOST2DIV 1:4; AVCODIV FVCO/2; 
    }

    // Fosc = 50MHz
    else if (CLOCK_SystemFrequencyGet() == 50000000UL) {
        PLLDIV = 0x242;                     // POST1DIV 1:4; VCODIV FVCO/2; POST2DIV 1:2;
        APLLDIV1 = 0x242;                   // APOST1DIV 1:2; APOST2DIV 1:4; AVCODIV FVCO/2; 
    }

    // Fosc = 100MHz
    else if (CLOCK_SystemFrequencyGet() == 100000000UL) {
        PLLDIV = 0x241;                     // POST1DIV 1:4; VCODIV FVCO/2; POST2DIV 1:1; 
        APLLDIV1 = 0x241;                   // APOST1DIV 1:4; APOST2DIV 1:1; AVCODIV FVCO/2;
    }

    REFOCONL = 0x00;                    // ROEN disabled; ROSWEN disabled; ROSLP disabled; ROSEL FOSC; ROOUT disabled; ROSIDL disabled;
    REFOCONH = 0x00;                    // RODIV 0;
    REFOTRIMH = 0x00;                   // ROTRIM 0;
    RPCON = 0x00;                       // IOLOCK disabled;
    PMDCON = 0x00;                      // PMDLOCK disabled;
    PMD1 = 0x00;                        // ADC1MD enabled; T1MD enabled; U2MD enabled; U1MD enabled; SPI2MD enabled; SPI1MD enabled; QEI1MD enabled; PWMMD enabled; I2C1MD enabled;
    PMD2 = 0x00;                        // CCP2MD enabled; CCP1MD enabled; CCP4MD enabled; CCP3MD enabled; CCP7MD enabled; CCP8MD enabled; CCP5MD enabled; CCP6MD enabled; CCP9MD enabled;
    PMD3 = 0x00;                        // I2C3MD enabled; U3MD enabled; QEI2MD enabled; CRCMD enabled; I2C2MD enabled;
    PMD4 = 0x00;                        // REFOMD enabled;
    PMD6 = 0x00;                        // DMA1MD enabled; SPI3MD enabled; DMA2MD enabled; DMA3MD enabled; DMA0MD enabled;
    PMD7 = 0x00;                        // CMP3MD enabled; PTGMD enabled; CMP1MD enabled; CMP2MD enabled; 
    PMD8 = 0x00;                        // DMTMD enabled; CLC3MD enabled; OPAMPMD enabled; BIASMD enabled; CLC4MD enabled; SENT2MD enabled; SENT1MD enabled; CLC1MD enabled; CLC2MD enabled;

    if (CLOCK_SystemFrequencyGet() == 8000000UL) {
        // CF no clock failure; NOSC FRC; CLKLOCK unlocked; OSWEN Switch is Complete; 
        __builtin_write_OSCCONH((uint8_t) (0x00));
        __builtin_write_OSCCONL((uint8_t) (0x00));        
    }

    else {
        // CF no clock failure; NOSC FRCPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
        __builtin_write_OSCCONH((uint8_t) (0x01));
        __builtin_write_OSCCONL((uint8_t) (0x01));
        
        // Wait for Clock switch to occur
        while (OSCCONbits.OSWEN != 0);
        while (OSCCONbits.LOCK != 1);
    }
    
    WDTCONLbits.ON = 0;                 // Disable Watchdog Timer 
}


void GPIO_Initialize( void ) {

    TRISA = 0x001F;                     // initialize all as INPUT
    TRISB = 0xFFFF;                     // initialize all as INPUT
    LATA = 0x0000;                      // drive all pins to LOW
    LATB = 0x0000;                      // drive all pins to LOW
    CNPDA = 0x0000;                     // Disable weak pullup/pulldown
    CNPDB = 0x0000;                     // Disable weak pullup/pulldown
    CNPUA = 0x0000;                     // Disable weak pullup/pulldown
    CNPUB = 0x0000;                     // Disable weak pullup/pulldown
    ODCA = 0x0000;                      // Disable open-drain
    ODCB = 0x0000;                      // Disable open-drain
    ANSELA = 0x001F;                    // Default to Analog Input
    ANSELB = 0x038F;                    // Default to Analog Input
}


void Digital_SetPin( uint8_t _pin, bool _dir ) {
    switch (_pin) {
        case PA0:
            _TRISA0 = _dir;
            _ANSELA0 = _dir;
            break;
        case PA1:
            _TRISA1 = _dir;
            _ANSELA1 = _dir;
            break;
        case PA2:
            _TRISA2 = _dir;
            _ANSELA2 = _dir;
            break;
        case PA3:
            _TRISA3 = _dir;
            _ANSELA3 = _dir;
            break;
        case PA4:
            _TRISA4 = _dir;
            _ANSELA4 = _dir;
            break;
        case PB0:
            _TRISB0 = _dir;
            _ANSELB0 = _dir;
            break;
        case PB1:
            _TRISB1 = _dir;
            _ANSELB1 = _dir;
            break;
        case PB2:
            _TRISB2 = _dir;
            _ANSELB2 = _dir;
            break;
        case PB3:
            _TRISB3 = _dir;
            _ANSELB3 = _dir;
            break;
        case PB4:
            _TRISB4 = _dir;
            _ANSELB4 = _dir;
            break;
        case PB5:
            _TRISB5 = _dir;
            //_ANSELB5 = _dir;
            break;
        case PB6:
            _TRISB6 = _dir;
            //_ANSELB6 = _dir;
            break;
        case PB7:
            _TRISB7 = _dir;
            _ANSELB7 = _dir;
            break;
        case PB8:
            _TRISB8 = _dir;
            _ANSELB8 = _dir;
            break;
        case PB9:
            _TRISB9 = _dir;
            _ANSELB9 = _dir;
            break;
        case PB10:
            _TRISB10 = _dir;
            //_ANSELB10 = _dir;
            break;
        case PB11:
            _TRISB11 = _dir;
            //_ANSELB11 = _dir;
            break;
        case PB12:
            _TRISB12 = _dir;
            //_ANSELB12 = _dir;
            break;
        case PB13:
            _TRISB13 = _dir;
            //_ANSELB13 = _dir;
            break;
        case PB14:
            _TRISB14 = _dir;
            //_ANSELB14 = _dir;
            break;
        case PB15:
            _TRISB15 = _dir;
            //_ANSELB15 = _dir;
            break;
    }
}


void Digital_DrvPin( uint8_t _pin, bool _state ) {
    switch (_pin) {
        case PA0:
            _LATA0 = _state;
            break;
        case PA1:
            _LATA1 = _state;
            break;
        case PA2:
            _LATA2 = _state;
            break;
        case PA3:
            _LATA3 = _state;
            break;
        case PA4:
            _LATA4 = _state;
            break;
        case PB0:
            _LATB0 = _state;
            break;
        case PB1:
            _LATB1 = _state;
            break;
        case PB2:
            _LATB2 = _state;
            break;
        case PB3:
            _LATB3 = _state;
            break;
        case PB4:
            _LATB4 = _state;
            break;
        case PB5:
            _LATB5 = _state;
            break;
        case PB6:
            _LATB6 = _state;
            break;
        case PB7:
            _LATB7 = _state;
            break;
        case PB8:
            _LATB8 = _state;
            break;
        case PB9:
            _LATB9 = _state;
            break;
        case PB10:
            _LATB10 = _state;
            break;
        case PB11:
            _LATB11 = _state;
            break;
        case PB12:
            _LATB12 = _state;
            break;
        case PB13:
            _LATB13 = _state;
            break;
        case PB14:
            _LATB14 = _state;
            break;
        case PB15:
            _LATB15 = _state;
            break;
    }
}


bool Digital_ReadPin( uint8_t _pin ) {
    
    switch (_pin) {
        case PA0:
            return _RA0;
        case PA1:
            return _RA1;
        case PA2:
            return _RA2;
        case PA3:
            return _RA3;
        case PA4:
            return _RA4;
        case PB0:
            return _RB0;
        case PB1:
            return _RB1;
        case PB2:
            return _RB2;
        case PB3:
            return _RB3;
        case PB4:
            return _RB4;
        case PB5:
            return _RB5;
        case PB6:
            return _RB6;
        case PB7:
            return _RB7;
        case PB8:
            return _RB8;
        case PB9:
            return _RB9;
        case PB10:
            return _RB10;
        case PB11:
            return _RB11;
        case PB12:
            return _RB12;
        case PB13:
            return _RB13;
        case PB14:
            return _RB14;
        case PB15:
            return _RB15;
    }
    return false;
}