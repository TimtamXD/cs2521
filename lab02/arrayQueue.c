#include <stdlib.h>
#include <stdio.h>
#include "Item.h"
#include "Queue.h"
#include <assert.h>

#define DEFAULT_SIZE 10

struct queueImp{
    Item *items;
    int size;
    int maxSize;
};

int start = 0;
int end = 0;

//O(1)
Queue createQueue(){
    Queue q = malloc(sizeof(struct queueImp));
    q->items = malloc(DEFAULT_SIZE * sizeof(Item));
    q->size =0;
    q->maxSize = DEFAULT_SIZE;
    return q;
}

void destroyQueue(Queue q){
    assert(q != NULL);
    start = 0;
    end = 0;
    free(q->items);
    free(q);
}

//O(1)
int queueSize(Queue q){
    assert(q != NULL);
    return (q->size);
}

//O(1);
void putQueue(Queue q, Item i){
    assert(q != NULL);
    if (q->size == DEFAULT_SIZE) {
      fprintf(stderr,"queue overflow\n");
      abort();
    }
    assert(q->size < q->maxSize);
    q->items[q->size++]  = i;
}

//O(1)
// TODO
Item getQueue(Queue q){
    assert(q != NULL);
    end = start + q->size - 1;
    if (q->size > 0){
        Item item = q->items[start];
        q->size--;
        start++;
        end = start + q->size - 1;
      return item;
    } else {
        fprintf(stderr,"queue underflow\n");
        abort();
    }
}

void queueWhiteBoxTests(void){
    printf("=== WHITE BOX TESTS ===\n");
    printf("Test 1: Creating Queue & Adding Queue Items & Checking Queue Items\n");
    Queue k = createQueue();
    putQueue(k,1);
    assert(k->items[0] == 1);
    putQueue(k,2);
    assert(k->items[0] == 1);
    assert(k->items[1]== 2);
    putQueue(k,3);
    assert(k->items[0] == 1);
    assert(k->items[1] == 2);
    assert(k->items[2] == 3);
    putQueue(k,4);
    assert(k->items[0] == 1);
    assert(k->items[1] == 2);
    assert(k->items[2] == 3);
    assert(k->items[3] == 4);
    printf("Passed\n");
    printf("Test 2: Correct Queue Size\n");
    assert(k->size == 4);
    printf("Passed\n");
    printf("Test 3: Getting Correct Item From Queue & Check Queue Size & Queue Item\n");
    Item a = getQueue(k);
    assert(a == 1);
    assert(k->items[start] == 2);
    printf("The start is %d and the end is %d\n",start,end);
    assert(k->items[end] == 4);
    assert(k->size == 3);
    printf("Passed\n");
    printf("Destroying Queue...\n");
    destroyQueue(k);
    printf("=== WHITE BOX TESTS ALL PASSED ===\n");
}
