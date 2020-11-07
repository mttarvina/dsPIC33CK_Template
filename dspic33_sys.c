/* 
 * File:            sys_config.c
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     Source code for initialization routines
 * 
 * Revision history:
 *      11/04/2020  --> Added TIMER1, SCCP1 Initializations
 *                  --> Converted GPIO and System Clock Initializations as separate functions
 *      ************************************************************************
 *      11/02/2020  --> Modified clock setting to Fosc = 100MHz
 *      ************************************************************************
 *      11/01/2020  --> Initial creation of sys_config.c
 *                  --> Created SYS_INIT function
 *                  --> Added custom bit config settings
 */

#include "dspic33_sys.h"
#include "dspic33_time.h"


// Configuration bits

// FSEC
#pragma config BWRP = OFF                                                       //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED                                                   //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
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


void SYS_INIT (void) {
    GPIO_Initialize();
    SYSCLOCK_Initialize();
    TIMER1_Initialize();
    SCCP1_Initialize();
}


void GPIO_Initialize (void) {

    TRISA = 0x001F;                     // RA0 - RA4 as INPUT
    if (DEBUG){
        TRISB = 0xFFFD;                 // RB0 - RB15 as INPUT except RB1
                                        // Display CLK at RB1
    }
    else {
        TRISB = 0xFFFF;                 // RB0 - RB15 as INPUT
    }
    LATA = 0x0000;                      // Default as 0
    LATB = 0x0000;                      // Default as 0
    CNPDA = 0x0000;                     // Disable weak pullup/pulldown
    CNPDB = 0x0000;                     // Disable weak pullup/pulldown
    CNPUA = 0x0000;                     // Disable weak pullup/pulldown
    CNPUB = 0x0000;                     // Disable weak pullup/pulldown
    ODCA = 0x0000;                      // Disable open-drain
    ODCB = 0x0000;                      // Disable open-drain
    ANSELA = 0x0000;                    // Default to Digital Input
    ANSELB = 0x0000;                    // Default to Digital Input
}


void SYSCLOCK_Initialize (void) {
   
    CLKDIV = 0x3001;                    // FRCDIV FRC/1; PLLPRE 1; DOZE 1:8; DOZEN disabled; ROI disabled;
    PLLFBD = 0x32;                      // PLLFBDIV 50;
    OSCTUN = 0x00;                      // TUN Center frequency;
    PLLDIV = 0x21;                      // POST1DIV 1:2; VCODIV FVCO/4; POST2DIV 1:1;
    ACLKCON1 = 0x8101;                  // APLLEN enabled; FRCSEL FRC; APLLPRE 1:1;
    APLLFBD1 = 0x32;                    // APLLFBDIV 50;
    APLLDIV1 = 0x21;                    // APOST1DIV 1:2; APOST2DIV 1:1; AVCODIV FVCO/4;
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
    
    // CF no clock failure; NOSC FRCPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) (0x01));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
    
    WDTCONLbits.ON = 0;                 // Disable Watchdog Timer 
}