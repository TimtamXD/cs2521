#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "BST.h"

void whiteBoxTests (void);

int main (int argc, char ** argv) {
  whiteBoxTests();
  printf ("=== BLACK BOX TESTS ===\n");
  printf ("Test 3: countLeaves 1:\n");
  treelink test3 = NULL;
  test3 = insertTreeNode (test3, 1);
  assert (countLeaves (test3) == 1);
  assert (size (test3) == 1);
  printf ("Passed.\n");
  printf ("Test 4: countLeaves multiple:\n");
  treelink test4 = NULL;
  test4 = insertTreeNode (test4, 5);
  test4 = insertTreeNode (test4, 3);
  test4 = insertTreeNode (test4, 9);
  test4 = insertTreeNode (test4, -2);
  test4 = insertTreeNode (test4, 2);
  test4 = insertTreeNode (test4, -1);
  test4 = insertTreeNode (test4, 0);
  test4 = insertTreeNode (test4, 1);
  test4 = insertTreeNode (test4, 4);
  test4 = insertTreeNode (test4, -3);
  test4 = insertTreeNode (test4, 6);
  assert (size (test4) == 11);
  assert (countLeaves (test4) == 5);
  printf ("Passed.\n");
  printf ("Test 5: levelOrderTraversal 1:\n");
  int *array3 = levelOrderTraversal (tree3);
  printf ("The expected output is: 1\n");
  printf ("The current output is:");
  printf ("%d\n", array3[0]);
  printf ("Passed.\n");
  free (array3);
  printf ("Test 5: levelOrderTraversal 2:\n");
  int *array4 = levelOrderTraversal (tree4);
  printf ("The expected output is: 5 3 9 -2 6 2 -1 4 -3 0 1\n");
  printf ("The current output is:");
  printf ("%d %d %d %d %d %d %d %d %d %d %d %d\n", array4[0], array4[1], array4[2], array4[3], array4[4], array4[5], array4[6], array4[7], array4[8], array4[9], array4[10]);
  printf ("Passed.\n");
  printf ("Test 6: isEven:\n");
  assert (countIf (test4, isEven) == 5);
  printf ("Passed.\n");
  printf ("Test 7: isOdd:\n");
  assert (countIf (test4, isOdd) == 6);
  printf ("Passed.\n");
  printf ("Test 8: isNegative:\n");
  assert (countIf (test4, isNegative) == 3);
  printf ("Passed.\n");
  free (array4);
  free (test3);
  free (test4);
  printf ("=== BB TESTS PASSED ===\n");
  return EXIT_SUCCESS;
}

