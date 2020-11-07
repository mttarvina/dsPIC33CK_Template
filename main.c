/*  
 * File:            TestProject/main
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     Sample code for Demo and Testing
 */

#include <xc.h>
#include "dspic33_sys.h"
#include "dspic33_time.h"

#define INTERVAL1       1                                                       // 1 ms interval
#define INTERVAL2       1000                                                    // 1 second interval

unsigned long long time_stamp1 = 0;
unsigned long long time_stamp2 = 0;

void main (void) {
    
    // setup
    SYS_INIT();
    TRISB = TRISB & ~(RB2);                                                     // RB2 as digital output
    TRISB = TRISB & ~(RB8);                                                     // RB8 as digital output
    time_stamp1 = milliseconds();
    time_stamp2 = milliseconds();
    
    // loop
    while (true) {
        if ((milliseconds() - time_stamp1) >= INTERVAL1) {
            LATBbits.LATB2 = !LATBbits.LATB2;
            time_stamp1 = milliseconds();
        }
        
        if ((milliseconds() - time_stamp2) >= INTERVAL2) {
            LATBbits.LATB8 = !LATBbits.LATB8;
            time_stamp2 = milliseconds();
        }
    }
}

