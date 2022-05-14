// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CSE13E Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
#include "Adc.h"
#include "Buttons.h"
#include "Ascii.h"
#include "Leds.h"
#include "Oled.h"
#include "OledDriver.h"

//void flash(void); //function prototype

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define TOPOFF "|\x02\x02\x02\x02\x02\x02|"
#define TOPON "|\x01\x01\x01\x01\x01\x01|"
#define SPACE "|      |"
#define MID "|------|"
#define BOTOFF "|\x04\x04\x04\x04\x04\x04|"
#define BOTON "|\x03\x03\x03\x03\x03\x03|"
#define DEGREE "\xF8"
#define LONG_PRESS 5
#define DEFAULT_TIME 1
#define DEFAULT_TEMP 300
#define DEFAULT_BAKE_TEMP 350
#define BROIL_TEMP 500


// **** Set any local typedefs here ****
typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING
} OvenState;

typedef enum{
    BAKE, BROIL, TOAST
}OvenMode;

typedef enum{
    TIME_CHANGE, TEMP_CHANGE
}Change;

typedef struct {
    OvenState state;
    OvenMode mode;
    int timeleft;
    int timeinit;
    int temp;
    Change select;
    //add more members to this struct
} OvenData;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
static OvenData ovenData;
static uint16_t RUN_TIME;
static uint16_t STORED_TIME;
static uint16_t TIMER_TICK;
static uint8_t ButtonEvent;
//static uint8_t StoreButtonEvent;
static uint8_t AdcEvent;
static uint8_t Timer;
static uint8_t str[80];
// **** Put any helper functions here ****


/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData)
{
    if(ovenData.state == COOKING)
    {
        
        if(ovenData.mode == BAKE)
        {
            sprintf(str, "%s  Mode: Bake\n%s  Time: %d:%02d\n%s  Temp: %d%sF\n%s", 
                TOPON, SPACE, ovenData.timeleft/60 ,ovenData.timeleft%60, MID, ovenData.temp, DEGREE, BOTON);
        }
        else if(ovenData.mode == BROIL)
        {
            sprintf(str, "%s  Mode: Broil\n%s  Time: %d:%02d\n%s  Temp: %d%sF\n%s", 
                TOPON, SPACE, ovenData.timeleft/60 ,ovenData.timeleft%60, MID, BROIL_TEMP, DEGREE, BOTOFF);
        }
        else if(ovenData.mode == TOAST)
        {
            sprintf(str, "%s  Mode: Toast\n%s  Time: %d:%02d\n%s\n%s", 
                TOPOFF, SPACE, ovenData.timeleft/60 ,ovenData.timeleft%60, MID, BOTON);
        }
    }
    else if(ovenData.state != COOKING)
    {
        if(ovenData.mode == BAKE)
        {
            if(ovenData.select == TIME_CHANGE)
            {
                //LEDS_SET(0x04);
                sprintf(str, "%s  Mode: Bake\n%s >Time: %d:%02d\n%s  Temp: %d%sF\n%s", 
                TOPOFF, SPACE, ovenData.timeinit/60 ,ovenData.timeinit%60, MID, ovenData.temp, DEGREE, BOTOFF);
            }
            else if(ovenData.select == TEMP_CHANGE)
            {
                sprintf(str, "%s  Mode: Bake\n%s  Time: %d:%02d\n%s >Temp: %d%sF\n%s", 
                TOPOFF, SPACE, ovenData.timeinit/60 ,ovenData.timeinit%60, MID, ovenData.temp, DEGREE, BOTOFF);
            }
        } else if(ovenData.mode == BROIL)
        {
            sprintf(str, "%s  Mode: Broil\n%s  Time: %d:%02d\n%s  Temp: %d%sF\n%s", 
                TOPOFF, SPACE, ovenData.timeinit/60 , ovenData.timeinit%60, MID, BROIL_TEMP, DEGREE, BOTOFF);
        } else if(ovenData.mode == TOAST)
        {
            sprintf(str, "%s  Mode: Toast\n%s  Time: %d:%02d\n%s\n%s", 
                TOPOFF, SPACE, ovenData.timeinit/60 ,ovenData.timeinit%60, MID, BOTOFF);
        }
    }
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(str);
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    //write your SM logic here.
    //updateOvenOLED(ovenData);
    static uint16_t ELAPSED_TIME = 0;
    switch (ovenData.state)
    {
        case SETUP:
            if(ButtonEvent & BUTTON_EVENT_3DOWN) //check button events 3 and 4 down
            {
                STORED_TIME = RUN_TIME;
                //StoreButtonEvent = BUTTON_EVENT_3DOWN;
                ovenData.state = SELECTOR_CHANGE_PENDING;    
            }
            if(ButtonEvent & BUTTON_EVENT_4DOWN)
            {
                //STORED_TIME = RUN_TIME;
                updateOvenOLED(ovenData);
                LEDS_SET(0xFF); //turn on all leds
                ovenData.state = COOKING;
            }
            if(AdcEvent) //check if adc changed
            {
                if(ovenData.mode == BAKE)
                {
                    //change oven state as needed
                    if(ovenData.select == TIME_CHANGE)
                    {
                        ovenData.timeinit = ((AdcRead() >> 2) + DEFAULT_TIME);
                        updateOvenOLED(ovenData);
                    }
                    else if(ovenData.select == TEMP_CHANGE)
                    {
                        ovenData.temp = ((AdcRead() >> 2) + DEFAULT_TEMP);
                        updateOvenOLED(ovenData);
                    }
                }
                else if(ovenData.mode == BROIL)
                {
                    ovenData.select == TIME_CHANGE;
                    ovenData.timeinit = ((AdcRead() >> 2) + 1);
                    ovenData.temp = BROIL_TEMP;
                    updateOvenOLED(ovenData);
                }
                else if(ovenData.mode == TOAST)
                {
                    ovenData.select == TIME_CHANGE;
                    ovenData.timeinit = ((AdcRead() >> 2) + 1);
                    updateOvenOLED(ovenData);
                }
           }
            else
            {
                updateOvenOLED(ovenData);
            }
            break; //break   
        case SELECTOR_CHANGE_PENDING:
            if(ButtonEvent & BUTTON_EVENT_3UP)//check button event 3 up
            {
               //keep track of elapsed time
               if((RUN_TIME - STORED_TIME) < LONG_PRESS)
               {
                   if(ovenData.mode == BAKE)
                   {
                       ovenData.state = SETUP;
                       ovenData.mode = BROIL;
                       updateOvenOLED(ovenData);
                   }
                   else if(ovenData.mode == BROIL)
                   {
                       ovenData.state = SETUP;
                       ovenData.mode = TOAST;
                       updateOvenOLED(ovenData);
                   }
                   else if(ovenData.mode == TOAST)
                   {
                       ovenData.state = SETUP;
                       ovenData.mode = BAKE;
                       ovenData.temp = DEFAULT_BAKE_TEMP;
                       updateOvenOLED(ovenData);
                   }
               }
               if((RUN_TIME - STORED_TIME) > LONG_PRESS)
               {
                   if(ovenData.select == TIME_CHANGE)
                   {
                        ovenData.select = TEMP_CHANGE;
                        ovenData.state = SETUP;
                        updateOvenOLED(ovenData);
                   }
                   else if(ovenData.select == TEMP_CHANGE)
                   {
                        ovenData.select = TIME_CHANGE;
                        ovenData.state = SETUP;
                        updateOvenOLED(ovenData);
                   }
               }
            }
            break;
        case COOKING:
            if(TIMER_TICK)
            {
                ELAPSED_TIME += 1;
            }
            ovenData.timeleft = ovenData.timeinit - ELAPSED_TIME; //decrement timeleft and update oled 
            updateOvenOLED(ovenData);
            if(ovenData.timeleft > 0)
            {  
                if (ovenData.timeleft%(ovenData.timeinit/8) == 0)
                {
                    LEDS_SET(LEDS_GET()<<1); //decrement led board 
                }
            }
            else if(ovenData.timeleft == 0)//else cooking is done, set timeleft back to timeinit and set mode to setup
            {   
                //flash();
                ovenData.timeleft = ovenData.timeinit;
                TIMER_TICK = FALSE;
                ELAPSED_TIME = 0;
                LEDS_SET(0x00); //turn off all LEDS
                updateOvenOLED(ovenData); //update oled 
                ovenData.state = SETUP; //go back to SETUP
            }
            if(ButtonEvent & BUTTON_EVENT_4DOWN)//check button event 4 down
            {
                STORED_TIME = RUN_TIME; //store runtime
                ovenData.state = RESET_PENDING; //set state to reset pending
            }   
            
            break;//break
        case RESET_PENDING:
            if(ButtonEvent & BUTTON_EVENT_4UP) //if btn event 4 up set mode to cooking
            {
                if((RUN_TIME - STORED_TIME) > LONG_PRESS)//if elapsed time >= LONG_PRESS
                {
                    //end cooking, reset to default values, update OLED, turn off LEDS and set mode to SETUP
                    LEDS_SET(0x00); //turn off all LEDS
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP; //go back to SETUP
                }
                else
                {
                     // update LED bar and update OLED
                    LEDS_SET(0xFF); //turn all leds back on
                    ovenData.timeleft = ovenData.timeinit; //reset time to default
                    updateOvenOLED(ovenData);
                    TIMER_TICK = FALSE;
                    ELAPSED_TIME = 0;
                    ovenData.state = COOKING;
                }    
            }      
    } 
}

