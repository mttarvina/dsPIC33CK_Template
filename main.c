/*  
 * File:            main.c
 * Device:          dsPIC33CK256MP202
 * Author:          Tarvs' Hobbytronics (mttarvina), email: mttarvina@gmail.com
 * Description:     Sample main code for Demo and Testing
 * 
 * 
 * Function:        <insert function here>
 * 
 * Hardware connection:
 *  <insert hardware connections here>
 */


#include <xc.h>
#include "dspic33ck_lib_mttarvina/dspic33ck_core.h"

uint16_t data;
unsigned long long time_stamp = 0;

int main (void) {

    // setup
    SYS_INIT();                                                                 // initialize clock and default pin configurations   
    
    DAC_Enable();
    DAC_SetValue(0);                                                            // initialize DAC output to 0V
    
    Analog_SetPin(AN11);                                                        // use RB9 or AN11 as ADC input
    
    Digital_SetPin(PB6, OUTPUT);
    //PWMA_Attach(PB6, 1000);                                                     // initialize PA4 as PWMA output with 1ms period
    //PWMA_SetDuty(0);                                                            // initialize duty cycle to 0
    //PWMA_Start();
    
    //SCCP2_SetSamplingPeriod(20);
    //time_stamp = milliseconds();
    
    // loop
    while (true) {        
        if (SCCP2_Triggered()){
        //if ((milliseconds() - time_stamp) >= 1) {                               // read ADC and forward it to DAC as output every 10ms
            _LATB6 = !_LATB6;
            //time_stamp = milliseconds();
            
            //data = ADC1_Read(AN11);
            //DAC_SetValue(data);
            DAC_SetValue(ADC1_Read(AN11));
            
            //PWMA_SetDuty(data);
        }
    }
}