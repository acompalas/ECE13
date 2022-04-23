// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"
#include "stack.h"

int main()
{
    BOARD_Init();
    int error;

    printf("\n###### Beginning CRUZID's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    
    char test0[] = "5 1 +";
    double result0;
    double expected0 = 6;
    printf("Testing RPN_Evaluate with \"%s\"... \n", test0);
    error = RPN_Evaluate(test0, &result0);
    printf("The result of addition is %f\n", result0);
    printf("Error type: %d\n", error);
    if (error) {
        printf("Failed, RPN_Evaluate produced an error\n");
    } else if (result0 != expected0) {
        printf("Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("Success!\n");
    }
    
    char test1[] = "5 1 -";
    double result1;
    double expected1 = -4;
    printf("\nTesting RPN_Evaluate with \"%s\"... \n", test1);
    error = RPN_Evaluate(test1, &result1);
    printf("The result of subtraction is %f\n", result1);
    printf("Error type: %d\n", error);
    if (error) {
        printf("Failed, RPN_Evaluate produced an error\n");
    } else if (result1 != expected1) {
        printf("Failed, expected = %f , result = %f\n", expected1, result1);
    } else {
        printf("Success!\n");
    }
    
    char test2[] = "2 5 *";
    double result2;
    double expected2 = 10;
    printf("\nTesting RPN_Evaluate with \"%s\"... \n", test2);
    error = RPN_Evaluate(test2, &result2);
    printf("The result of multiplication is %f\n", result2);
    printf("Error type: %d\n", error);
    if (error) {
        printf("Failed, RPN_Evaluate produced an error\n");
    } else if (result2 != expected2) {
        printf("Failed, expected = %f , result = %f\n", expected2, result2);
    } else {
        printf("Success!\n");
    }
    
    char test3[] = "4 9 /";
    double result3;
    double expected3 = 2.25;
    printf("\nTesting RPN_Evaluate with \"%s\"... \n", test3);
    error = RPN_Evaluate(test3, &result3);
    printf("The result of division is %f\n", result3);
    printf("Error type: %d\n", error);
    if (error) {
        printf("Failed, RPN_Evaluate produced an error\n");
    } else if (result3 != expected3) {
        printf("Failed, expected = %f , result = %f\n", expected3, result3);
    } else {
        printf("Success!\n");
    }
    
    char test4[] = "0 9 /";
    double result4;
    double expected4 = INFINITY;
    printf("\nTesting RPN_Evaluate with \"%s\"... \n", test4);
    error = RPN_Evaluate(test4, &result4);
    printf("The result of division is %f\n", result4);
    printf("Error type: %d\n", error);
    if (error == 4) {
        printf("Success! RPN_Evaluate successfully detected RPN_ERROR_DIVIDE_BY_ZERO error!\n");
    } else if (result4 != expected4) {
        printf("Failed, expected = %f , result = %f\n", expected4, result4);
    } else {
        printf("Failed, RPN_Evaluate failed to detect divide by zero error.\n");
    }
    
    char test5[] = "1 4 + 6 4 - *";
    double result5;
    double expected5 = 10;
    printf("\nTesting RPN_Evaluate with \"%s\"... \n", test5);
    error = RPN_Evaluate(test5, &result5);
    printf("The result is %f\n", result5);
    printf("Error type: %d\n", error);
    if (error) {
        printf("Failed, RPN_Evaluate produced an error\n");
    } else if (result5 != expected5) {
        printf("Failed, expected = %f , result = %f\n", expected5, result5);
    } else {
        printf("Success!\n");
    }
    
    char test6[] = "1 3";
    double result6;
    double expected6 = 1;
    printf("\nTesting RPN_Evaluate with \"%s\"... \n", test6);
    error = RPN_Evaluate(test6, &result6);
    printf("The result is %f\n", result6);
    printf("Error type: %d\n", error);
    if (error == 6) {
        printf("Success! RPN_Evaluate successfully detected RPN_ERROR_TOO_MANY_ITEMS_REMAIN error!\n");
    } else if (result6 != expected6) {
        printf("Failed, expected = %f , result = %f\n", expected6, result6);
    } else {
        printf("Success!\n");
    }
    
    char test7[] = "a b +";
    double result7;
    double expected7 = 1;
    printf("\nTesting RPN_Evaluate with \"%s\"... \n", test7);
    error = RPN_Evaluate(test7, &result7);
    printf("The result is %f\n", result7);
    printf("Error type: %d\n", error);
    if (error == 3) {
        printf("Success! RPN_Evaluate successfully detected RPN_ERROR_INVALID_TOKEN error!\n");
    } else if (result7 != expected7) {
        printf("Failed, expected = %f , result = %f\n", expected7, result7);
    } else {
        printf("Success!\n");
    }
    

    printf("\nTesting ProcessBackspaces:\n");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 1234\n");
    BOARD_End();
    while (1);
}


