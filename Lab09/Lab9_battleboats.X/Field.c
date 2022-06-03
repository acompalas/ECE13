
// **** Include libraries here ****
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "BOARD.h"
#include "Field.h"

#define MAX_STRING_LENGTH 100
#define DIR 0x01


/**
 * This function is optional, but recommended.   It prints a representation of both
 * fields, similar to the OLED display.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     SquareStatus.
 * typedef struct {
    uint8_t row; // Row of the coordinate that was guessed
    uint8_t col; // Column of the coordinate guessed
    ShotResult result; // result of a shot at this coordinate
} GuessData;
 */
void FieldPrint_UART(Field *own_field, Field *opp_field)
{
    //grid strings 
    char ownDisplay[MAX_STRING_LENGTH];
    char oppDisplay[MAX_STRING_LENGTH];
    
    //iterables for loops and strings
    int i, j;
    int grid1 = 0;
    int grid2 = 0;

    //Fills ownDisplay with information from own_field
    for (i = 0; i < FIELD_ROWS; i++) 
    {
        for (j = 0; j < FIELD_COLS; j++) 
        {
            if (own_field->grid[i][j] == FIELD_SQUARE_EMPTY) 
            {
                ownDisplay[grid1] = '.';
            } 
            else if (own_field->grid[i][j] == FIELD_SQUARE_SMALL_BOAT) 
            {
                ownDisplay[grid1] = '3';
            } 
            else if (own_field->grid[i][j] == FIELD_SQUARE_MEDIUM_BOAT) 
            {    
                ownDisplay[grid1] = '4';
            } 
            else if (own_field->grid[i][j] == FIELD_SQUARE_LARGE_BOAT) 
            {   
                ownDisplay[grid1] = '5';
            } 
            else if (own_field->grid[i][j] == FIELD_SQUARE_HUGE_BOAT) 
            {
                ownDisplay[grid1] = '6';
            } 
            else if (own_field->grid[i][j] == FIELD_SQUARE_HIT) 
            {
                ownDisplay[grid1] = '*';
            } 
            else if (own_field->grid[i][j] == FIELD_SQUARE_MISS) 
            {
                ownDisplay[grid1] = 'x';
            } 
            else 
            {
                ownDisplay[grid1] = '.';
            }
            grid1 += 1;
        }
        ownDisplay[grid1] = '\n';
        grid1 += 1;
    }

    //Prints own_field
    printf("\n----------\n");
    printf("%s", ownDisplay);
    printf("----------\n");

    //Fills oppDisplay with information from opp_field
    for (i = 0; i < FIELD_ROWS; i++) 
    {
        for (j = 0; j < FIELD_COLS; j++) 
        {
            if (opp_field->grid[i][j] == FIELD_SQUARE_UNKNOWN) 
            {
                oppDisplay[grid2] = '.';
            } else if (opp_field->grid[i][j] == FIELD_SQUARE_HIT) 
            {
                oppDisplay[grid2] = '*';
            } else if (opp_field->grid[i][j] == FIELD_SQUARE_MISS) 
            {
                oppDisplay[grid2] = 'x';
            } else if (opp_field->grid[i][j] == FIELD_SQUARE_EMPTY) 
            {
                oppDisplay[grid2] = '.';
            } else {
                oppDisplay[grid2] = '.';
            }
            grid2 += 1;
        }
        oppDisplay[grid2] = '\n';
        grid2 += 1;
    }
    
    //Prints opp_field
    printf("\n----------\n");
    printf("%s", oppDisplay);
    printf("----------\n");
}


