#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "textbuffer.h"

/* TB points to textNodes which will contain the line
 * and position
 */

typedef struct textNode * link;

struct textNode {
  char *text;
  link next;
  link prev;
};

struct textbuffer {
  link head;
  link tail;
  int lines;
};

/* This extra function createNode is to create and append
 * the Nodes that will form part of TB with
 * with consideration to the text being placed in each node
 */

static link createNode (char text[]) {
  assert (text != NULL);
  link new = malloc (sizeof (* new));
  assert (new != NULL);
  int textlen = strlen (text) + 1;
  new->text = (char *) malloc (textlen * sizeof (char));
  strcpy (new->text, text);
  new->next = NULL;
  new->prev = NULL;
  return new;
}

/* This extra function deleteNodes will help to
 * eliminate the nodes in the function deleteTB
 */

static void deleteNodes (link p) {
  assert (p != NULL);
  link temp = p;
  while (p != NULL) {
    temp = p;
    p = p->next;
    free (temp);
  }
}

/* This extra function copyTB will return a copy
 * of a tb for mergeTB, pasteTB
 */

static TB cloneTB (TB tb) {
  char *str = dumpTB (tb);
  TB copiedTB = newTB (str);
  return copiedTB;
}

/* These three following functions will attach
 * nodes to either the front, middle or end of
 * an existing tb. It will be used for mergeTB
 * and pasteTB.
 */

static void stitchToStart (TB tb1, TB tb2) {
  assert (tb1 != NULL && tb2 != NULL);
  if (tb1->head != NULL && tb2->head != NULL) {
    tb2->tail->next = tb1->head;
    tb1->head->prev = tb2->tail;
    tb1->head = tb2->head;
  } else if (tb1->head == NULL && tb2->head != NULL) {
    tb1->head = tb2->head;
    tb2->tail = tb2->tail;
  }
}

static void stitchToMid (TB tb1, TB tb2, int pos) {
  assert (tb1 != NULL && tb2 != NULL);
  assert (pos != 0 && pos != tb1->lines - 1);
  int i = 0;
  if (tb2->lines != 0) {
    link temp = tb1->head;
    link temp2 = tb1->head;
    for (i = 0; i < pos - 1; i++) {
      temp = temp->next;
    }
    for (i = 0; i < pos; i++) {
      temp2 = temp2->next;
    }
    temp->next = tb2->head;
    tb2->head->prev = temp;
    tb2->tail->next = temp2;
    temp2->prev = tb2->tail;
  }
}

static void stitchToEnd (TB tb1, TB tb2) {
  assert (tb1 != NULL && tb2 != NULL);
  if (tb1->head != NULL && tb2->head != NULL) {
    tb1->tail->next = tb2->head;
    tb2->head->prev = tb1->tail;
    tb1->tail = tb2->tail;
  } else if (tb1->head == NULL && tb2->head != NULL) {
    tb1->head = tb2->head;
    tb2->tail = tb2->tail;
  }
}

/* Text is used to extract text from a pointer
 * to the nodes in the ADT, used in cutTB.
 */

static char *extractText (link p) {
  assert (p != NULL);
  link curr = p;
  int mallocsize = 0;
  while (curr != NULL) {
    // mallocsize = string length + 1 (for new line character)
    mallocsize = mallocsize + strlen (curr->text) + 1;
    curr = curr->next;
  }
  mallocsize = mallocsize + 1; // add another for NULL character
  curr = p;
  char *returnString = (char *) malloc (mallocsize * sizeof(char));
  if (curr->text != NULL) {
    memcpy (returnString, curr->text, strlen(curr->text) + 1);
    strcat (returnString, "\n");
    curr = curr->next;
  }
  while (curr != NULL) {
    strcat (returnString, curr->text);
    strcat (returnString, "\n");
    curr = curr->next;
  }
  strcat (returnString, "\0");
  return returnString;
}

/* The following functions are the original required
 * functions from the assignment
 */

