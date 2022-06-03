// Include Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "BOARD.h"
#include "Negotiation.h"
#include "Message.h"
#include "Field.h"
#include "Agent.h"
#include "BattleBoats.h"
#include "Oled.h"
#include "FieldOled.h"

// define message payload
#define CHALLENGE "CHA"
#define ACCEPT "ACC"
#define REVEAL "REV"
#define SHOT "SHO"
#define RESULT "RES"

//Macros
#define RANGE_16 0xFF
#define SUNK 0x00
#define MAX_STRING_LENGTH 100
#define WIN_TXT "Victory"
#define LOSE_TXT "Defeat"
#define CHEAT_TXT "CHEATER DECTECTED"
#define ERROR_TXT "ERROR DETECTED"

// str agent info in struct

typedef struct {
    AgentState currentState;
    int turnCounter;
    NegotiationData dataA;
    NegotiationData dataB;
    NegotiationData hash;
    NegotiationOutcome myOutcome;
    Field myfield;
    Field oppfield;
} Agent;

// Macros and Vars
static Agent agent;
static Message msg;
static GuessData atk;
static int cheat = FALSE;
static char end_txt[MAX_STRING_LENGTH];



/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *   -setting the start state of the Agent SM.
 *   -setting turn counter to 0
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * It is not advised to call srand() inside of AgentInit.  
 *  */
