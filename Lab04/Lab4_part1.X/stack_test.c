// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

int main()
{
    BOARD_Init();

    printf("\n###### Beginning CRUZID's stack test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!
    
    printf("\nDoes StackInit() set the currentItemIndex appropriately?\n");
    struct Stack stack = {};
    StackInit(&stack);
    if (stack.currentItemIndex == -1) {
        printf("passed StackInit!\n");
    } else {
        printf("failed StackInit");
    }
    
    printf("\nDoes StackPush() push the stackItems[stack.currentItemIndex] appropriately?\n");
    StackPush(&stack, 8);
    if (stack.stackItems[stack.currentItemIndex] == 8){
        printf("Passed StackPush!\n");
    }
    else{
        printf("Failed StackPush");
    }
    
    printf("\nDoes StackPop() pop the value appropriately?\n");
    double value;
    StackPop(&stack, &value);
    if (value == 8){
        printf("Passed StackPop!\n");
    }
    else{
        printf("Failed StackPop");
    }
    
    printf("\nDoes StackGetSize() get the right size of the stack?\n");
    int size = StackGetSize(&stack);
    if (size == 0){
        printf("Passed StackGetSize!\n");
    }
    else{
        printf("Failed StackGetSize");
    }
    
    
    
    

    
    
    
    BOARD_End();
    //test stackInit:
    while (1);
    return 0;
}


