#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "BOARD.h"
#include <string.h>

#include "Message.h"
#include "BattleBoats.h"

// define message payload
#define CHALLENGE "CHA"
#define ACCEPT "ACC"
#define REVEAL "REV"
#define SHOT "SHO"
#define RESULT "RES"

#define STARTDELIMITER '$'
#define CHECKSUMDELIMITER '*'
#define ENDDELIMITER '\n'
#define ZEROASCII '0' //48
#define NINEASCII '9'//57
#define AASCII '9'//65
#define FASCII 'F'//70

typedef enum {
    WAITING, RECORD_PAYLOAD, RECORD_CHECKSUM
} Decodemode;

static Decodemode mode = WAITING;
static char decode_payload[MESSAGE_MAX_LEN + 1];
static char decode_checksum[MESSAGE_CHECKSUM_LEN + 2];
static int payloadlen = 0;
static int checksumlen = 0;

/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */
uint8_t Message_CalculateChecksum(const char* payload)
{
    uint8_t result = 0;
    int a = 0;
    
    while(payload[a] != '\0')  //while the next item isn't null
    {
        result ^= payload[a];  //bitwise xor the string
        a++;
    }
    return result;
}

/**
 * ParseMessage() converts a message string into a BB_Event.  The payload and
 * checksum of a message are passed into ParseMessage(), and it modifies a
 * BB_Event struct in place to reflect the contents of the message.
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //A BB_Event which will be modified by this function.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event)
{
    uint8_t conversion;
    
    conversion = strtoul(checksum_string, NULL, 16);
    
    // prompt error if checksum not up to spec
    if(Message_CalculateChecksum(payload) != conversion)
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // clear parameters
    message_event->param0 = 0;  
    message_event->param1 = 0;
    message_event->param2 = 0;
    
    if(strlen(checksum_string) != MESSAGE_CHECKSUM_LEN)  //if checksum string is not the correct length
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    char tempPayload[MESSAGE_MAX_PAYLOAD_LEN]; 
    strcpy(tempPayload, payload);
    
    // mk copy of payload to extract token
    char *tok = strtok(tempPayload, ",");
    
    // Tokens we expect, given from event
    int expectedTokens;   
   
    if(strcmp(tok, "CHA") == 0)  // if the tok is the same as the event
    { 
        expectedTokens = 1;
        message_event->type = BB_EVENT_CHA_RECEIVED;    // sw val to the 
    }
    else if(strcmp(tok, "ACC") == 0)
    {
        expectedTokens = 1;
        message_event->type = BB_EVENT_ACC_RECEIVED;
    }
    else if(strcmp(tok, "REV") == 0)
    {
        expectedTokens = 1;
        message_event->type = BB_EVENT_REV_RECEIVED;
    }
    else if(strcmp(tok, "SHO") == 0)
    {
        expectedTokens = 2;
        message_event->type = BB_EVENT_SHO_RECEIVED;
    }
    else if(strcmp(tok, "RES") == 0)
    {
        expectedTokens = 3;
        message_event->type = BB_EVENT_RES_RECEIVED;
    }
    else
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // iterate thru toks
    int ele;
    for(ele = 0; ele < expectedTokens; ele++)
    {
        uint16_t temp_param;
        tok = strtok(NULL, ",");
        
        if(tok == NULL)
        {
            message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
   
        temp_param = atoi(tok); // str to int
        
        if (ele == 0)
        {
           message_event->param0 = temp_param;    
        }
        else if (ele == 1)
        {
           message_event->param1 = temp_param;    
        }
        else if (ele == 2)
        {
           message_event->param2 = temp_param;    
        }
    }
    /*tok = strtok(NULL, ",");  //checks if the next token is not a NULL, then the length is too long
 
    if(tok)
    {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }*/
    return SUCCESS;
}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 typedef enum {
    MESSAGE_NONE, //used if no message is to be sent
    MESSAGE_CHA,
    MESSAGE_ACC,
    MESSAGE_REV,
    MESSAGE_SHO,
    MESSAGE_RES,   
    //while not required, an error message can be a useful debugging tool:
    MESSAGE_ERROR = -1, 
} MessageType;
 */