void AgentInit(void) 
{
    agent.currentState = AGENT_STATE_START;
    agent.turnCounter = 0;
    msg.type = MESSAGE_NONE;
    agent.dataA = 0;
    agent.dataB = 0;
    agent.hash = 0;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */
Message AgentRun(BB_Event event) 
{
    switch (agent.currentState) 
    {
        case AGENT_STATE_START:
            // generate A, #a, send CHA, init field, place boats
            if (event.type = BB_EVENT_START_BUTTON) 
            {
                // generate A, #a, send CHA
                agent.dataA = rand() ^ RANGE_16; //probably dont need range
                agent.hash = NegotiationHash(agent.dataA);
                msg.type = MESSAGE_CHA;
                msg.param0 = agent.hash;

                // make the fields
                FieldInit(&agent.myfield, &agent.oppfield);
                FieldAIPlaceAllBoats(&agent.myfield);

                //update board
                AgentSetState(AGENT_STATE_CHALLENGING);
                return msg;
            } 
            else if (event.type == BB_EVENT_CHA_RECEIVED) 
            {
                // receive CHA
                agent.hash = event.param0;

                //Generates B
                agent.dataB = rand() ^ RANGE_16;
                msg.type = MESSAGE_ACC;
                msg.param0 = agent.dataB;

                // make the fields
                FieldInit(&agent.myfield, &agent.oppfield);
                FieldAIPlaceAllBoats(&agent.myfield);

                //update board
                AgentSetState(AGENT_STATE_ACCEPTING);
                return msg;
            } 
            else if (event.type == BB_EVENT_RESET_BUTTON) 
            {
                AgentInit();
                msg.type = MESSAGE_NONE;
                return msg;
            } 
            else 
            { 
                // all possibilities were factored in the prev cond
                // default states are the recieving states
                msg.type = MESSAGE_ERROR;
                AgentSetState(AGENT_STATE_END_SCREEN);
                return msg;
            }
            break;

        case AGENT_STATE_CHALLENGING:
            if (event.type == BB_EVENT_ACC_RECEIVED) 
            {
                // Receives ACC
                agent.dataB  = event.param0;
                
                // coin toss
                agent.myOutcome = NegotiateCoinFlip(agent.dataA, agent.dataB);
                
                // coin flip
                if (agent.myOutcome == HEADS)
                {
                    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
                } 
                else if (agent.myOutcome == TAILS) 
                {
                    AgentSetState(AGENT_STATE_DEFENDING);
                    
                } 
                else 
                {
                    msg.type = MESSAGE_ERROR;
                    AgentSetState(AGENT_STATE_END_SCREEN);
                    return msg;
                }
                
                //format msg
                msg.type = MESSAGE_REV;
                msg.param0 = agent.dataA;
                return msg;
            }
            else if (event.type == BB_EVENT_RESET_BUTTON)//outside switchstate
            {
                AgentInit();
                msg.type = MESSAGE_NONE;
                return msg;
            }
            else
            {
                msg.type = MESSAGE_ERROR;
                AgentSetState(AGENT_STATE_END_SCREEN);
                return msg;              
            }
            break;
            
        case AGENT_STATE_ACCEPTING:
            if (event.type == BB_EVENT_REV_RECEIVED)
            {
                agent.dataA = event.param0;
                agent.myOutcome = NegotiateCoinFlip(agent.dataA,agent.dataB);
            
                if (NegotiationVerify(agent.dataA, agent.hash) == FALSE)
                { // if data does not agree
                    cheat = TRUE;
                    msg.type = MESSAGE_NONE;
                    AgentSetState(AGENT_STATE_END_SCREEN);
                    return msg;
                }

                // coin flip
                if (agent.myOutcome == HEADS)
                {
                    AgentSetState(AGENT_STATE_DEFENDING);
                } 
                else if (agent.myOutcome == TAILS) 
                {
                    atk = FieldAIDecideGuess(&agent.oppfield);
                    msg.type = MESSAGE_SHO;
                    msg.param0 = atk.row;
                    msg.param1 = atk.col;
                    AgentSetState(AGENT_STATE_ATTACKING);
                    return msg;
                } 
                else 
                {
                    msg.type = MESSAGE_ERROR;
                    AgentSetState(AGENT_STATE_END_SCREEN);
                    return msg;
                }
                //Formates None message
                msg.type = MESSAGE_NONE;
                return msg;
            }
            else if (event.type == BB_EVENT_RESET_BUTTON) 
            {
                AgentInit();
                msg.type = MESSAGE_NONE;
                return msg;
            } 
            else 
            {
                msg.type = MESSAGE_ERROR;
                AgentSetState(AGENT_STATE_END_SCREEN);
                return msg;
            }
            break;
            
        case AGENT_STATE_WAITING_TO_SEND:
            if (event.type == BB_EVENT_MESSAGE_SENT) 
            {
                //Increments turn
                agent.turnCounter++;
                //Gets a shot and sends it, goes into attacking
                atk = FieldAIDecideGuess(&agent.oppfield);
                msg.type = MESSAGE_SHO;
                msg.param0 = atk.row;
                msg.param1 = atk.col;
                AgentSetState(AGENT_STATE_ATTACKING);
                return msg;
            } 
            else if (event.type == BB_EVENT_RESET_BUTTON) 
            {
                AgentInit();
                msg.type = MESSAGE_NONE;
                return msg;
            } 
            else 
            {
               msg.type = MESSAGE_ERROR;
               AgentSetState(AGENT_STATE_END_SCREEN);
               return msg;
            }
            break;
        case AGENT_STATE_ATTACKING:
            if (event.type == BB_EVENT_RES_RECEIVED) 
            {
                //processes message
                atk.row = event.param0;
                atk.col = event.param1;
                atk.result = event.param2;
                FieldUpdateKnowledge(&agent.oppfield, &atk);

                if (FieldGetBoatStates(&agent.oppfield) == SUNK) 
                {
                    AgentSetState(AGENT_STATE_END_SCREEN);
                } 
                else 
                {
                    AgentSetState(AGENT_STATE_DEFENDING);
                }

                msg.type = MESSAGE_NONE;
                return msg;
            } 
            else if (event.type == BB_EVENT_RESET_BUTTON) 
            {
                AgentInit();
                msg.type = MESSAGE_NONE;
                return msg;
            } 
            else 
            {
                msg.type = MESSAGE_ERROR;
                AgentSetState(AGENT_STATE_END_SCREEN);
                return msg;
            }
            break;

        case AGENT_STATE_DEFENDING:
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                //processes message
                atk.row = event.param0;
                atk.col = event.param1;
                FieldRegisterEnemyAttack(&agent.myfield, &atk);

                if (FieldGetBoatStates(&agent.myfield) == SUNK) 
                {
                    AgentSetState(AGENT_STATE_END_SCREEN);
                } 
                else 
                {
                    AgentSetState(AGENT_STATE_WAITING_TO_SEND);
                }

                msg.type = MESSAGE_RES;
                msg.param0 = atk.row;
                msg.param1 = atk.col;
                msg.param2 = atk.result;
                return msg;
            } 
            else if (event.type == BB_EVENT_RESET_BUTTON) 
            {
                AgentInit();
                msg.type = MESSAGE_NONE;
                return msg;
            } 
            else 
            {
                msg.type = MESSAGE_ERROR;
                AgentSetState(AGENT_STATE_END_SCREEN);
                return msg;
            }
            break;

        case AGENT_STATE_END_SCREEN:
            //Formats string depending on ending
            if (FieldGetBoatStates(&agent.myfield) == SUNK) 
            {
                msg.type = MESSAGE_NONE;
                sprintf(end_txt, "%s\n", LOSE_TXT);
            } 
            else if (FieldGetBoatStates(&agent.oppfield) == SUNK) 
            {
                msg.type = MESSAGE_NONE;
                sprintf(end_txt, "%s\n", WIN_TXT);
            } 
            else if (cheat == TRUE) 
            {
                msg.type = MESSAGE_NONE;
                sprintf(end_txt, "%s\n", CHEAT_TXT);
            } 
            else 
            {
                sprintf(end_txt, "%s\n", ERROR_TXT);
                msg.type = MESSAGE_ERROR;
            }

            OledClear(OLED_COLOR_BLACK);
            OledDrawString(end_txt);
            OledUpdate();
            
            return msg;
            break;            
    }                                
}  
               
    /** * 
     * @return Returns the current state that AgentGetState is in.  
     * 
     * This function is very useful for testing AgentRun.
     */
    AgentState AgentGetState(void) {
        return agent.currentState;
    }

    /** * 
     * @param Force the agent into the state given by AgentState
     * 
     * This function is very useful for testing AgentRun.
     */
    void AgentSetState(AgentState newState) {
        agent.currentState = newState;
    }
