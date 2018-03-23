#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Item.h"
#include "Stack.h"

//This function should contain
//tests that have access to the internal structure
//of the stack. It should be implemented in arrayStack.c
void whiteBoxTests(void);


int main (int argc, char *argv[]){
    Stack s;
    //Run white box tests
    whiteBoxTests();

    //Run black box tests
    //Black box tests should test the ADT through the interface only
    printf("Black Box tests:\n");
    printf("This is just a small example for you to get started\n");

    printf("Test 1: CreateStack\n");
    s = createStack();
    assert(s != NULL);
    assert(stackSize(s) == 0);
    printf("Passed\n");

    printf("Test 2: Double Stack Size\n");
    push (s,1); // 1
    push (s,1); // 2
    push (s,1); // 3
    push (s,1); // 4
    push (s,1); // 5
    push (s,1); // 6
    push (s,1); // 7
    push (s,1); // 8
    push (s,1); // 9
    push (s,1); // 10
    push (s,1); // 11
    assert (stackSize(s) == 11);
    printf ("Passed\n");

    printf("Test 3: 1/4 Stack Size\n");
    push (s,1); // 12
    push (s,1); // 13
    push (s,1); // 14
    push (s,1); // 15
    push (s,1); // 16
    push (s,1); // 17
    push (s,1); // 18
    push (s,1); // 19
    push (s,1); // 20
    push (s,1); // 21
    assert (stackSize(s) == 21);
    push (s,1); // 22
    push (s,1); // 23
    push (s,1); // 24
    push (s,1); // 25
    push (s,1); // 26
    push (s,1); // 27
    push (s,1); // 28
    push (s,1); // 29
    push (s,1); // 30
    push (s,1); // 31
    push (s,1); // 32
    push (s,1); // 33
    push (s,1); // 34
    push (s,1); // 35
    push (s,1); // 36
    push (s,1); // 37
    push (s,1); // 38
    push (s,1); // 39
    push (s,1); // 40
    pop(s); // 39
    pop(s); // 38
    pop(s); // 37
    pop(s); // 36
    pop(s); // 35
    pop(s); // 34
    pop(s); // 33
    pop(s); // 32
    pop(s); // 31
    pop(s); // 30
    pop(s); // 29
    pop(s); // 28
    pop(s); // 27
    pop(s); // 26
    pop(s); // 25
    pop(s); // 24
    pop(s); // 23
    pop(s); // 22
    pop(s); // 21
    pop(s); // 20
    pop(s); // 19
    pop(s); // 18
    pop(s); // 17
    pop(s); // 16
    pop(s); // 15
    pop(s); // 14
    pop(s); // 13
    pop(s); // 12
    pop(s); // 11
    pop(s); // 10
    pop(s); // 9
    assert(stackSize(s) == 9);
    printf("Passed\n");

    printf("Test 3: 1/4 Stack Size below original size\n");
    pop(s); // 8
    pop(s); // 7
    pop(s); // 6
    pop(s); // 5
    pop(s); // 4
    pop(s); // 3
    pop(s); // 2
    pop(s); // 1
    assert(stackSize(s) == 1);
    printf("Passed\n");
    destroyStack(s);
    /*
    printf("Test 4: Last pop - passes if aborts!\n");
    pop(s); // 0
    pop(s); // underflow
    */
    return 0;
}

