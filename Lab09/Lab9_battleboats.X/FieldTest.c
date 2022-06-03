// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

//CE13 standard libraries:
#include "Buttons.h"
#include "Uart1.h"
#include "Oled.h"
#include "BattleBoats.h"
#include "Field.h"


/**
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
 */


#define NUM_AI_SHOTS 10

static Field ownField, oppField, aiField, aiGuessField;
static GuessData ownGuess, oppGuess, aiGuess;

#define OPP_GUESS_ROW_COL(r, c) { \
    oppGuess.row = r; \
    oppGuess.col = c; \
}

#define OWN_GUESS_ROW_COL(r, c, guess) { \
    ownGuess.row = r; \
    ownGuess.col = c; \
    ownGuess.result = guess; \
}


int main() {
    BOARD_Init();
    
    printf("\n Running tests for Field.c:\n\n");
    
    FieldInit(&ownField, &oppField);
    
     /**    * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ . . . . . . . . . . ]
 *  1 [ . . . . . . . . . . ]
 *  2 [ . . . . . . . . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 */
    
    printf("\nTesting FieldInit() and FieldGetStatus():\n");
    
    printf("Test 1 - If no boat then space should be empty: ");
    if (FieldGetSquareStatus(&ownField, 2, 4) == FIELD_SQUARE_EMPTY) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("Test 2 - Enemy field initialized to FIELD_SQUARE_UNKNOWN: ");
    if (FieldGetSquareStatus(&oppField, 2, 4) == FIELD_SQUARE_UNKNOWN) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    printf("Test 3 - Check for invalid guess outside bounds: ");
    if (FieldGetSquareStatus(&ownField, 7, 11) == FIELD_SQUARE_INVALID) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("\nTesting FieldSetSquareStatus():\n");
    
    printf("Test 4 - Test if old value is properly detected: ");
    if (FieldSetSquareStatus(&ownField, 0, 3, FIELD_SQUARE_SMALL_BOAT) == FIELD_SQUARE_EMPTY) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("Test 5 - Test if new value for square was set: ");
    if (FieldGetSquareStatus(&ownField, 0, 3) == FIELD_SQUARE_SMALL_BOAT) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("\nTesting FieldAddBoat()\n");
    FieldInit(&ownField, &oppField); // resets field to empty just in case
    FieldAddBoat(&ownField, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(&ownField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(&ownField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE);
    FieldPrint_UART(&ownField, &oppField);
 /**    * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 5 . . . ]
 *  1 [ 4 4 4 4 . . 5 . . . ]
 *  2 [ . . . . . . 5 . . . ]
 *  3 [ . . . . . . 5 . . . ]
 *  4 [ . . . . . . 5 . . . ]
 *  5 [ . . . . . . . . . . ]
 */

   
    printf("Test 6 - Return error is not enough space for boat: ");
    if (FieldAddBoat(&ownField, 2, 8, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) == STANDARD_ERROR) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("Test 7 - Test if boat is successfully placed: ");
    if (FieldAddBoat(&ownField, 0, 8, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_HUGE) == SUCCESS) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("Test 8 - Can't double stack boats: ");
    if (FieldAddBoat(&ownField, 1, 2, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) == STANDARD_ERROR) 
    {
        printf("SUCCESS\n");
    } else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("\nTesting FieldRegisterEnemyAttack()\n");
    
    printf("Test 9 - Miss: ");
    OPP_GUESS_ROW_COL(4, 3);
    FieldRegisterEnemyAttack(&ownField, &oppGuess);
    if (oppGuess.result == RESULT_MISS) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("Test 10 - Hit: ");
    OPP_GUESS_ROW_COL(0, 0);
    FieldRegisterEnemyAttack(&ownField, &oppGuess);
    if (oppGuess.result == RESULT_HIT) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("Test 11 - Small Sunk: ");
    OPP_GUESS_ROW_COL(0, 1);
    FieldRegisterEnemyAttack(&ownField, &oppGuess);
    OPP_GUESS_ROW_COL(0, 2);
    FieldRegisterEnemyAttack(&ownField, &oppGuess);
    if (oppGuess.result == RESULT_SMALL_BOAT_SUNK) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("\nTesting FieldUpdateKnowledge():\n");
    printf("Test 12 - Miss: ");
    OWN_GUESS_ROW_COL(4, 3, RESULT_MISS);
    FieldUpdateKnowledge(&oppField, &ownGuess);
    if (FieldGetSquareStatus(&oppField, ownGuess.row, ownGuess.col) == FIELD_SQUARE_EMPTY) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("Test 13 - Hit: ");
    OWN_GUESS_ROW_COL(2, 6, RESULT_HIT);
    FieldUpdateKnowledge(&oppField, &ownGuess);
    if (FieldGetSquareStatus(&oppField, ownGuess.row, ownGuess.col) == FIELD_SQUARE_HIT) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("Test 14 - Small Sunk: ");
    OWN_GUESS_ROW_COL(3, 6, RESULT_HIT);
    FieldUpdateKnowledge(&oppField, &ownGuess);
    OWN_GUESS_ROW_COL(4, 6, RESULT_HIT);
    FieldUpdateKnowledge(&oppField, &ownGuess);
    if (FieldGetSquareStatus(&oppField, ownGuess.row, ownGuess.col) == FIELD_SQUARE_HIT) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    FieldPrint_UART(&ownField, &oppField);
    printf("\nTesting FieldGetBoatStates():\n");
    printf("Test 15 - Test if own small updated: ");
    if ((FieldGetBoatStates(&ownField) & FIELD_BOAT_STATUS_SMALL) != FIELD_BOAT_STATUS_SMALL) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("Test 16 - Test if enemy small sunk updated: ");
    if ((FieldGetBoatStates(&ownField) & FIELD_BOAT_STATUS_SMALL) != FIELD_BOAT_STATUS_SMALL) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    
    FieldPrint_UART(&ownField, &oppField);
    
    printf("\nTest AI Board and Guesses:\n");
    FieldInit(&aiField, &aiGuessField);
    FieldAIPlaceAllBoats(&aiField);
    
    
    int i;
    for (i = 0; i < NUM_AI_SHOTS; i++)
    {
        aiGuess = FieldAIDecideGuess(&aiGuessField);
        FieldRegisterEnemyAttack(&ownField, &aiGuess);
        FieldUpdateKnowledge(&aiGuessField, &aiGuess);
    } 
    
    FieldPrint_UART(&aiField, &aiGuessField);
    
    BOARD_End();
    
    return (EXIT_SUCCESS);
}