TB newTB (char text[]) {
  TB newTextB = malloc (sizeof (*newTextB));
  assert (newTextB != NULL);
  newTextB->head = NULL;
  newTextB->tail = NULL;
  newTextB->lines = 0;
  link prev = NULL;
  char *copyOfText, *lineOfText, *freeableCopy;
  freeableCopy = copyOfText = strdup (text);
  while ((lineOfText = strsep (&copyOfText, "\n")) != NULL) {
    if (strcmp (lineOfText, "\0")) {
      link newNode = createNode (lineOfText);
      if (newTextB->head == NULL) {
        newTextB->head = newNode;
        newTextB->tail = newNode;
        prev = newNode;
      } else {
        newTextB->tail = newNode;
        prev->next = newNode;
        newNode->prev = prev;
        prev = prev->next;
      }
      newTextB->lines++;
    }
  }
  free (freeableCopy);
  // printf ("=== newTB function is good! ===\n");
  return newTextB;
}

void releaseTB (TB tb) {
  assert (tb != NULL);
  link curr;
  link next;
  curr = tb->head;
  if (tb->lines != 0) {
    while (curr != NULL) {
    next = curr->next;
    free (curr->text);
    free (curr);
    curr = next;
    }
  }
  free (tb);
  // printf ("=== release function is good! ===\n");
}

char *dumpTB (TB tb) {
  assert (tb != NULL);
  link curr = tb->head;
  char *returnString = (char *) malloc (10000 * sizeof (char));
  memset (returnString, 0, strlen (returnString));
  if (tb->lines > 0) {
    memcpy (returnString, curr->text, strlen(curr->text) + 1);
    strcat (returnString, "\n");
    curr = curr->next;
    while (curr != NULL) {
      strcat (returnString, curr->text);
      strcat (returnString, "\n");
      curr = curr->next;
    }
  }
  strcat (returnString, "\0");
  // printf ("=== Dump function is good! ===\n");
  return returnString;
}

int linesTB (TB tb) {
  return tb->lines;
  // printf ("=== linesTB function is good! ===\n");
}

void swapTB (TB tb, int pos1, int pos2) {
  assert (tb != NULL);
  if (pos1 < 0 || pos2 < 0 || pos1 > tb->lines - 1 || pos2 > tb->lines - 1) {
    fprintf(stderr, "Position given is out of range.\n");
    abort();
  }
  int i, j = 0;
  char *firstString = NULL;
  char *secondString = NULL;
  link firstNode = tb->head;
  link secondNode = tb->head;
  for (i = 0; i < pos1; i++) {
    firstNode = firstNode->next;
  }
  for (j = 0; j < pos2; j++) {
    secondNode = secondNode->next;
  }
  firstString = strdup (firstNode->text);
  secondString = strdup (secondNode->text);
  firstNode->text = (char *) realloc (firstNode->text, strlen (secondNode->text) * sizeof (char));
  secondNode->text = (char *) realloc (secondNode->text, strlen (firstNode->text) * sizeof (char));
  memset (firstNode->text, 0, strlen (secondNode->text));
  memset (secondNode->text, 0, strlen (firstNode->text));
  strcpy (firstNode->text, secondString);
  strcpy (secondNode->text, firstString);
  // printf ("=== swapTB function is good! ===\n");
}

void mergeTB (TB tb1, int pos, TB tb2) {
  pasteTB (tb1, pos, tb2);
  releaseTB (tb2);
  // printf ("=== mergeTB function is good! ===\n");
}

void pasteTB (TB tb1, int pos, TB tb2) {
  assert (tb1 != NULL && tb2 != NULL);
  if (pos < 0 || pos > tb1->lines) {
    fprintf(stderr, "Position given is out of range.\n");
    abort();
  }
  TB tb2Copy = cloneTB (tb2);
  if (pos == 0) {
    stitchToStart (tb1, tb2Copy);
  } else if (pos != 0 && pos < tb1->lines - 1) {
    stitchToMid (tb1, tb2Copy, pos);
  } else if (pos == tb1->lines - 1) {
    stitchToEnd (tb1, tb2Copy);
  }
  tb1->lines = tb1->lines + tb2->lines;
  // printf ("=== pasteTB function is good! ===\n");
}