/*void flash(void)
{
    
}*/

int main()
{
    BOARD_Init();

     //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">
    
    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
   
    printf("Welcome to acompala's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);
    
    //initialize state machine (and anything else you need to init) here
    //initialize state of ovenData, ADC, Buttons, LED, and OLED
    AdcInit();
    ButtonsInit();
    LEDS_INIT();
    OledInit();
    ovenData.select = TIME_CHANGE;
    ovenData.temp = DEFAULT_BAKE_TEMP;
    ovenData.timeinit = DEFAULT_TIME;
    ovenData.state = SETUP;
    ovenData.mode = BAKE;
    RUN_TIME = 0;
    AdcEvent = FALSE;
    ButtonEvent = BUTTON_EVENT_NONE;
    TIMER_TICK = FALSE;
    
    while (1){
        // Add main loop code here: 
        // check 3 different events: button events, adc events, and timer events
        if(AdcEvent)
        {
            runOvenSM();
            AdcEvent = FALSE;
        }
        if(ButtonEvent)
        {
            runOvenSM();
            ButtonEvent = BUTTON_EVENT_NONE;
        }
        if(TIMER_TICK)
        {
            runOvenSM();
            TIMER_TICK = FALSE;
        }
        
        
        // on event, run runOvenSM() for each event
        // clear event flags
    };
}


/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    // increase timer by 1
    RUN_TIME += 1;
    //TIMER_TICK = TRUE;
    //set timer flag to true
    Timer += 1;
    if(Timer == 5)
    {
        Timer = 0;
        TIMER_TICK = TRUE;
    }
    //add event-checking code here
}


/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    // check if ADC got changed and save to a checker variable
    AdcEvent = AdcChanged();
    //check if there are any button event and save to a variable
    ButtonEvent = ButtonsCheckEvents();
    //add event-checking code here
}