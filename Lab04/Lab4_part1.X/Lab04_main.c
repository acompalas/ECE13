// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main() {
    BOARD_Init();

    //What follows is starter code.  You will need to modify it!

    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    double result;
    int error;

    printf("Welcome to CRUZID'S RPN calculator.  Compiled on %s %s", __DATE__, __TIME__);
    while (1) {

        printf("\nEnter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);
        if(strlen(rpn_sentence) > MAX_INPUT_LENGTH){
            printf("ERROR: No more room on stack.");
        }else{
            error = RPN_Evaluate(rpn_sentence, &result);
            if (error == 3){
                printf("ERROR: Invalid character in RPN string.");
            }else if(error == 1){
                printf("ERROR: RPN_ERROR_STACK_OVERFLOW ");
            }else if(error == 2){
                printf("ERROR: RPN_ERROR_STACK_UNDERFLOW ");
            }else if(error == 4){
                printf("ERROR: Invalid RPN calculation: divide by zero error. ");
            }else if(error == 5){
                printf("ERROR: RPN_ERROR_TOO_FEW_ITEMS_REMAIN ");
            }else if(error == 6){
                printf("ERROR: RPN_ERROR_TOO_MANY_ITEMS_REMAIN ");
            }else{
                printf("result = %f\n", result);
            }
        }
    }

    while (1);
}