TB cutTB (TB tb, int from, int to) {
  TB cuttedTB = copyTB (tb, from, to);
  deleteTB (tb, from, to);
  // printf ("=== cutTB function is good! ===\n");
  return cuttedTB;
}

TB copyTB (TB tb, int from, int to) {
  assert (tb != NULL);
  if (from < 0 || to < 0 || from > tb->lines - 1 || to > tb->lines - 1) {
    fprintf(stderr, "Position given is out of range.\n");
    abort();
  } else if (to < from) return NULL;
  link fromCut = tb->head;
  link toCut = tb->head;
  link temp = NULL;
  int i, j, tempFlag = 0;
  for (i = 0; i < from; i++) {
    fromCut = fromCut->next;
  }
  for (j = 0; j < to; j++) {
    toCut = toCut->next;
  }
  if (toCut->next != NULL) {
    temp = toCut->next;
    toCut->next = NULL;
    tempFlag = 1;
  }
  char *cutText = extractText (fromCut);
  TB copiedTB = newTB (cutText);
  if (tempFlag == 1) {
    toCut->next = temp;
    temp->prev = toCut;
  }
  // printf ("=== copyTB function is good! ===\n");
  return copiedTB;
}

void deleteTB (TB tb, int from, int to) {
  assert (tb != NULL);
  if (from < 0 || to < 0 || from > tb->lines - 1 || to > tb->lines - 1) {
    fprintf(stderr, "Position given is out of range.\n");
    abort();
  }
  if (from <= to) {
    link fromCut = tb->head;
    link toCut = tb->head;
    link curr = tb->head;
    int i, j = 0;
    int linesCut = to - from + 1;
    for (i = 0; i < from; i++) {
      fromCut = fromCut->next;
    }
    for (j = 0; j < to; j++) {
      toCut = toCut->next;
    }
    if (from == 0 && toCut->next != NULL) {
      tb->head = toCut->next;
      tb->head->prev = NULL;
      toCut->next = NULL;
      deleteNodes (fromCut);
    } else if (from != 0 && toCut->next == NULL) {
      tb->tail = fromCut->prev;
      tb->tail->next = NULL;
      deleteNodes (fromCut);
    } else if (from == 0 && toCut->next == NULL) {
      deleteNodes (fromCut);
    } else if (from !=0 && toCut->next != NULL) {
      while (curr->next != fromCut) {
        curr = curr->next;
      }
      curr->next = toCut->next;
      toCut->next->prev = curr;
      toCut->next = NULL;
      deleteNodes (fromCut);
    }
    tb->lines = tb->lines - linesCut;
  }
  // printf ("=== deleteTB function is good! ===\n");
}

void replaceText (TB tb, char* str1, char* str2) {
  assert (tb != NULL);
  link curr = tb->head;
  char newLine[1000];
  if (strcmp (str1, "") != 0 && strcmp (str2, "") != 0) {
    while (curr != NULL) {
      char *p = curr->text;
      while ((p = strstr (p, str1))) {
        strncpy (newLine, curr->text, p - curr->text);
        newLine[p - curr->text] = '\0';
        strcat (newLine, str2);
        strcat (newLine, p + strlen (str1));
        curr->text = (char *) realloc (curr->text, strlen (newLine));
        strcpy (curr->text, newLine);
        memset (newLine, 0, strlen (newLine));
      }
      curr = curr->next;
    }
  }
  // printf ("=== replaceText function is good! ===\n");
}

/* Bonus Challenges
 */

char* diffTB (TB tb1, TB tb2) ;

void undoTB (TB tb) ;

void redoTB (TB tb) ;

/* White Box Testing
 */