int Message_Encode(char *message_string, Message message_to_encode)
{
    uint8_t checksum_temp;
    char msg_temp[MESSAGE_MAX_PAYLOAD_LEN];
    switch(message_to_encode.type)
    {       
        case MESSAGE_NONE:
            return 0;
            break;
            
        case MESSAGE_ERROR:
            break;
      
        case MESSAGE_CHA:
            sprintf(msg_temp, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            checksum_temp= Message_CalculateChecksum(msg_temp);
            sprintf(message_string, MESSAGE_TEMPLATE, msg_temp, checksum_temp);   
            break;
            
        case MESSAGE_ACC:
            sprintf(msg_temp, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            checksum_temp= Message_CalculateChecksum(msg_temp);
            sprintf(message_string, MESSAGE_TEMPLATE, msg_temp, checksum_temp);   
            break;
        case MESSAGE_REV:
            sprintf(msg_temp, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            checksum_temp= Message_CalculateChecksum(msg_temp);
            sprintf(message_string, MESSAGE_TEMPLATE, msg_temp, checksum_temp);   
            break;
        case MESSAGE_SHO:
            sprintf(msg_temp, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            checksum_temp= Message_CalculateChecksum(msg_temp);
            sprintf(message_string, MESSAGE_TEMPLATE, msg_temp, checksum_temp);   
            break;
            
        case MESSAGE_RES:
            sprintf(msg_temp, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            checksum_temp= Message_CalculateChecksum(msg_temp);
            sprintf(message_string, MESSAGE_TEMPLATE, msg_temp, checksum_temp);   
            break;
    }
    return strlen(message_string);
}


/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                          if char_in is the last character of an invalid message,
 *                              then decoded_message should have an ERROR type.
 *                          otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 * typedef enum {
    BB_EVENT_NO_EVENT, //0
    BB_EVENT_START_BUTTON, //1
    BB_EVENT_RESET_BUTTON, //2
    BB_EVENT_CHA_RECEIVED, //3
    BB_EVENT_ACC_RECEIVED, //4
    BB_EVENT_REV_RECEIVED, //5
    BB_EVENT_SHO_RECEIVED, //6
    BB_EVENT_RES_RECEIVED, //7
    BB_EVENT_MESSAGE_SENT, //8
    BB_EVENT_ERROR, //9

    //the following events are only used for human agents:
    BB_EVENT_SOUTH_BUTTON, //10
    BB_EVENT_EAST_BUTTON, //11
} BB_EventType;
 * 
typedef struct {
    BB_EventType type;
    uint16_t param0; //defined in Message.h
    uint16_t param1;
    uint16_t param2;
} BB_Event;
 * 
 typedef enum {
    BB_SUCCESS = 0, //0
    BB_ERROR_BAD_CHECKSUM, //1
    BB_ERROR_PAYLOAD_LEN_EXCEEDED, //2
    BB_ERROR_CHECKSUM_LEN_EXCEEDED, //3
    BB_ERROR_CHECKSUM_LEN_INSUFFICIENT, //3
    BB_ERROR_INVALID_MESSAGE_TYPE, //4
    BB_ERROR_MESSAGE_PARSE_FAILURE,
} BB_Error; 
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event)
{
    switch(mode)
    {
        case WAITING:
            if(char_in == STARTDELIMITER)  
            {
                mode = RECORD_PAYLOAD;
                decoded_message_event->type = BB_EVENT_NO_EVENT;
                payloadlen = 0;
            }
            else  //raise error if not start delimiter
            {
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                return STANDARD_ERROR;
            }
            break;
            
        case RECORD_PAYLOAD:
            if(payloadlen > MESSAGE_MAX_PAYLOAD_LEN)  
            {
                mode = WAITING;
                decoded_message_event->type = BB_EVENT_ERROR;
                //decoded_message_event->param0 = BB_ERROR_PAYLOAD_LEN_EXCEEDED;
                return STANDARD_ERROR;
            }
            if(char_in == STARTDELIMITER || char_in == ENDDELIMITER)  //raise error unexpected delimiter ($) or (\n)
            {
                mode = WAITING;
                decoded_message_event->type = BB_EVENT_ERROR;
                return STANDARD_ERROR;
            }
            if(char_in == CHECKSUMDELIMITER)
            {
                mode = RECORD_CHECKSUM;
                decode_payload[payloadlen] = '\0';
                checksumlen = 0;
            }
            else
            {
                decode_payload[payloadlen] = char_in;
                payloadlen += 1;
            }
            break;
            
        case RECORD_CHECKSUM:
            if(char_in == ENDDELIMITER)
            {
                if(checksumlen != MESSAGE_CHECKSUM_LEN)  //if incorrect checksum length
                {
                    decoded_message_event->type = BB_EVENT_ERROR;
                    mode = WAITING;
                    return STANDARD_ERROR;
                }
                /*if(checksumlen > MESSAGE_CHECKSUM_LEN)  //if incorrect checksum length
                {
                    decoded_message_event->type = BB_EVENT_ERROR;
                    //decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_EXCEEDED;
                    mode = WAITING;
                    return STANDARD_ERROR;
                }
                if(checksumlen < MESSAGE_CHECKSUM_LEN)  //if incorrect checksum length
                {
                    decoded_message_event->type = BB_EVENT_ERROR;
                    //decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_INSUFFICIENT;
                    mode = WAITING;
                    return STANDARD_ERROR;
                }*/
            }   
            if(checksumlen == MESSAGE_CHECKSUM_LEN)
            {
                if(char_in != ENDDELIMITER)
                {
                    decoded_message_event->type = BB_EVENT_ERROR;
                    mode = WAITING;
                    return STANDARD_ERROR;
                }
                
                int parse;
                parse = Message_ParseMessage(decode_payload, decode_checksum, decoded_message_event);
                
                if(parse)  //if checksum matches payload
                {
                    mode = WAITING;
                    return SUCCESS;
                }
                else
                {
                    mode = WAITING;
                    decoded_message_event->type = BB_EVENT_ERROR;
                    return STANDARD_ERROR;
                }  
            }
            if(((ZEROASCII <= char_in) && (char_in <= NINEASCII)) || ((AASCII <= char_in) && (char_in <= FASCII)))
            {
                decode_checksum[checksumlen] = char_in;
                checksumlen += 1;
            }
            else
            {
                mode = WAITING;
                decoded_message_event->type = BB_EVENT_ERROR;
                return STANDARD_ERROR;
            }
            break;
    }
    return SUCCESS;
}
