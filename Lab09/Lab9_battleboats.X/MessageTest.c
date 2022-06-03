#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Message.h"
#include "BOARD.h"

static int TestsPassed = 0;
static char *payload;
static uint8_t ProcessPayload;
static uint8_t checksum;

static char *checkParse;
static int parseRun;
static BB_Event testEvent;

static Message testMessage;
static char encodeMessage[MESSAGE_MAX_LEN];
static char *decodeMessage;


int main() 
{
    printf("Running tests for Message.c:\n\n");
    
    //Checksum Tests
    printf("Test Message_CalculateChecksum():\n");
    
    payload = "CHA,3";  //check input with expected value
    ProcessPayload = Message_CalculateChecksum(payload);
    checksum = ProcessPayload;
    if(checksum == 0x55)
    {
        printf("Test 1: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 1: Failed\n");
    }
    
    payload = "SHO,5,6";
    ProcessPayload = Message_CalculateChecksum(payload);
    checksum = ProcessPayload;
    if(checksum == 0x57)
    {
        printf("Test 2: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 2: Failed\n");
    }
    
    //ParseMessage
    printf("\nTest Message_ParseMessage():\n");
    
    //Test 1 is for an expected value
    payload = "CHA,1"; //parses with expected value
    checkParse = "57";
    parseRun = Message_ParseMessage(payload, checkParse, &testEvent);
    
    if(testEvent.type == BB_EVENT_CHA_RECEIVED && testEvent.param0 == 1) 
    {
        printf("Test 1: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 1: Failed\n");
        printf("Event type: %d\n",testEvent.type);
        printf("Param 0: %d\n", testEvent.param0);
    }
    
    //Test 2 is for an unexpected value
    payload = "CHA,1,2"; 
    checkParse = "57";
    parseRun = Message_ParseMessage(payload, checkParse, &testEvent);
    
    if(testEvent.type == BB_EVENT_ERROR)
    {
        printf("Test 2: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 2: Failed\n");
        printf("Event type: %d\n",testEvent.type);
        printf("Param 0: %d\n", testEvent.param0);
        printf("Param 1: %d\n", testEvent.param1);
    }
    
    //Encode
    printf("\nTest Message_Encode():\n");
    testMessage.param0 = 1;
    testMessage.type = MESSAGE_CHA;
    Message_Encode(encodeMessage, testMessage);
    //printf("%s", encodeMessage);
    if(strcmp(encodeMessage, "$CHA,1*57\n") == 0)
    {
        printf("Test 1: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 1: Failed\n");
        printf("Encoded message:%s\n", encodeMessage);
    }
    
    testMessage.param0 = 6;
    testMessage.param1 = 9;
    testMessage.type = MESSAGE_SHO;
    Message_Encode(encodeMessage, testMessage);
    //printf("%s", encodeMessage);
    if(strcmp(encodeMessage, "$SHO,6,9*5B\n") == 0)
    {
        printf("Test 2: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 2: Failed\n");
        printf("Encoded message:%s\n", encodeMessage);
    }
    
    //Decode
    int a = 0;
    printf("\nTests for Decode:\n");
    decodeMessage = "$RES,4,2,0*5E\n"; //parameters 420
    for(a = 0; a < strlen(decodeMessage); a++)
    {
        Message_Decode(decodeMessage[a], &testEvent);
    }
    
    if(testEvent.type == BB_EVENT_RES_RECEIVED && testEvent.param0 == 4 && testEvent.param1 == 2 && testEvent.param2 == 0)
    {
        printf("Test 1: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 1: Failed\n");
        printf("Event type:%d\n",testEvent.type);
        printf("Param 0: %d\n", testEvent.param0);
        printf("Param 1: %d\n", testEvent.param1);
    }
    
    decodeMessage = "$SHO,6,9*5B\n"; //parameters 69 ;)
    for(a = 0; a < strlen(decodeMessage); a++)
    {
        Message_Decode(decodeMessage[a], &testEvent);
    }
    
    
    if(testEvent.type == BB_EVENT_SHO_RECEIVED && testEvent.param0 == 6 && testEvent.param1 == 9)
    {
        printf("Test 2: Passed\n");
        TestsPassed += 1;
    }
    else
    {
        printf("Test 2: Failed\n");
        printf("Event type:%d\n",testEvent.type);
        printf("Param 0: %d\n", testEvent.param0);
        printf("Param 1: %d\n", testEvent.param1);
        
    }
    
    if(TestsPassed == 8)
    {
        printf("\nAll tests passed successfully! %d/8 Tests Passed\n", TestsPassed);
    }
    else
    {
        printf("\nSome tests failed! %d/8 Tests Passed\n", TestsPassed);
    }
    return (EXIT_SUCCESS);
}