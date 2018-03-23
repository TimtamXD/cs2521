//An implementation of a binary search tree

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "BST.h"

// typedef struct treeNode * treelink
struct treeNode {
    TreeItem item;
    treelink left;
    treelink right;
};

static treelink createNode (TreeItem item) {
     treelink t = (treelink) malloc (sizeof (*t));
     t->item = item;
     t->left = NULL;
     t->right = NULL;
     return t;
}

TreeItem getItem (treelink t) {
     assert (t != NULL);
     return t->item;
}

void printTreeNode (treelink t) {
     if (t != NULL)
     printf ("%d ",t->item);
}

void preorderTraversal (treelink tree, void (*visit) (treelink)) {
    if (tree == NULL) {
       return;
    }
    (*visit)(tree);
    preorderTraversal (tree->left,visit);
    preorderTraversal (tree->right,visit);
}

//This function inserts duplcates on the left
treelink insertTreeNode (treelink tree, TreeItem item) {
  if (tree == NULL) {
      tree = createNode(item);
      tree->item = item;
      tree->left = NULL;
      tree->right = NULL;

  } else if (tree->item < item) {
      tree->right = insertTreeNode (tree->right, item);
  } else {
      tree->left = insertTreeNode (tree->left, item);
  }
  return tree;
}

int size(treelink t){
    if(t == NULL){
        return 0;
    } else {
        return 1 + size(t->left) + size(t->right);
    }
}

//returns the node of the element with item i
treelink search(treelink t, TreeItem i){
    treelink result = NULL;
    if (t == NULL) {
        result = NULL;
    } else if( i < t->item ){
        result = search(t->left,i);
    } else if( i > t->item ){
        result = search(t->right,i);
    } else {
        result = t;
    }
    return result;
}

int countLeaves (treelink tree) {
  if (tree == NULL) return 0;
  if (tree->left == NULL && tree->right == NULL) return 1;
  else
    return countLeaves (tree->left) + countLeaves (tree->right);
}

TreeItem * levelOrderTraversal (treelink tree) {
  int treeSize = countLeaves (tree);
  if (tree == NULL || treeSize == 0) return NULL;
  int *array = (int *) malloc (treeSize * sizeof (int));
  memset (array, 0, treeSize);
  int arrayIndex = 0;
  array[0] = tree->item;
  for (int head = 0; head < treeSize; head++) {
    treelink curr = search (tree, array[head]);
    if (curr->left != NULL && curr->right != NULL) {
      array[arrayIndex + 1] = curr->left->item;
      array[arrayIndex + 2] = curr->right->item;
      arrayIndex = arrayIndex + 2;
    } else if (curr->left == NULL && curr->right != NULL) {
      array[arrayIndex + 1] = curr->right->item;
      arrayIndex++;
    } else if (curr->left != NULL && curr->right == NULL) {
      array[arrayIndex + 1] = curr->left->item;
      arrayIndex++;
    }
  }
  return array;
}

int countIf (treelink tree, int (*pred)(TreeItem)) {
  int *array = levelOrderTraversal (tree);
  int i, count = 0;
  int arrayLen = sizeof (array) / sizeof (int);
  for (i = 0; i < arrayLen; i++) {
    if ((*pred)(array[i]) == 1) count++;
  }
   return count;
}

int isEven (TreeItem n) {
  if (n % 2 == 0) return 1;
  return 0;
}

int isOdd (TreeItem n) {
  if (n % 2 != 0) return 1;
  return 0;
}

int isNegative (TreeItem n) {
  if (n < 0) return 1;
  return 0;
}

static void whiteBoxTests (void) {
  printf ("=== WHITE BOX TESTS ===\n");
  printf ("Test 1: Tree of 1 node:\n");
  treelink tree1 = createNode (-1);
  int *array1 = levelOrderTraversal (tree1);
  int count1 = countLeaves (tree1);
  assert (array1[0] == -1);
  assert (tree1->item == -1);
  assert (count1 == 1);
  printf ("Test 1.1: isEven:\n");
  int a = countIf (tree1, isEven());
  assert (a == 0);
  printf ("Test 1.2: isOdd:\n");
  int b = countIf (tree1, isOdd());
  assert (b == 0);
  printf ("Test 1.3: isNegative:\n");
  int c = countIf (tree1, isNegative());
  assert (c == 1);
  printf ("Passed.\n");
  free (tree1);
  free (array1);
  printf ("Test 2: Tree of multiple nodes:\n");
  treelink tree2 = createNode (1);
  tree2 = insertTreeNode (tree2, 0);
  tree2 = insertTreeNode (tree2, 2);
  tree2 = insertTreeNode (tree2, 5);
  tree2 = insertTreeNode (tree2, 3);
  tree2 = insertTreeNode (tree2, -3);
  tree2 = insertTreeNode (tree2, -2);
  tree2 = insertTreeNode (tree2, 4);
  int *array2 = levelOrderTraversal (tree2);
  int count2 = countLeaves (tree2);
  assert (array2[0] == 1);
  assert (array2[1] == 0);
  assert (array2[2] == 2);
  assert (array2[3] == -3);
  assert (array2[4] == 5);
  assert (array2[5] == -2);
  assert (array2[6] == 3);
  assert (array2[7] == 4);
  assert (count2 == 2);
  printf ("Test 2.1: isEven:\n");
  a = countIf (tree2, isEven());
  assert (a == 4);
  printf ("Test 2.2: isOdd:\n");
  b = countIf (tree2, isOdd());
  assert (b == 4);
  printf ("Test 2.3: isNegative:\n");
  c = countIf (tree2, isNegative());
  assert (c == 2);
  printf ("Passed.\n");
  free (tree2);
  free (array2);
  printf ("=== WB TESTS PASSED ===\n");
}