/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus (
 * FIELD_SQUARE_EMPTY for your own field, FIELD_SQUARE_UNKNOWN for opponent's).
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be filled when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */
void FieldInit(Field *own_field, Field *opp_field)
{
    for(int i = 0; i < FIELD_ROWS; i++)
    {
        for(int j = 0; j < FIELD_COLS; j++)
        {
            own_field->grid[i][j] = FIELD_SQUARE_EMPTY; //set ownfield to empty
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN; //set opp_field to unknown
        }
    }
    //Add lives
    own_field->hugeBoatLives = 0;
    own_field->largeBoatLives = 0;
    own_field->mediumBoatLives = 0;
    own_field->smallBoatLives = 0;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  FIELD_SQUARE_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced square 
 */
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col)
{
    SquareStatus contents = f->grid[row][col];
    if(row >= FIELD_ROWS || row < 0)
    {
        return FIELD_SQUARE_INVALID;
    }
    if(col >= FIELD_COLS || col < 0)
    {
        return FIELD_SQUARE_INVALID;
    }
    else
    {
        return contents;
    }
    //return whatever is in i,j
}


/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p)
{
    SquareStatus old_value = f->grid[row][col]; //save f[row][col] to variable
    f->grid[row][col] = p;
    return old_value; //return saved variable
}



/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. 
 * 
 * All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 * 
 * In addition, this function should update the appropriate boatLives parameter of the field.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type)
{
    //adding boat 
    //iterate size of boat and call the set function
    //if...SUCCESS
    //else...STANDARD_ERROR
    BoatSize boat_size;
    SquareStatus squareType;
    int bounds;
    
    //Determine type of boat
    if(boat_type == FIELD_BOAT_TYPE_SMALL)
    {
        boat_size = FIELD_BOAT_SIZE_SMALL;
        squareType = FIELD_SQUARE_SMALL_BOAT;
    }
    else if(boat_type == FIELD_BOAT_TYPE_MEDIUM)
    {
        boat_size = FIELD_BOAT_SIZE_MEDIUM;
        squareType = FIELD_SQUARE_MEDIUM_BOAT;
    }
    else if(boat_type == FIELD_BOAT_TYPE_LARGE)
    {
        boat_size = FIELD_BOAT_SIZE_LARGE;
        squareType = FIELD_SQUARE_LARGE_BOAT;
    }
    else if(boat_type == FIELD_BOAT_TYPE_HUGE)
    {
        boat_size = FIELD_BOAT_SIZE_HUGE;
        squareType = FIELD_SQUARE_HUGE_BOAT;
    }
    else
    {
        return STANDARD_ERROR;
    }
    
    //Place boat
    if(dir == FIELD_DIR_SOUTH)
    {
        bounds = row + boat_size;

        //check if there is room to place boat
        if(bounds > FIELD_ROWS)
        {
            return STANDARD_ERROR;
        }
        
        //check if squares not already occupied
        for(int i = row; i < bounds; i++)
        {
            if(own_field->grid[i][col] == FIELD_SQUARE_EMPTY)
            {
                continue;
            }
            else
            {
                return STANDARD_ERROR; //if square is not empty 
            }
        }
        
        //Add boat
        for (int ii = row; ii < bounds; ii++) 
        {
            own_field->grid[ii][col] = squareType;
        }
    }
    if(dir == FIELD_DIR_EAST)
    {
        bounds = col + boat_size;
        
        //check if there is room to place boat
        if(bounds > FIELD_COLS)
        {
            return STANDARD_ERROR;
        }
        
        //check if squares not already occupied
        for(int j = col; j < boat_size; j++)
        {
            if(own_field->grid[row][j] == FIELD_SQUARE_EMPTY)
            {
                continue;
            }
            else
            {
                return STANDARD_ERROR; //if square is not empty 
            }
        }
        
        //Add boat
        for (int jj = col; jj < bounds; jj++) 
        {
            own_field->grid[row][jj] = squareType;
        }
    }
    //Set lives of boats
    if(boat_type == FIELD_BOAT_TYPE_SMALL) 
    {
        own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    } 
    else if(boat_type == FIELD_BOAT_TYPE_MEDIUM) 
    {
        own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    } 
    else if(boat_type == FIELD_BOAT_TYPE_LARGE) 
    {
        own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    } 
    else if(boat_type == FIELD_BOAT_TYPE_HUGE) 
    {
        own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
    } 
    else 
    {
        return STANDARD_ERROR;
    }

    return SUCCESS;
}


