// std lib
#include <stdio.h>
#include <stdlib.h>

// hardware
#include "BOARD.h"
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"

// microchip
#include <xc.h>
#include <sys/attribs.h>

// BB stuff
#include "Agent.h"
#include "BattleBoats.h"
#include "Field.h"
#include "Message.h"

#define PARAM_A 0x03
#define PARAM_B 0x08
#define TEST_HASH 0x09
#define TEST_CHEAT 0xFF

#define COL 7
#define ROW 2

// Structs and Macros
static BB_Event event;
static Message msg;


int main(void)
{
    
    BOARD_Init();
    
    // testing set states
    printf("\n##### Beginning Dyu40 and acompala's agent test harness: ####\n\n");
    printf("\nStart and finish\n");
    printf("1. Testing AgentInit(): ");
    AgentInit();
    if(AgentGetState() == AGENT_STATE_START)
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILURE\n");
    }    
    
    printf("2. Testing AgentSetState(End): ");
    AgentSetState(AGENT_STATE_END_SCREEN);
    if (AgentGetState() == AGENT_STATE_END_SCREEN) 
    {
        printf("SUCCESS\n");
    }
    else
    {
        printf("FAILURE\n");
    }
    
    printf("\nButton Events\n");
    AgentInit();
    printf("3. Testing Start Button Event: ");
    event.type = BB_EVENT_START_BUTTON;
    msg = AgentRun(event);
    
    if (AgentGetState() == AGENT_STATE_CHALLENGING) 
    {
        printf("SUCCESS\n");
    } else 
    {
        printf("FAILURE\n");
    }
    
    printf("4. Testing if CHA sent detected: ");
    if (msg.param0 > 0 && msg.type == MESSAGE_CHA) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
        printf("Message type:%d\n", msg.type);
        printf("Param 0:%d\n", msg.param0);
    }
    
    printf("\nAGENT_STATE_CHALLENGING\n");
    AgentInit();
    printf("5. Testing Received ACC Event: ");
    event.type = BB_EVENT_ACC_RECEIVED;
    event.param0 = PARAM_B;
    msg = AgentRun(event);
    if (AgentGetState() == AGENT_STATE_DEFENDING || AgentGetState() == AGENT_STATE_WAITING_TO_SEND ) 
    {
        // acc rec: should be defending and waiting for msg to be sent
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("6. Testing REV sent:");
    if (msg.type == MESSAGE_REV && msg.param0 > 0) 
    {
        // reveal sent: param should be greater than bc param0 should have some non zero val and message
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    AgentInit();
    printf("\n Agent State Start\n");
    printf("7. Testing CHA Received\n");
    event.type = BB_EVENT_CHA_RECEIVED;
    event.param0 = TEST_HASH;
    msg = AgentRun(event);
    
    if (AgentGetState() == AGENT_STATE_ACCEPTING) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }   
    printf("8. Test if ACC sent:");
    if (msg.type == MESSAGE_ACC && msg.param0 > 0) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    } 


    printf("\nAgent State Accepting\n");
    printf("9. REV received:");
    event.type = BB_EVENT_REV_RECEIVED;
    event.param0 = PARAM_A;
    msg = AgentRun(event);
    if (AgentGetState() == AGENT_STATE_ATTACKING || AgentGetState() == AGENT_STATE_DEFENDING) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    
    printf("10. Test if Cheater Detected:"); // cheating is detected when the param is the same as the hash
    AgentSetState(AGENT_STATE_ACCEPTING);
    event.type = BB_EVENT_REV_RECEIVED;
    event.param0 = TEST_CHEAT;
    msg = AgentRun(event);
    
    // game should end if the cheat is detected -> check that the game has ended
    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    } 
    
    // waiting to send
    printf("\nAgent State Waiting to Send\n");
    AgentInit();
    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
    printf("11. Test Message sent:");
    event.type = BB_EVENT_MESSAGE_SENT;
    msg = AgentRun(event);
    
    if (AgentGetState() == AGENT_STATE_ATTACKING) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    } 
    
    printf("12. Test if SHO sent");
    if (msg.param0 >= 0 && msg.param1 >= 0 && msg.type == MESSAGE_SHO) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    printf("\nAgent State Attacking\n");
    printf("13. Test if RES received\n");
    event.type = BB_EVENT_RES_RECEIVED;
    event.param0 = ROW;
    event.param1 = COL;
    event.param2 = RESULT_MISS;
    msg = AgentRun(event);
    
    if (AgentGetState() == AGENT_STATE_DEFENDING) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    
    printf("\nTesting AGENT_STATE_DEFENDING\n");
    printf("14.Test if SHO Received: ");
    event.type = BB_EVENT_SHO_RECEIVED;
    event.param0 = ROW;
    event.param1 = COL;
    msg = AgentRun(event);
    
    if (AgentGetState() == AGENT_STATE_WAITING_TO_SEND) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    printf("15. Test if RES sent): ");
    if (msg.type == MESSAGE_RES && msg.param0 >= 0 && msg.param1 >= 0 && msg.param2 >= 0) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    printf("\nWin Condition\n");
    printf("16. Test Victory Prompt");
    //Resets all lives on ship
    AgentInit();
    event.type = BB_EVENT_START_BUTTON;
    msg = AgentRun(event);
    //Destroys each ship one by one
    AgentSetState(AGENT_STATE_ATTACKING);
    event.type = BB_EVENT_RES_RECEIVED;
    event.param0 = ROW;
    event.param1 = COL;
    event.param2 = RESULT_SMALL_BOAT_SUNK;
    msg = AgentRun(event);
    
    AgentSetState(AGENT_STATE_ATTACKING);
    event.type = BB_EVENT_RES_RECEIVED;
    event.param0 = ROW;
    event.param1 = COL;
    event.param2 = RESULT_MEDIUM_BOAT_SUNK;
    msg = AgentRun(event);
    
    AgentSetState(AGENT_STATE_ATTACKING);
    event.type = BB_EVENT_RES_RECEIVED;
    event.param0 = ROW;
    event.param1 = COL;
    event.param2 = RESULT_LARGE_BOAT_SUNK;
    msg = AgentRun(event);
    
    AgentSetState(AGENT_STATE_ATTACKING);
    event.type = BB_EVENT_RES_RECEIVED;
    event.param0 = ROW;
    event.param1 = COL;
    event.param2 = RESULT_HUGE_BOAT_SUNK;
    msg = AgentRun(event);
    
    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    BOARD_End();
    return (EXIT_SUCCESS);
}
    
    
    
    
