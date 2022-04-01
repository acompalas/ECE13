/* 
 * Every file you submit should start with a header like this
 * 
 * File:   part0.c
 * Author: Anderson Compalas (acompala@ucsc.edu)
 *
 * Created on April 1, 2022, 12:03 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"

/*
 * 
 */
int main(void)
{
    BOARD_Init();
    printf("Hello World\n");
    
    
    BOARD_End(); // handles cleanup of the system
    while(1); // if embedded we should never exit.
}