/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.  The result can be a RESULT_HIT, RESULT_MISS, or RESULT_***_SUNK.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess)
{
    //update f with FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on f[opp_guess->row][opp_guess->col]
    //update gdata with HitStatus value: what happened(if something) and SquareStatus FieldGetSquareStatus() 
    //reduces lives for any boat hit
    uint8_t row = opp_guess->row;
    uint8_t col = opp_guess->col;
    SquareStatus getstatus = FieldGetSquareStatus(own_field, row, col);
    
    if(getstatus == FIELD_SQUARE_EMPTY)
    {
        own_field->grid[row][col] = FIELD_SQUARE_MISS;
        opp_guess->result = RESULT_MISS;
    }
    else if(getstatus == FIELD_SQUARE_SMALL_BOAT)
    {
        own_field->grid[row][col] = FIELD_SQUARE_HIT;
        own_field->smallBoatLives -= 1;
        if(own_field->smallBoatLives <= 0)
        {
            opp_guess->result = RESULT_SMALL_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        } 
    }
    else if(getstatus == FIELD_SQUARE_MEDIUM_BOAT)
    {
        own_field->grid[row][col] = FIELD_SQUARE_HIT;
        own_field->mediumBoatLives -= 1;
        if(own_field->mediumBoatLives <= 0)
        {
            opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        } 
    }
    else if(getstatus == FIELD_SQUARE_LARGE_BOAT)
    {
        own_field->grid[row][col] = FIELD_SQUARE_HIT;
        own_field->largeBoatLives -= 1;
        if(own_field->largeBoatLives <= 0)
        {
            opp_guess->result = RESULT_LARGE_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        } 
    }
    else if(getstatus == FIELD_SQUARE_HUGE_BOAT)
    {
        own_field->grid[row][col] = FIELD_SQUARE_HIT;
        own_field->hugeBoatLives -= 1;
        if(own_field->hugeBoatLives <= 0)
        {
            opp_guess->result = RESULT_HUGE_BOAT_SUNK;
        }
        else
        {
            opp_guess->result = RESULT_HIT;
        } 
    }
    else
    {
        own_field->grid[row][col] = FIELD_SQUARE_INVALID;
    }
    return getstatus;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess)
{
    //update FieldState opp_field->grid[own_guess->row][own_guess->col]
    //if hit, then update field with FIELD_SQUARE_HIT
    //if miss, display FIELD_SQUARE_EMPTY
    //if RESULT_*_BOAT_SUNK, then clear opp_field->*boatlives
    //opp_field->*boatlives = 0;
    
    uint8_t row = own_guess->row;
    uint8_t col = own_guess->col;
    SquareStatus getstatus = FieldGetSquareStatus(opp_field, row, col);
    
    if (own_guess->result == RESULT_HIT)
    {
        opp_field->grid[row][col] = FIELD_SQUARE_HIT;
    }
    else if(own_guess->result == RESULT_MISS)
    {
        opp_field->grid[row][col] = FIELD_SQUARE_EMPTY;
    }
    else if(own_guess->result == RESULT_SMALL_BOAT_SUNK)
    {
        opp_field->grid[row][col] = FIELD_SQUARE_HIT;
        opp_field->smallBoatLives = 0;
    }
    else if(own_guess->result == RESULT_MEDIUM_BOAT_SUNK)
    {
        opp_field->grid[row][col] = FIELD_SQUARE_HIT;
        opp_field->mediumBoatLives = 0;
    }
    else if(own_guess->result == RESULT_LARGE_BOAT_SUNK)
    {
        opp_field->grid[row][col] = FIELD_SQUARE_HIT;
        opp_field->largeBoatLives = 0;
    }
    else if(own_guess->result == RESULT_HUGE_BOAT_SUNK)
    {
        opp_field->grid[row][col] = FIELD_SQUARE_HIT;
        opp_field->hugeBoatLives = 0;
    }
    return getstatus;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f)
{
    //f[row][col]
    //check lives 
    //if f->*boatlives != 0;
    uint8_t state = 0x0;
    if (f->smallBoatLives > 0)
    {
        state = state | FIELD_BOAT_STATUS_SMALL;
    }
    if (f->mediumBoatLives > 0)
    {
        state = state | FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->largeBoatLives > 0)
    {
        state = state | FIELD_BOAT_STATUS_LARGE;
    }
    if (f->hugeBoatLives > 0)
    {
        state = state | FIELD_BOAT_STATUS_HUGE;
    }
    return state;
}


