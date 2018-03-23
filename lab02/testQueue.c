#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Item.h"
#include "Queue.h"

void queueWhiteBoxTests(void);
int main (int argc, char *argv[]){
    //Run white box tests
    queueWhiteBoxTests();

    //Run black box tests
    printf("Black Box tests:\n");

    printf("Test 1: Queue Creation\n");
    Queue q = createQueue();
    assert (q != NULL);
    assert (queueSize(q) == 0);
    printf("Passed\n");

    printf("Test 2: Placing Things In Queue\n");
    putQueue(q, 1);
    putQueue(q, 2);
    putQueue(q, 3);
    putQueue(q, 4);
    putQueue(q, 5);
    putQueue(q, 6);
    putQueue(q, 7);
    assert(queueSize(q) == 7);
    printf("Passed\n");

    printf("Test 3: Getting Item from Queue\n");
    Item n = getQueue(q);
    assert(n == 1);
    n = getQueue(q);
    assert(n == 2);
    n = getQueue(q);
    assert(n == 3);
    n = getQueue(q);
    assert(n == 4);
    n = getQueue(q);
    assert(n == 5);
    n = getQueue(q);
    assert(n == 6);
    printf("Passed\n");

    printf("Test 4: Destroy Queue\n");
    destroyQueue(q);
    printf("Passed\n");

    printf("Test 5: ABORT!\n");
    Queue r = createQueue();
    n = getQueue(r);

    return 0;
}