void whiteBoxTests (void) {
  printf ("========== WHITE BOX TESTS ==========\n");

  printf ("Test 1: Creating and releasing TB\n");
  TB tb1 = newTB ("hello\nworld\n");
  assert (tb1->lines == 2);
  releaseTB (tb1);
  printf ("Passed\n");

  printf ("Test 2: Creating TB with only newLines\n");
  TB tb2 = newTB ("\n\n\n\n\n");
  assert (tb2->lines == 0);
  releaseTB (tb2);
  printf ("Passed\n");

  printf ("Test 3: Creating TB with NULL chararacter\n");
  TB tb3 = newTB ("\0");
  assert (tb3->lines == 0);
  releaseTB (tb3);
  printf ("Passed\n");

  printf ("Test 4: Creating TB, checking if dumped string is equal to original string\n");
  TB tb4 = newTB ("hello\nworld\nhow\n is life\n?\n");
  char *s4 = dumpTB (tb4);
  assert (strcmp (s4, "hello\nworld\nhow\n is life\n?\n") == 0);
  free (s4);
  releaseTB (tb4);
  printf ("Passed\n");

  printf ("Test 5: Creating TB and checking if number of lines is correct\n");
  TB tb5 = newTB ("1\n2\n3\n\n\n\n4\n5\n");
  assert (tb5->lines == 5);
  releaseTB (tb5);
  printf ("Passed\n");

  printf ("Test 6: Creating TB of 1 line, swapping\n");
  TB tb6 = newTB ("hey!\n");
  assert (tb6->lines == 1);
  swapTB (tb6, 0, 0);
  assert (tb6->lines == 1);
  char *s6 = dumpTB (tb6);
  assert (strcmp (s6, "hey!\n") == 0);
  free (s6);
  releaseTB (tb6);
  printf ("Passed\n");

  printf ("Test 7: Creating TB of 10 lines, swapping\n");
  TB tb7 = newTB("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");
  assert (tb7->lines == 10);
  swapTB (tb7, 0, 1);
  char *s7 = dumpTB (tb7);
  assert (strcmp (s7, "2\n1\n3\n4\n5\n6\n7\n8\n9\n10\n") == 0);
  swapTB (tb7, 1, 8);
  s7 = dumpTB (tb7);
  assert (strcmp (s7, "2\n9\n3\n4\n5\n6\n7\n8\n1\n10\n") == 0);
  swapTB (tb7, 8, 9);
  s7 = dumpTB (tb7);
  assert (strcmp (s7, "2\n9\n3\n4\n5\n6\n7\n8\n10\n1\n") == 0);
  assert (tb7->lines == 10);
  free (s7);
  printf ("Passed\n");

  printf ("Test 8: Merge TB1 and TB2 with no content\n");
  TB tb8_1 = newTB ("\n");
  TB tb8_2 = newTB ("\n");
  assert (tb8_1->lines == 0);
  assert (tb8_2->lines == 0);
  mergeTB (tb8_1, 0, tb8_2);
  assert (tb8_1->lines == 0);
  releaseTB (tb8_1);
  printf ("Passed\n");

  printf ("Test 9: Merge TB1 and TB2 at the start\n");
  TB tb9_1 = newTB ("1\n2\n3\n4\n5\n");
  TB tb9_2 = newTB ("6\n7\n8\n9\n10\n");
  assert (tb9_1->lines == 5 && tb9_2->lines == 5);
  mergeTB (tb9_1, 0, tb9_2);
  assert (tb9_1->lines == 10);
  char *s9 = dumpTB (tb9_1);
  assert (strcmp (s9, "6\n7\n8\n9\n10\n1\n2\n3\n4\n5\n") == 0);
  free (s9);
  releaseTB (tb9_1);
  printf ("Passed\n");

  printf ("Test 10: Merge TB1 and TB2 in the middle\n");
  TB tb10_1 = newTB ("1\n2\n3\n4\n5\n");
  TB tb10_2 = newTB ("6\n7\n8\n9\n10\n");
  assert (tb10_1->lines == 5 && tb10_2->lines == 5);
  mergeTB (tb10_1, 2, tb10_2);
  assert (tb10_1->lines == 10);
  char *s10 = dumpTB (tb10_1);
  assert (strcmp (s10, "1\n2\n6\n7\n8\n9\n10\n3\n4\n5\n") == 0);
  free (s10);
  releaseTB (tb10_1);
  printf ("Passed\n");

  printf ("Test 11: Merge TB1 and TB2 at the end\n");
  TB tb11_1 = newTB ("1\n2\n3\n4\n5\n");
  TB tb11_2 = newTB ("6\n7\n8\n9\n10\n");
  assert (tb11_1->lines == 5 && tb11_2->lines == 5);
  mergeTB (tb11_1, 4, tb11_2);
  assert (tb11_1->lines == 10);
  char *s11 = dumpTB (tb11_1);
  assert (strcmp (s11, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n") == 0);
  free (s11);
  releaseTB (tb11_1);
  printf ("Passed\n");

  printf ("Test 12: Paste TB1 and TB2 with no content\n");
  TB tb12_1 = newTB ("\n");
  TB tb12_2 = newTB ("\n");
  assert (tb12_1->lines == 0);
  assert (tb12_2->lines == 0);
  pasteTB (tb12_1, 0, tb12_2);
  assert (tb12_1->lines == 0);
  assert (tb12_2->lines == 0);
  releaseTB (tb12_1);
  releaseTB (tb12_2);
  printf ("Passed\n");

  printf ("Test 13: Paste TB1 and TB2 at the start\n");
  TB tb13_1 = newTB ("1\n2\n3\n4\n5\n");
  TB tb13_2 = newTB ("6\n7\n8\n9\n10\n");
  assert (tb13_1->lines == 5 && tb13_2->lines == 5);
  pasteTB (tb13_1, 0, tb13_2);
  assert (tb13_1->lines == 10);
  assert (tb13_2->lines == 5);
  char *s13 = dumpTB (tb13_1);
  char *s13_2 = dumpTB (tb13_2);
  assert (strcmp (s13, "6\n7\n8\n9\n10\n1\n2\n3\n4\n5\n") == 0);
  assert (strcmp (s13_2, "6\n7\n8\n9\n10\n") == 0);
  free (s13);
  free (s13_2);
  releaseTB (tb13_1);
  releaseTB (tb13_2);
  printf ("Passed\n");

  printf ("Test 14: Paste TB1 and TB2 in the middle\n");
  TB tb14_1 = newTB ("1\n2\n3\n4\n5\n");
  TB tb14_2 = newTB ("6\n7\n8\n9\n10\n");
  assert (tb14_1->lines == 5 && tb14_2->lines == 5);
  pasteTB (tb14_1, 2, tb14_2);
  assert (tb14_1->lines == 10);
  assert (tb14_2->lines == 5);
  char *s14 = dumpTB (tb14_1);
  char *s14_2 = dumpTB (tb14_2);
  assert (strcmp (s14, "1\n2\n6\n7\n8\n9\n10\n3\n4\n5\n") == 0);
  assert (strcmp (s14_2, "6\n7\n8\n9\n10\n") == 0);
  free (s14);
  free (s14_2);
  releaseTB (tb14_1);
  releaseTB (tb14_2);
  printf ("Passed\n");

  printf ("Test 15: Paste TB1 and TB2 at the end\n");
  TB tb15_1 = newTB ("1\n2\n3\n4\n5\n");
  TB tb15_2 = newTB ("6\n7\n8\n9\n10\n");
  assert (tb15_1->lines == 5 && tb15_2->lines == 5);
  pasteTB (tb15_1, 4, tb15_2);
  assert (tb15_1->lines == 10);
  assert (tb15_2->lines == 5);
  char *s15 = dumpTB (tb15_1);
  char *s15_2 = dumpTB (tb15_2);
  assert (strcmp (s15, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n") == 0);
  assert (strcmp (s15_2, "6\n7\n8\n9\n10\n") == 0);
  free (s15);
  free (s15_2);
  releaseTB (tb15_1);
  releaseTB (tb15_2);
  printf ("Passed\n");

  printf ("Test 16: CutTB check if from > to returns NULL\n");
  TB tb16 = newTB ("1\n2\n3\n");
  assert ((cutTB (tb16, 2, 1)) == NULL) ;
  assert (tb16->lines == 3);
  releaseTB (tb16);
  printf ("Passed\n");

  printf ("Test 17: Cut 1 node from a TB with 1 node\n");
  TB tb17 = newTB ("1\n");
  TB cut_tb17 = cutTB (tb17, 0, 0);
  assert (tb17->lines == 0 && cut_tb17->lines == 1);
  releaseTB (tb17);
  releaseTB (cut_tb17);
  printf ("Passed\n");

  printf ("Test 18: Cut 2 nodes from a TB with 5 nodes from the start\n");
  TB tb18 = newTB ("1\n2\n3\n4\n5\n");
  TB cut_tb18 = cutTB (tb18, 0, 1);
  char *str18 = dumpTB (tb18);
  char *str_cut_tb18 = dumpTB (cut_tb18);
  assert (strcmp (str18, "3\n4\n5\n") == 0);
  assert (strcmp (str_cut_tb18, "1\n2\n") == 0);
  assert (tb18->lines == 3);
  assert (cut_tb18->lines == 2);
  free (str18);
  free (str_cut_tb18);
  releaseTB (tb18);
  releaseTB (cut_tb18);
  printf ("Passed\n");


  printf ("Test 19: Cut 2 nodes from a TB with 5 nodes from the middle\n");
  TB tb19 = newTB ("1\n2\n3\n4\n5\n");
  TB cut_tb19 = cutTB (tb19, 1, 2);
  char *str19 = dumpTB (tb19);
  char *str_cut_tb19 = dumpTB (cut_tb19);
  assert (tb19->lines == 3);
  assert (cut_tb19->lines == 2);
  assert (strcmp (str19, "1\n4\n5\n") == 0);
  assert (strcmp (str_cut_tb19, "2\n3\n") == 0);
  free (str19);
  free (str_cut_tb19);
  releaseTB (tb19);
  releaseTB (cut_tb19);
  printf ("Passed\n");


  printf ("Test 20: Cut 2 nodes from a TB with 5 nodes from the end\n");
  TB tb20 = newTB ("1\n2\n3\n4\n5\n");
  TB cut_tb20 = cutTB (tb20, 3, 4);
  char *str20 = dumpTB (tb20);
  char *str_cut_tb20 = dumpTB (cut_tb20);
  assert (strcmp (str20, "1\n2\n3\n") == 0);
  assert (strcmp (str_cut_tb20, "4\n5\n") == 0);
  assert (tb20->lines == 3);
  assert (cut_tb20->lines == 2);
  free (str20);
  free (str_cut_tb20);
  releaseTB (tb20);
  releaseTB (cut_tb20);
  printf ("Passed\n");

  printf ("Test 21: CopyTB check if from > to returns NULL\n");
  TB tb21 = newTB ("1\n2\n3\n");
  assert ((copyTB (tb21, 2, 1)) == NULL) ;
  assert (tb21->lines == 3);
  releaseTB (tb21);
  printf ("Passed\n");

  printf ("Test 22: Copy 1 node from a TB with 1 node\n");
  TB tb22 = newTB ("1\n");
  TB cut_tb22 = copyTB (tb22, 0, 0);
  assert (tb22->lines == 1 && cut_tb22->lines == 1);
  releaseTB (tb22);
  releaseTB (cut_tb22);
  printf ("Passed\n");

  printf ("Test 23: Copy 2 nodes from a TB with 5 nodes from the start\n");
  TB tb23 = newTB ("1\n2\n3\n4\n5\n");
  TB cut_tb23 = copyTB (tb23, 0, 1);
  char *str23 = dumpTB (tb23);
  char *str_cut_tb23 = dumpTB (cut_tb23);
  assert (strcmp (str23, "1\n2\n3\n4\n5\n") == 0);
  assert (strcmp (str_cut_tb23, "1\n2\n") == 0);
  assert (tb23->lines == 5);
  assert (cut_tb23->lines == 2);
  free (str23);
  free (str_cut_tb23);
  releaseTB (tb23);
  releaseTB (cut_tb23);
  printf ("Passed\n");
  
  printf ("Test 24: Copy 2 nodes from a TB with 5 nodes from the middle\n");
  TB tb24 = newTB ("1\n2\n3\n4\n5\n");
  TB cut_tb24 = copyTB (tb24, 1, 2);
  char *str24 = dumpTB (tb24);
  char *str_cut_tb24 = dumpTB (cut_tb24);
  assert (tb24->lines == 5);
  assert (cut_tb24->lines == 2);
  assert (strcmp (str24, "1\n2\n3\n4\n5\n") == 0);
  assert (strcmp (str_cut_tb24, "2\n3\n") == 0);
  free (str24);
  free (str_cut_tb24);
  releaseTB (tb24);
  releaseTB (cut_tb24);
  printf ("Passed\n");
  
  printf ("Test 25: Copy 2 nodes from a TB with 5 nodes from the end\n");
  TB tb25 = newTB ("1\n2\n3\n4\n5\n");
  TB cut_tb25 = copyTB (tb25, 3, 4);
  char *str25 = dumpTB (tb25);
  char *str_cut_tb25 = dumpTB (cut_tb25);
  assert (strcmp (str25, "1\n2\n3\n4\n5\n") == 0);
  assert (strcmp (str_cut_tb25, "4\n5\n") == 0);
  assert (tb25->lines == 5);
  assert (cut_tb25->lines == 2);
  free (str25);
  free (str_cut_tb25);
  releaseTB (tb25);
  releaseTB (cut_tb25);
  printf ("Passed\n");

  printf ("Test 26: Delete 1 node from a TB with 1 node\n");
  TB tb26 = newTB ("1\n");
  assert (tb26->lines == 1);
  deleteTB (tb26, 0, 0);
  assert (tb26->lines == 0);
  releaseTB (tb26);
  printf ("Passed\n");

  printf ("Test 27: Delete 3 nodes from a TB with 5 nodes from the start\n");
  TB tb27 = newTB ("1\n2\n3\n4\n5\n");
  deleteTB (tb27, 0, 2);
  assert (tb27->lines == 2);
  char *str27 = dumpTB (tb27);
  assert (strcmp (str27, "4\n5\n") == 0);
  releaseTB (tb27);
  printf ("Passed\n");

  printf ("Test 28: Delete 3 nodes from a TB with 5 nodes from the middle\n");
  TB tb28 = newTB ("1\n2\n3\n4\n5\n");
  deleteTB (tb28, 1, 3);
  assert (tb28->lines == 2);
  char *str28 = dumpTB (tb28);
  assert (strcmp (str28, "1\n5\n") == 0);
  releaseTB (tb28);
  printf ("Passed\n");

  printf ("Test 29: Delete 3 nodes from a TB with 5 nodes from the end\n");
  TB tb29 = newTB ("1\n2\n3\n4\n5\n");
  deleteTB (tb29, 2, 4);
  assert (tb29->lines == 2);
  char *str29 = dumpTB (tb29);
  assert (strcmp (str29, "1\n2\n") == 0);
  releaseTB (tb29);
  printf ("Passed\n");

  printf ("Test 30: Delete 3 nodes from a TB with 3 nodes\n");
  TB tb30 = newTB ("1\n2\n3\n");
  deleteTB (tb30, 0, 2);
  assert (tb30->lines == 0);
  releaseTB (tb30);
  printf ("Passed\n");

  printf ("Test 31: Try replacing str1 that doesn't exist with str2\n");
  TB tb31 = newTB ("aa\nbc\ncd\nda\nefg\n");
  replaceText (tb31, "109238019283", "929388383");
  char *str31 = dumpTB (tb31);
  assert (strcmp (str31, "aa\nbc\ncd\nda\nefg\n") == 0);
  free (str31);
  releaseTB (tb31);
  printf ("Passed\n");

  printf ("Test 32: Try replacing str1 with str2 of smaller length than str1\n");
  TB tb32 = newTB ("aa\nbc\ncd\nda\nefg\n");
  replaceText (tb32, "aa", "A");
  char *str32 = dumpTB (tb32);
  assert (strcmp (str32, "A\nbc\ncd\nda\nefg\n") == 0);
  free (str32);
  releaseTB (tb32);
  printf ("Passed\n");

  printf ("Test 33: Try replacing str1 with str2 of greater length than str2\n");
  TB tb33 = newTB ("aa\nbc\ncd\nda\nefg\n");
  replaceText (tb33, "a", "AAAA");
  char *str33 = dumpTB (tb33);
  assert (strcmp (str33, "AAAAAAAA\nbc\ncd\ndAAAA\nefg\n") == 0);
  free (str33);
  releaseTB (tb33);
  printf ("Passed\n");

  printf ("Test 34: Try replacing str1 with str2 of equal length as str1\n");
  TB tb34 = newTB ("aa\nbc\ncd\nda\nefg\n");
  replaceText (tb34, "a", "$");
  char *str34 = dumpTB (tb34);
  assert (strcmp (str34, "$$\nbc\ncd\nd$\nefg\n") == 0);
  free (str34);
  releaseTB (tb34);
  printf ("Passed\n");

  printf ("Test 35: Try replacing nothing with nothing\n");
  TB tb35 = newTB ("aa\nbc\ncd\nda\nefg\n");
  replaceText (tb35, "", "");
  char *str35 = dumpTB (tb35);
  assert (strcmp (str35, "aa\nbc\ncd\nda\nefg\n") == 0);
  free (str35);
  releaseTB (tb35);
  printf ("Passed\n");

  printf ("Test 36: Try replacing newline characters with null characters\n");
  TB tb36 = newTB ("aa\nbc\ncd\nda\nefg\n");
  replaceText (tb36, "\n", "\0");
  char *str36 = dumpTB (tb36);
  assert (strcmp (str36, "aa\nbc\ncd\nda\nefg\n") == 0);
  free (str36);
  releaseTB (tb36);
  printf ("Passed\n");

  printf ("Test 37: Swapping strings from same position\n");
  TB tb37 = newTB ("a\nb\nc\n");
  swapTB (tb37, 1, 1);
  char *str37 = dumpTB (tb37);
  assert (strcmp (str37, "a\nb\nc\n") == 0);
  free (str37);
  releaseTB (tb37);
  printf ("Passed\n");

  printf ("Test 38: Swapping strings from different position\n");
  TB tb38 = newTB ("a\nb\nc\n");
  swapTB (tb38, 1, 2);
  char *str38 = dumpTB (tb38);
  assert (strcmp (str38, "a\nc\nb\n") == 0);
  free (str38);
  releaseTB (tb38);
  printf ("Passed\n");

  printf ("Test 39: Merging something with nothing (tests paste function as well)\n");
  TB tb39 = newTB("1\n2\n3\n");
  TB tb39_2 = newTB("\n");
  assert (tb39->lines == 3);
  assert (tb39_2->lines == 0);
  mergeTB (tb39, 0, tb39_2);
  char *s39 = dumpTB (tb39);
  assert (strcmp (s39, "1\n2\n3\n") == 0);
  free (s39);
  releaseTB (tb39);
  printf ("Passed\n");

  printf ("===== WHITE BOX TESTS ARE GOOD! =====\n");
}