/**
 * This function is responsible for placing all four of the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 * */
 
uint8_t FieldAIPlaceAllBoats(Field *own_field)
{
    uint8_t hugePlaced = FALSE;
    uint8_t largePlaced = FALSE;
    uint8_t mediumPlaced = FALSE;
    uint8_t smallPlaced = FALSE;
    uint8_t dir = rand() & DIR;
    uint8_t row, col;
    
    while (hugePlaced == FALSE)
    {
        if (dir == FIELD_DIR_SOUTH)
        {
            row = 0;
            col = rand() % FIELD_COLS;
        }
        else 
        {
            row = rand() % FIELD_ROWS;
            col = rand() % FIELD_COLS - FIELD_BOAT_SIZE_HUGE + 1;
        }
 
        if (FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_HUGE) == SUCCESS)
        {
            printf("SUCCESS\n");
            hugePlaced = TRUE;
        }
        dir = dir ^ DIR;
    }
    
    while (largePlaced == FALSE)
    {
        if (dir == FIELD_DIR_SOUTH)
        {
            row = rand() % (FIELD_ROWS - FIELD_BOAT_SIZE_LARGE + 1);
            col = rand();
        } else 
        {
            row = rand() % FIELD_ROWS;
            col = rand() % FIELD_COLS - FIELD_BOAT_SIZE_LARGE + 1;
        }
        
        if (FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_LARGE) == SUCCESS)
        {
            printf("SUCCESS\n");
            largePlaced = TRUE;
        }
        dir = dir ^ DIR;
    }
    
    while (mediumPlaced == FALSE)
    {
        if (dir == FIELD_DIR_SOUTH)
        {
            row = rand() % (FIELD_ROWS - FIELD_BOAT_SIZE_MEDIUM + 1);
            col = rand();
        } 
        else 
        {
            row = rand() % FIELD_ROWS;
            col = rand() % FIELD_COLS - FIELD_BOAT_SIZE_MEDIUM + 1;
        }
        if (FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_MEDIUM) == SUCCESS)
        {
            printf("SUCCESS\n");
            mediumPlaced = TRUE;
        }
        dir = dir ^ DIR;
    }
    
    while (smallPlaced == FALSE)
    {
        if (dir == FIELD_DIR_SOUTH)
        {
            row = rand() % (FIELD_ROWS - FIELD_BOAT_SIZE_SMALL + 1);
            col = rand();
        } 
        else 
        {
            row = rand() % FIELD_ROWS;
            col = rand() % FIELD_COLS - FIELD_BOAT_SIZE_SMALL + 1;
        }
        if (FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_SMALL) == SUCCESS)
        {
            printf("SUCCESS\n");
            smallPlaced = TRUE;
        }
        dir = dir ^ DIR;
    }
    return SUCCESS;
}

 /* Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a square which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
         
GuessData FieldAIDecideGuess(const Field *opp_field)
 {
    static GuessData guess;
    int row = 0;
    int col = 0;
    uint8_t ourTurn = FALSE;
    
    while(ourTurn == FALSE)
    {
        row = rand() % FIELD_ROWS;
        col = rand() % FIELD_COLS;
        //if opponent misses it becomes our turn
        if (opp_field->grid[row][col] == FIELD_SQUARE_UNKNOWN)
        {
            ourTurn = TRUE;
        }
    }
    guess.col = col;
    guess.row = row;
    return guess;
 }
    

/** 
 * For Extra Credit:  Make the two "AI" functions above 
 * smart enough to beat our AI in more than 55% of games.
**/ 