//An array based implementation of a stack

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Item.h"
#include "Stack.h"
#include "assert.h"

#define DEFAULT_SIZE 10

struct stackImp{
    Item * items;
    int top;
    int maxSize;
};

//A stack is created with a default initial size of 10
Stack createStack (void){
    Stack s = malloc(sizeof(struct stackImp));
    assert(s != NULL);
    s->items = malloc(DEFAULT_SIZE * sizeof(Item));
    assert(s->items != NULL);
    s->top = 0;
    s->maxSize = DEFAULT_SIZE;
    return s;
}

void destroyStack(Stack stack){
    assert(stack != NULL);
    free(stack->items);
    free(stack);
}

//You need to modify this
// TODO
/*
When an attempt is made to push an item onto a stack that is already full,
the array is resized to accomodate the extra element. Make sure the stack is
only resized when it is full and make the resized array be 2 times bigger.

Hint: Look at the C function realloc. (Note: this should ONLY be used to resize
memory that has been malloced. Thus if you had declared you array like: int
array[100]; You could NOT use realloc to resize it).s
*/
void push (Stack stack, Item item){
    assert(stack != NULL);
    if (stack->top == stack->maxSize) {
      stack->items = realloc(stack->items,2 * stack->maxSize * sizeof (Item));
      stack->maxSize = stack->maxSize * 2;
    }
    stack->items[stack->top] = item;
    stack->top++;
}

//You need to modify this
// TODO
/*
It prints the message "stack underflow" when an attempt is made to pop from an
empty stack and calls the function abort to terminate the program. Also when
the size of the stack drops below 1/4 of the size of the array, the array size
should be resized to half of its size. However the array size should never drop
below its original starting size.
*/
Item pop (Stack stack) {
    assert (stack != NULL);
    if (stack->top == 0) {
      printf("stack underflow\n");
      abort();
    }
    if (stack->top < (stack->maxSize / 4)) {
      int newsize = stack->maxSize / 2;
      if (newsize >= DEFAULT_SIZE) {
        stack->items = realloc(stack->items,2 * stack->maxSize * sizeof (Item));
        stack->maxSize = newsize;
      }
    }
    //int oldtop = stack->top;
    stack->top--;
    return stack->items[stack->top];
}

int stackSize(Stack stack){
    assert(stack != NULL);
    return stack->top;
}

//You need to write these
// TODO
void whiteBoxTests(void){
    printf ("=== WHITE BOX TESTS ===\n");
    Stack newStack = createStack();
    //printf ("Testing popping nothing:\n");
    //pop (newStack);
    printf ("Test 1: Testing popping\n");
    push (newStack, 1);
    push (newStack, 2);
    push (newStack, 3);
    push (newStack, 4);
    push (newStack, 5);
    printf("The current top is %d.\n", newStack->top);
    pop (newStack); // 4 3 2 1
    pop (newStack); // 3 2 1
    pop (newStack); // 2 1
    pop (newStack); // 1
    pop (newStack); // -
    printf("The current top is %d.\n", newStack->top);
    if (newStack->maxSize == DEFAULT_SIZE) {
      printf ("Test passed.\n");
    }
    printf ("Test 2: Testing doubling size of stack.\n");
    push (newStack, 1);
    push (newStack, 2);
    push (newStack, 3);
    push (newStack, 4);
    push (newStack, 5);
    push (newStack, 6);
    push (newStack, 7);
    push (newStack, 8);
    push (newStack, 9);
    push (newStack, 10);
    push (newStack, 11);
    if (newStack->maxSize == DEFAULT_SIZE * 2) { // size is 20
      printf("The current top 11 is %d.\n", newStack->top);
      printf("The current size of stack is %d.\n", newStack->maxSize);
      printf("Test passed.\n");
    }
    printf ("Test 3: Testing doubling size and halving size.\n");
    push (newStack, 12);
    push (newStack, 13);
    push (newStack, 14);
    push (newStack, 15);
    push (newStack, 16);
    push (newStack, 17);
    push (newStack, 18);
    push (newStack, 19);
    push (newStack, 20);
    push (newStack, 21);
    printf("The current size of stack is %d.\n", newStack->maxSize);
    printf("The current top 21 is %d.\n", newStack->top);
    assert (newStack->maxSize == 40);
    push (newStack, 22);
    push (newStack, 23);
    push (newStack, 24);
    push (newStack, 25);
    push (newStack, 26);
    push (newStack, 27);
    push (newStack, 28);
    push (newStack, 29);
    push (newStack, 30);
    push (newStack, 31);
    push (newStack, 32);
    push (newStack, 33);
    push (newStack, 34);
    push (newStack, 35);
    push (newStack, 36);
    push (newStack, 37);
    push (newStack, 38);
    push (newStack, 39);
    push (newStack, 40);
    push (newStack, 41);
    printf("The current size of stack is %d.\n", newStack->maxSize);
    //printf("The current top 41 is %d.\n", newStack->top);
    assert (newStack->maxSize == 80);
    push (newStack, 42);
    push (newStack, 43);
    push (newStack, 44);
    //printf("The current top 44 is %d.\n", newStack->top);
    pop (newStack); // 43
    //printf("The current top 43 is %d.\n", newStack->top);
    pop (newStack); // 42
    pop (newStack); // 41
    pop (newStack); // 40
    pop (newStack); // 39
    pop (newStack); // 38
    pop (newStack); // 37
    pop (newStack); // 36
    pop (newStack); // 35
    pop (newStack); // 34
    pop (newStack); // 33
    pop (newStack); // 32
    pop (newStack); // 31
    pop (newStack); // 30
    pop (newStack); // 29
    pop (newStack); // 28
    pop (newStack); // 27
    pop (newStack); // 26
    pop (newStack); // 25
    pop (newStack); // 24
    pop (newStack); // 23
    pop (newStack); // 22
    pop (newStack); // 21
    pop (newStack); // 20
    pop (newStack); // 19
    pop (newStack); // 18
    printf("The current top 18 is %d.\n", newStack->top);
    printf("The current size of stack is %d.\n", newStack->maxSize);
    assert (newStack->maxSize == 40);
    pop (newStack); // 17
    pop (newStack); // 16
    pop (newStack); // 15
    pop (newStack); // 14
    pop (newStack); // 13
    pop (newStack); // 12
    pop (newStack); // 11
    pop (newStack); // 10
    pop (newStack); // 9
    pop (newStack); // 8
    printf("The current top 8 is %d.\n", newStack->top);
    printf("The current size of stack is %d.\n", newStack->maxSize);
    assert (newStack->maxSize == 20);
    pop (newStack); // 7
    pop (newStack); // 6
    pop (newStack); // 5
    printf("The current top 5 is %d.\n", newStack->top);
    assert (newStack->maxSize == 20);
    pop (newStack); // 4
    pop (newStack); // 3
    printf("The current size of stack is %d.\n", newStack->maxSize);
    printf("The current top 3 is %d.\n", newStack->top);
    assert (newStack->maxSize == 10);
    pop (newStack); // 2
    printf("The current size of stack is %d.\n", newStack->maxSize);
    printf("The current top 2 is %d.\n", newStack->top);
    assert (newStack->maxSize == 10);
    pop (newStack); // 1
    printf("The current size of stack is %d.\n", newStack->maxSize);
    printf("The current top 1 is %d.\n", newStack->top);
    assert (newStack->maxSize == 10);
    printf("Test passed.\n");
    printf("Destroying stack...\n");
    destroyStack (newStack);
    printf("=== WHITE BOX TESTS ALL PASSED ===\n");
}

