#include "rpn.h"
#include <string.h>
#include "BOARD.h"
#include "stack.h"

/* RPN_Evaluate() parses and evaluates a string that contains 
 * a valid Reverse Polish Notation string (no newlines!)  
 * @param:  rpn_string - a string in polish notation.  Tokens must be either 
 *          arithmetic operators or numbers.
 * @param:  result - a pointer to a double that will be modified to contain
 *          the return value of the rpn expression.
 * @return: error - if the rpn expression is invalid, 
 *          an appropriate rpn_error value is returned.
 * 
 * RPN_Evaluate supports the following basic arithmetic operations:
 *   + : addition
 *   - : subtraction
 *   * : multiplication
 *   / : division
 * Numeric tokens can be positive or negative, and can be integers or 
 * decimal floats.  RPN_Evaluate should be able to handle strings of 
 * at least 255 length.
 * */
int RPN_Evaluate(char * rpn_string, double * result) {
    struct Stack stack = {};
    StackInit(&stack);
    double a;
    double b;
    char *token = NULL;
    token = strtok(rpn_string, "\n");
    token = strtok(rpn_string, " ");
    double number;
    while (token != NULL) {
        //filter ascii's
        for (int i = 0; i < strlen(token); i++) {
            if (token[i] >= 48 && token[i] <= 57) {
                number = atof(token);
                if (StackPush(&stack, number) == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_OVERFLOW;
                }
            } else if (strcmp(token, "+") == 0) {
                //pop, pop, calculate, push
                if (StackPop(&stack, &a) == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                if (StackPop(&stack, &b) == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                *result = b+a;
                if (StackGetSize(&stack) < STACK_SIZE) {
                    StackPush(&stack, *result);
                } else {
                    return RPN_ERROR_STACK_OVERFLOW;
                }
            } else if (strcmp(token, "-") == 0) {
                //pop, pop, calculate, push
                int pop1 = StackPop(&stack, &a);
                int pop2 = StackPop(&stack, &b);
                if (pop1 == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                if (pop2 == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                *result = a-b  ;
                if (StackGetSize(&stack) < STACK_SIZE) {
                    StackPush(&stack, *result);
                } else {
                    return RPN_ERROR_STACK_OVERFLOW;
                }
            } else if (strcmp(token, "*") == 0) {
                //pop, pop, calculate, push
                if (StackPop(&stack, &a) == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                if (StackPop(&stack, &b) == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                *result = b*a;
                if (StackGetSize(&stack) < STACK_SIZE) {
                    StackPush(&stack, *result);
                } else {
                    return RPN_ERROR_STACK_OVERFLOW;
                }
            } else if (strcmp(token, "/") == 0) {
                //pop, pop, calculate, push
                if (StackPop(&stack, &a) == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                if (StackPop(&stack, &b) == SUCCESS) {
                } else {
                    return RPN_ERROR_STACK_UNDERFLOW;
                }
                if (b == 0) {
                    return RPN_ERROR_DIVIDE_BY_ZERO;
                } else {
                    *result = a/b;
                    if (StackGetSize(&stack) < STACK_SIZE) {
                        StackPush(&stack, *result);
                    } else {
                        return RPN_ERROR_STACK_OVERFLOW;
                    }
                }
            } else {
                return RPN_ERROR_INVALID_TOKEN;
            }
        }
        token = strtok(NULL, " ");
    }
    if (StackGetSize(&stack) > 1) {
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    } else if (StackGetSize(&stack)== STANDARD_ERROR) {
        return RPN_ERROR_STACK_OVERFLOW;
    }
    return RPN_NO_ERROR;
}



/**
 * This function should read through an array of characters, checking for backspace characters. 
 * When it encounters a backspace character, it eliminates the backspace, the preceeding character
 * (if such a character exists), and shifts all subsequent characters as appropriate.
 * @param string_to_modify The string that will be processed for backspaces. 
 *        This string is modified "in place", so it is both an input and an output to the function.
 * @return Returns the size of the resulting string in "string_to_modify".
 *
 * ProcessBackspaces() should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings of at least 255 length.
 * 
 * */
int ProcessBackspaces(char *rpn_sentence){
    char *source, *dest;
    for (source = dest = rpn_sentence; *source != '\0'; source++){
        if (*source == '\b'){
            if (dest != rpn_sentence){
                dest--;
            }
            continue;
        }
        *dest++ = *source;
    }
    *dest = '\0';
    return 1;
}



