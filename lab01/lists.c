#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lists.h"

void printList (link list) {
    link curr = list;
    while (curr != NULL) {
        printf ("[%d]->", curr->item);
        curr = curr->next;
    }
    printf ("[X]\n");
}


void printDList (dlink list) {
    dlink curr = list;
    while (curr != NULL) {
        printf ("[%d]->", curr->item);
        curr = curr->next;
    }
    printf ("[X]\n");
}


// Create a new node, initialised with the item provided. Return
// pointer to node (link)
link newNode (Item item) {
    //todo
    link new = malloc(sizeof(struct node)); // BUG
    new->item = item;
    new->next = NULL;
    return new;
}

// Insert a new node into a given non-empty list
// The node is inserted directly after the head of the list ls
void insertNext (link ls, link node) {
    //todo
    if (ls == NULL) {
      printf("Error, was given an empty list.\n");
    } else {
      link head = ls;
      link curr = ls->next;
      head->next = node;
      node->next = curr;
    }
}

// return the sum of all items in list
int sumListItems (link ls) {
    //todo
    // will keep on going for circular list, loops forever
    int sum  = 0;
    link curr = ls;
    while (curr != NULL) {
      sum = curr->item + sum;
      curr = curr->next;
    }
    return sum;
}

//frees all memory used in the list
void freeList (link list) {
    // todo
    if (list != NULL) {
      if (list->next == NULL) {
        free(list);
      } else {
        link head = list->next;
        link temp = NULL;
        while (head != NULL && head != list) {
          // printf("curr->next = %p, curr->value = %d\n", head->next, head->item);
          temp = head;
          head = head->next;
          free(temp);
        }
        free(list); // TODO
      }
    }
}

// create a circular list with the number of nodes specified with
// each link storing data from 1 to the number of nodes specified
link createCircularList (int numNodes) {
    //todo
    link head;
    if (numNodes == 0) {
      head = NULL;
    } else {
      link new = newNode(1);
      head = new;
      if (numNodes == 1) {
        new->next = new;
      } else if (numNodes > 1) {
        for (int i = 2; i < numNodes + 1; i++) {
          new->next = newNode(i);
          new = new->next;
        }
        new->next = head;
      }
    }
    return head;
}

// print the data in a circular fashion starting from any node
void printCircularList (link list) {
    //todo
    if (list == NULL) {
      printf ("[X]\n");
    } else {
      link curr = list;
      link start = list;
      printf ("[%d]->", curr->item);
      if (curr->next != NULL) {
        curr = curr->next;
        while (curr != start) {
          printf ("[%d]->", curr->item);
          curr = curr->next;
        }
      }
      printf ("[X]\n");
    }
}



static dlink newdNode() {
    dlink new = calloc(1, sizeof(struct dnode));
    new->next = NULL;
    //new->item = item;
    new->prev = NULL;
    return new;
}

// create a double-linked list which has contains the same elements,
// in the same order as 'list'

dlink doublify (link list) {
  // TODO
  // memory problems
  assert(list != NULL);
  dlink curr;
  dlink prev = NULL;
  dlink newList = NULL;
  while (list != NULL) {
    curr = newdNode();
    if (newList == NULL) {
      newList = curr;
    } else {
      curr->prev = prev;
      prev->next = curr;
    }
    prev = curr;
    curr->item = list->item;
    list = list->next;
  }
  return newList;
}

// frees all the memory used in the double-linked list
void freeDList (dlink list) {
    // TODO
    // memory problems
    assert(list != NULL);
    dlink head = list;
    dlink temp = list;
    while (head != NULL) {
      temp = head;
      head = head->next;
      free(temp);
    }
    // free(list); // SOMETHING WRONG
}


