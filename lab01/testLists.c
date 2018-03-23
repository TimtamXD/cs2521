#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lists.h"

// insert proper tests here
int main (int argc, const char * argv[]) {

    //creates an  empty list  and then prints it
    link list = NULL;
    printList(list);
    printf("The sum is %d\n", sumListItems(list));
    // tests newNode and insertNext
    // Exercise 1
    link newOne = newNode(1); // BUG
    printList(newOne);
    printf("The sum is %d\n", sumListItems(newOne));
    link newTwo = newNode(2);
    insertNext(newOne, newTwo);
    printList(newOne);
    printf("The sum is %d\n", sumListItems(newOne));
    link newThree = newNode(3);
    insertNext(newTwo, newThree);
    printList(newOne);
    printf("The sum is %d\n", sumListItems(newOne));
    printf("You passed Exercise 1!\n");
    // Exercise 2
    freeList(list);
    freeList(newOne); // TODO
    printf("You passed Exercise 2 if valgrind is good!\n");
    // Exercise 3
    link newCircular = createCircularList(0);
    printCircularList (newCircular);
    link newCircularOne = createCircularList(1);
    printCircularList (newCircularOne);
    link newCircularTwo = createCircularList(4);
    printCircularList (newCircularTwo);
    freeList(newCircular);
    freeList(newCircularOne);
    freeList(newCircularTwo);
    printf("You passed Exercise 3 if valgrind if good!\n");
    // Exercise 4
    link NewOne = newNode(1);
    link NewTwo = newNode(2);
    insertNext(NewOne, NewTwo);
    link NewThree = newNode(3);
    insertNext(NewTwo, NewThree);
    dlink newDlink = doublify(NewOne);
    free(NewOne);
    free(NewTwo);
    free(NewThree);
    printDList(newDlink);
    freeDList (newDlink);
    printf("You passed Exercise 4 if valgrind if good!\n");
    
    return 0;
}

