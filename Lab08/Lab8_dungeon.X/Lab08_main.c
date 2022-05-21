// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CSE013E Support Library
#include "UNIXBOARD.h"
#include "Game.h"

static char Title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
static char Description[GAME_MAX_ROOM_DESC_LENGTH + 1];
static char input;


// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****

// **** Declare any function prototypes here ****

int main()
{


    /******************************** Your custom code goes below here ********************************/
    
    if(GameInit()== 0)
    {
        FATAL_ERROR();
    }
    
    while(1)
    {
        //print room title
        GameGetCurrentRoomTitle(Title);
        printf("%s\n\n",Title);
    
        //print room description
        GameGetCurrentRoomDescription(Description);
        printf("%s\n\n",Description);
    
        //print possible exits
        if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS)
        {
            printf("North exit exists!\n\n"); // The current room has a north exit.
        }
    
        if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS)
        {
            printf("East exit exists!\n\n"); // The current room has a east exit.
        }
    
        if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS)
        {
            printf("South exit exists!\n\n"); // The current room has a west exit.
        }
        
        if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS)
        {
            printf("West exit exists!\n\n"); // The current room has a west exit.
        }
    
        printf("Pick a direction!\n     n = North\n     e = East\n     s = South\n     w = west\n     q = quit\n\n");
    
        printf("Input:");
        input = getchar(); //for direction

    /*  if not n, e, s, w, q, printf(invalid input)
        if n GameGoNorth(void); printf("You decide to go north);
        if e GameGoEast(void); printf("You decide to go east);
        if s GameGoSouth(void); printf("You decide to go south);
        if w GameGoWest(void); printf("You decide to go west);
        if q break 
    */
        while (getchar() != '\n')//flush the input buffer if there's leftover
            ;
        
            if(input == 'n')
            {
                if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS)
                {
                    printf("\nYou decide to go north...\n");
                    GameGoNorth();
                }
                else
                {
                    printf("\nYou can't go that way...\n");
                } 
            }

            else if(input == 'e')
            {
                if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS)
                {
                    printf("\nYou decide to go east...\n");
                    GameGoEast();
                }
                    else
                {
                    printf("\nYou can't go that way...\n");
                }
            }
    
            else if(input == 's')
            {
                if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS)
                {
                    printf("\nYou decide to go south...\n");
                    GameGoSouth();
                }
                else
                {
                    printf("\nYou can't go that way...\n");
                }
            }
    
            else if(input == 'w')
            {
                if(GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS)
                {
                    printf("\nYou decide to go west...\n");
                    GameGoWest();
                }   
                else
                {
                    printf("You can't go that way...\n");
                }   
            }
    
            else if(input == 'q')
            {
                printf("\nYou exit the game...\n\n");
                exit(SUCCESS);
            }
        
            else
            {
                printf("\nNot a valid input\n\n");
            }
        }
    /**************************************************************************************************/
}

