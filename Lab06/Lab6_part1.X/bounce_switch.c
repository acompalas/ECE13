
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
					


//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Declare any datatypes here ****
#define LED1 0x01
#define LED8 0x80
#define LEFT 0
#define RIGHT 1

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
												 

// **** Declare function prototypes ****
static struct Timer timer;

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to CRUZID's lab6 part2 (bounce_switch).  Compiled on %s %s.\n",__TIME__,__DATE__);
    
    LEDS_INIT();
    timer.event = FALSE;
    timer.timeRemaining = 1;
    LEDS_SET(LED1);
    int dir;
							 
	while(1){
        //poll timer events and react if any occur
        if (timer.event){
            unsigned char current = LEDS_GET();
            if(current == LED8){
                dir = RIGHT;
            } 
            if(current == LED1){
                dir = LEFT;
            }
            if(dir == LEFT){
                LEDS_SET(current<<1);
            }
            if(dir == RIGHT){
                LEDS_SET(current>>1);
            }
            timer.event = FALSE;
        }
    }			


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}


/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    timer.timeRemaining -= 1;
    
    if(timer.timeRemaining <= 0){
        timer.event = TRUE;
        timer.timeRemaining = 1 + SWITCH_STATES();
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/									
	 
}