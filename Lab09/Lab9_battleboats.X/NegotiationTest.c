// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//CMPE13 Support Library
#include "BOARD.h"

// Battleboats Libraries:
#include "BattleBoats.h"
#include "Negotiation.h"

#define T1 3
#define R1 9

#define T2 12345
#define R2 43182


int main(void) {
    
    printf("\n##### Beginning Dyu40 and Acompala's Negotiations test harness: ####\n\n");
    
    printf("NegotiationHash Test 1: Can 3 hash to 9\n");
    printf("NegotiationHash Test: ");
    if (NegotiationHash(T1) == R1) 
    {
        printf("Test 1: SUCCESS\n");
    } 
    else 
    {
        printf("Test 1: FAILURE\n");
    }
    
    printf("NegotiationHash Test 2: Can 12345 hash to 43182\n");
    if (NegotiationHash(T2) == R2) 
    {
        printf("Test 2: SUCCESS\n");
    } 
    else 
    {
        printf("Test 2: FAILURE\n");
    }
    
    
    printf("\nTesting NegotiationVerify()\n");
    printf("Test 3: Comparing 3 to 9");
    if (NegotiationVerify(T1, R1) == TRUE) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("Test 4: Comparing 12345 hashes to 43182\n");
    if (NegotiationVerify(T2, R2) == TRUE) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
        
    printf("Test 5: 3 should not hash to 0:\n ");
    if (NegotiationVerify(T1, 0) == FALSE) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    
    printf("Test 6: 12345 should not hash to 0:\n ");
    if (NegotiationVerify(T2, 0) == FALSE) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }

    
    printf("\nTesting NegotiateCoinFlip()\n");
    printf("Test 7 (11 with 11 -> TAILS):");
    if (NegotiateCoinFlip(11, 11) == TAILS) 
    {
        printf("SUCCESS\n");
    } 
    else 
    {
        printf("FAILURE\n");
    }
    printf("Test 8 (11 & 3 == HEADS): ");
    if (NegotiationVerify(11, 3) == FALSE) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
    
    return (EXIT_SUCCESS);
}
