
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "LinkedList.h"


// **** Declare any function prototypes here ***
ListItem *CreateUnsortedList(void); //this function has been written for you
ListItem *SelectionSort(ListItem* list);
ListItem *InsertionSort(ListItem* list);

/* Students should not need to modify main! */
int main(void) {
    BOARD_Init();

    printf("\n\nWelcome to CRUZID's sort.c, compiled on %s %s.\n", __DATE__, __TIME__);

    ListItem* listToSort;

    //Do a timing test 
    printf("\nStarting stopwatch for selection sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    listToSort = SelectionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);


    printf("\nStarting stopwatch for insertion sort...\n");
    listToSort = CreateUnsortedList();
    __asm("nop"); //put a breakpoint on this line
    listToSort = InsertionSort(listToSort);
    __asm("nop"); //put a breakpoint on this line, and record the stopwatch time
    LinkedListPrint(listToSort);

    BOARD_End();
    while (1);
    return SUCCESS;
}

/**
 * SelectionSort() performs a selection sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *SelectionSort(ListItem* list) {
    //YOUR CODE GOES HERE!
    ListItem *FU = list;
    ListItem *tail = LinkedListGetLast(list);
    while (FU != tail) {
        ListItem *S = FU->nextItem;
        while (S != NULL) {
            int cmp = strcmp(FU->data, S->data);
            if (cmp > 0) {
                LinkedListSwapData(FU, S);
            }
            S = S->nextItem;
        }
        FU = FU->nextItem;
    }
    return LinkedListGetFirst(list);
}

/**
 * InsertionSort() performs an insertion sort on a passed to it.  The list is sorted alphabetically,
 * as determined by string.h's strcmp().  
 *
 * @param list Any element in a linked list to sort.  
 * @return a pointer to the head of the sorted list.
 * 
 * SelectionSort() assumes that no list items have a NULL data member.  It also assumes that 'list'
 * is a valid, non-NULL pointer to a linked list.
 * 
 * This function does not print.
 */
ListItem *InsertionSort(ListItem* list) {
    //YOUR CODE GOES HERE!
    ListItem *FS = LinkedListGetLast(list);
    while (FS->previousItem != NULL) {
        ListItem *LU = FS->previousItem;
        int cmp = strcmp(LU->data, FS->data);
        if (cmp < 0) {
            FS = LU;
        } else {
            ListItem *S = FS;
            while (S->nextItem != NULL) {
                int cmp2 = strcmp(S->nextItem->data, LU->data);
                if (cmp2 > 0) {
                    break;
                } else {
                    S = S->nextItem;
                }
            }
            LinkedListCreateAfter(S, LinkedListRemove(LU));
        }
    }
    return LinkedListGetFirst(list);
}

/* CreateUnsortedList() uses the functions in the LinkedList library to
 * generate an unsorted list.  
 * 
 * Students should not modify this function!
 * If an error occurs in this function, there is almost certainly 
 * a bug in your implementation of LinkedList.h.
 *   
 * @return A pointer to a ListItem which is the head of a new unsorted list. 
 */
ListItem *CreateUnsortedList(void)
/// <editor-fold defaultstate="collapsed" desc="CreateUnsortedList definition">
{
    char* wordList[] = {
        "decide", "toothpaste", "lowly", "robin", "reign", "crowd",
        "pies", "reduce", "tendency", "surround", "finger", "rake",
        "alleged", "hug", "nest", "punishment", "eggnog", "side",
        "beef", "exuberant", "sort", "scream", "zip", "hair",
        "ragged", "damage", "thought", "jump", "frequent", "substance",
        "head", "step", "faithful", "sidewalk", "pig", "raspy",
        "juggle", "shut", "maddening", "rock", "telephone", "selective",
        NULL
    };
    //  char* wordList[] = {"D", "A", "C", "E", "B", NULL};


    int i = 0;
    ListItem* head = LinkedListNew(wordList[i]);
    ListItem* tail = head;
    for (i = 1; wordList[i] != NULL; i++) {
        tail = LinkedListCreateAfter(tail, wordList[i]);
        if (tail == NULL) {
            printf("ERROR:  Heap full! Please increase heap size.\n");
            FATAL_ERROR();
        }
    }
    return head;
}
// </editor-fold>
