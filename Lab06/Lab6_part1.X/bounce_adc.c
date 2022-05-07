// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

#define WINDOW 5
#define MAX_VOLTAGE 1023.0
// User libraries

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
struct AdcResult{
    uint8_t event;
    uint16_t voltage;
};
// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
static struct AdcResult Adc;


int main(void)
{
    BOARD_Init();

// Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
     AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time
    
    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
  printf("Welcome to CRUZID's lab6 part3 (bounce_adc).  Compiled on %s %s.\n",__TIME__,__DATE__);


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    Adc.voltage = 0;
    Adc.event = FALSE;
    OledInit();
    
    while (1);
    // check event flag
        // if true
            //get percentage of the reading
            //sprintf the raw value and percentage
            // 
            // draw the oled str
            // update oled
            // reset event flag
        // else if
    if(Adc.event){
        char string[80];
        double percent = (Adc.voltage/MAX_VOLTAGE)*100.0;
        sprintf(string, "Potentiometer value:\n %d\n %.0lf%% ", Adc.voltage, percent);
        OledClear(OLED_COLOR_BLACK);
        OledDrawString(string);
        OledUpdate();
        Adc.event = FALSE;
    }
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void)
{
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;
    int adcSUM = ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7;
    int average = adcSUM/8;
    
    if(abs(average - Adc.voltage) > WINDOW){
        Adc.event = TRUE;
        Adc.voltage = average;
    }

}