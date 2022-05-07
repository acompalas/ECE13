//initialize everything to zero in the init
//asynchronously check every button as it is checked then released
#include "Buttons.h"
#include "BOARD.h"

static int previousEvent = 0;

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified, so that this library does not interfere with other libraries.
 */
void ButtonsInit(void){
    TRISD |= 0x00E0;
    TRISF |= 0x002;
}
/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t ButtonsCheckEvents(void){
    int debounceCounter = BUTTONS_DEBOUNCE_PERIOD;
    uint8_t buttonEvent = BUTTON_EVENT_NONE;
    uint8_t buttonState = BUTTON_STATES();
    
    if(buttonState != BUTTONS_EVENT_NONE){
        if(debounceCounter == 0){
            if(buttonState & BUTTON_STATE_1){
                buttonEvent |= BUTTON_EVENT_1DOWN;
            }else if(!(buttonState & BUTTON_STATE_1)&& (previousEvent & BUTTON_STATE_1)){
                buttonEvent |= BUTTON_EVENT_1UP;
            }
            if(buttonState & BUTTON_STATE_2){
                buttonEvent |= BUTTON_EVENT_2DOWN;
            }else if(!(buttonState & BUTTON_STATE_2)&& (previousEvent & BUTTON_STATE_2)){
                buttonEvent |= BUTTON_EVENT_2UP;
            }
            if(buttonState & BUTTON_STATE_3){
                buttonEvent |= BUTTON_EVENT_3DOWN;
            }else if(!(buttonState & BUTTON_STATE_3)&& (previousEvent & BUTTON_STATE_3)){
                buttonEvent |= BUTTON_EVENT_3UP;
            }
            if(buttonState & BUTTON_STATE_4){
                buttonEvent |= BUTTON_EVENT_3DOWN;
            }else if(!(buttonState & BUTTON_STATE_4)&& (previousEvent & BUTTON_STATE_4)){
                buttonEvent |= BUTTON_EVENT_4UP;
            }
    }else{
            debounceCounter -= 1;
            return BUTTON_EVENT_NONE;
    }
        return buttonEvent;
}
