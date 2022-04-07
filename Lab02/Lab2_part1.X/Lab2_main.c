// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

#ifdef PIC32
// Microchip libraries
#include <xc.h>
#endif

//prototype RunCalculator())
void CalculatorRun(void);

int main(void)
{
    BOARD_Init();
    
    CalculatorRun();

    printf(" Calculator has exited, must restart to perform more calculations\n ");

    BOARD_End();
    while(1);
}

