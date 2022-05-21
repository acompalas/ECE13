#include "Game.h"
#include "Player.h"
#include "UNIXBOARD.h"
#include <stdio.h>
#include <string.h>

#define Header 3

typedef struct {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char description[GAME_MAX_ROOM_DESC_LENGTH + 1];
    uint8_t itemrequired;
    uint8_t itemcontained;
    uint8_t northexit;
    uint8_t southexit;
    uint8_t eastexit;
    uint8_t westexit;
    uint8_t currentexits;
    
}RoomData;


static RoomData roomdata;

int ReadRoom(int r);

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void)
{
    if(roomdata.northexit != 0)
    {
        ReadRoom(roomdata.northexit);
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void)
{
    if(roomdata.eastexit != 0)
    {
        ReadRoom(roomdata.eastexit);
    }
    return STANDARD_ERROR;
}


/**
 * @see GameGoNorth
 */
int GameGoSouth(void)
{
    if(roomdata.southexit != 0)
    {
        ReadRoom(roomdata.southexit);
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void)
{
    if(roomdata.westexit != 0)
    {
        ReadRoom(roomdata.westexit);
    }
    return STANDARD_ERROR;
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void)
{
    ReadRoom(STARTING_ROOM);
    return SUCCESS;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title)
{
    strcpy(title, roomdata.title);
    return strlen(title);
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc)
{
    strcpy(desc, roomdata.description);
    return strlen(desc);
}
    
/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void)
{ // 1000
    roomdata.currentexits = 0;
    if(roomdata.northexit != 0)
    {
        roomdata.currentexits |= GAME_ROOM_EXIT_NORTH_EXISTS;
    }
    if(roomdata.eastexit != 0)
    {
        roomdata.currentexits |= GAME_ROOM_EXIT_EAST_EXISTS;
    }
    if(roomdata.southexit != 0)
    {
        roomdata.currentexits |= GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    if(roomdata.westexit != 0)
    {
        roomdata.currentexits |= GAME_ROOM_EXIT_WEST_EXISTS;
    }
    return roomdata.currentexits;
}

int ReadRoom(int r)
{
    //declare variables
    char filestr[80];
    int title_len;
    int itemreq_len;
    int items_obtained = 0;
    int items_added = 0;
    int itemcont_len;
    int desc_len;
    
    //open file
    sprintf(filestr, "RoomFiles/room%d.txt", r);
    FILE *file = fopen(filestr, "rb");
    
    //check NULL
    if(file == NULL)
    {
        FATAL_ERROR();
    }
    
    //skip header
    fseek(file, Header, SEEK_SET);
    
    //get title length then read title
    title_len = fgetc(file); 
    fread(roomdata.title, title_len, 1, file);
    roomdata.title[title_len] = '\0';
    
    //store number of items required
    itemreq_len = fgetc(file);
    
    //iterate through required items
    for (int i = 0; i < itemreq_len; i++)
    {
        roomdata.itemrequired = fgetc(file); //get item
        if(FindInInventory(roomdata.itemrequired) == SUCCESS) //if item exists in inventory
        {
            items_obtained += 1; //add to number of required items contained   
        }
        else
        {
            break;
        }
    }
    
    if(items_obtained == itemreq_len)
    {
        //get length of first description read first description
        desc_len = fgetc(file);
        fread(roomdata.description, desc_len, 1, file);
        roomdata.description[desc_len] = '\0';       
        //get length of contained items
        itemcont_len = fgetc(file);
        //add items to inventory
        for(int j = 0; j < itemcont_len; j++)
        {
            roomdata.itemcontained = fgetc(file);
            if(AddToInventory(roomdata.itemcontained) == SUCCESS)
            {
                items_added += 1;
            }
        }   

        if(items_added == itemcont_len)
        {
            //store exits
            roomdata.northexit = fgetc(file); 
            roomdata.eastexit = fgetc(file);
            roomdata.southexit = fgetc(file);
            roomdata.westexit = fgetc(file);
            return SUCCESS;
        }
        else
        {
            return STANDARD_ERROR;
        }
    }
    
    else if(FindInInventory(roomdata.itemrequired) == STANDARD_ERROR)// item required wasn't in inventory
    {
        //skip first description and go to second description 
        desc_len = fgetc(file); 
        fseek(file, desc_len, SEEK_CUR); 
            
        //skip items contained 
        itemcont_len = fgetc(file); 
        fseek(file, itemcont_len, SEEK_CUR);
            
        //skip exits of of room
        fseek(file, 5, SEEK_CUR);
        desc_len = fgetc(file);
            
        //read desc of room
        fread(roomdata.description, desc_len, 1, file);    
            
        //read contained items (it should always be zero)
        itemcont_len = fgetc(file);
            
        //store exits for room.type = WITHOUT_ITEM
        roomdata.northexit = fgetc(file);
        roomdata.eastexit = fgetc(file);
        roomdata.southexit = fgetc(file);
        roomdata.westexit = fgetc(file);
        return SUCCESS;
    }
    return STANDARD_ERROR;
}