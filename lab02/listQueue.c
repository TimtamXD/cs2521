//A linked list implementation of a queue

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Item.h"
#include "Queue.h"

typedef struct queueNode * link;

struct queueNode {
    Item item;
    link next;
};

struct queueImp {
    link head;
    link tail;
    int size;
};

static link createNode(Item item){
    link n = malloc (sizeof(*n));
    assert(n != NULL);
    n->item = item;
    n->next = NULL;
    return n;
}

// Creates an empty Queue
Queue createQueue (void){
    Queue q = malloc (sizeof (*q));
    assert(q != NULL);
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void destroyQueue(Queue queue){
    link curr;
    link next;
    assert(queue != NULL);
    curr = queue->head;
    while(curr!=NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(queue);

}

int queueSize (Queue q){
    assert(q != NULL);
    return q->size;
}


void putQueue (Queue q, Item item){
    assert(q != NULL);
    link n = createNode(item);
    if(q->head != NULL){
       q->head->next = n;
    } else {
       q->tail = n;
    }
    q->head = n;
    q->size++;
}

Item getQueue (Queue q){
    assert(q != NULL);
    if (q->size > 0) {
      Item item = q->tail->item;
      link delNode = q->tail;
      q->tail = q->tail->next;
      free(delNode);
      q->size--;
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
    assert(k->head->item == 1);
    assert(k->tail->item == 1);
    putQueue(k,2);
    assert(k->tail->item == 1);
    assert(k->head->item == 2);
    putQueue(k,3);
    assert(k->head->item == 3);
    assert(k->tail->item == 1);
    putQueue(k,4);
    assert(k->head->item == 4);
    assert(k->tail->item == 1);
    printf("Passed\n");
    printf("Test 2: Correct Queue Size\n");
    assert(queueSize(k) == 4);
    printf("Passed\n");
    printf("Test 3: Getting Correct Item From Queue & Check Queue Size & Queue Item\n");
    Item a = getQueue(k);
    assert(a == 1);
    assert(k->tail->item == 2);
    assert(queueSize(k) == 3);
    printf("Passed\n");
    printf("Destroying Queue...\n");
    destroyQueue(k);
    printf("=== WHITE BOX TESTS ALL PASSED ===\n");
}

