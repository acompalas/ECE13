/* 
 * File:   LinkedListTest.c
 * Author: ajcom
 *
 * Created on April 26, 2022, 6:06 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include <string.h>

/*
 * 
 */

void main() {

    //Test LinkedListNew
    char *a = "A";
    ListItem *test = LinkedListNew(a);
    if (test == NULL) {
        printf("1.Error - NULL");
    } else {
        printf("1.Test LinkedListNew() Successful!: ");
        printf("\n - Created List: ");
        LinkedListPrint(test);
    }

    //Test LinkedListCreateAfter
    char *b = "B";
    char *c = "C";
    char *z = "Z";
    ListItem *item1 = LinkedListCreateAfter(test, b);
    ListItem *item2 = LinkedListCreateAfter(item1, c);
    ListItem *item3 = LinkedListCreateAfter(item2, z);
    if (test == NULL) {
        printf("\n2.Error - NULL");
    } else {
        printf("\n2.Test LinkedListCreateAfter() Successful! ");
        printf("\n- Items to add: %c, %c, %c", *b, *c, *z);
        printf("\n- New List: ");
        LinkedListPrint(test);
    }

    //Test LinkedListGetFirst
    ListItem *first = LinkedListGetFirst(item1);
    printf("\n3.Test LinkedListGetFirst() ");
    printf("\n -Test list: ");
    LinkedListPrint(test);
    printf("\n -Expected head: A");
    printf("\n -Resulting head: %s", first->data);

    //Test LinkedListGetLast
    ListItem *last = LinkedListGetLast(test);
    printf("\n4.Test LinkedListGetLast()");
    printf("\n -Test list: ");
    LinkedListPrint(test);
    printf("\n -Expected Tail: Z");
    printf("\n -Resulting Tail: %s\n", last->data);

    //Test LinkedListSize
    int expected_size = 4;
    int size = LinkedListSize(test);
    if (size == expected_size) {
        printf("5.Test LinkedListSize() - Success!\n");
        printf(" -Test List: ");
        LinkedListPrint(test);
        printf("\n -Expected size: %d\n", expected_size);
        printf(" -Resulting size: %d\n", size);
    } else {
        printf("5.LinkedListSize() - Failed!\n");
    }

    //Test LinkedListSwapData()
    printf("6.Test LinkedListSwapData()");
    printf("\n -Swap: %s and %s\n", test->data, item2->data);
    printf(" -List before swap: ");
    LinkedListPrint(test);
    LinkedListSwapData(test, item2);
    printf("\n -List after swap: ");
    LinkedListPrint(test);

    //Test LinkedListRemove
    printf("\n7.Test LinkedListRevmove()");
    printf("Item to remove: %s", item1->data);
    printf("\n -List before remove: ");
    LinkedListPrint(test);
    LinkedListRemove(item1);
    printf("\n -List after remove: ");
    LinkedListPrint(test);

    //Test LinkedListPrint()
    printf("\n8.Test LinkedListPrint()");
    printf("\n -Expected Print: [C, A, Z]");
    printf("\n -Actual Print: ");
    LinkedListPrint(test);
}

