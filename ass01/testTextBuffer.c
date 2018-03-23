#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "textbuffer.h"

void whiteBoxTests (void);

int main (int argc, char *argv[]) {
  whiteBoxTests ();
  printf ("========== BLACK BOX TESTS ==========\n");
  printf ("Test 40: Mixed testing of all functions\n");

  char str[] = "line 01\n"
               "line 02\n"
               "line 03\n"
               "line 04\n"
               "line 05\n"
               "line 06\n"
               "line 07\n"
               "line 08\n"
               "line 09\n"
               "line 10\n";
  char str2[] = "string 01\n"
                "string 02\n"
                "string 03\n"
                "string 04\n"
                "string 05\n";
  char str3[] = "A\nBCDEFG\nHIJK\n"
                "L\nM\nNOP\n"
                "QRST\nUVW\nX\nY\nZ\n";
  char str4[] = "abcdefghijklmnopqrstuvwxyz\n"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";

  printf ("STEP 1: Creating TBs ONE, TWO, THREE, and FOUR...\n");

  TB ONE = newTB (str);
  TB TWO = newTB (str2);
  TB THREE = newTB (str3);
  TB FOUR = newTB (str4);
  char *one = dumpTB (ONE);
  char *two = dumpTB (TWO);
  char *three = dumpTB (THREE);
  char *four = dumpTB (FOUR);
  int I_lines = linesTB (ONE);
  int II_lines = linesTB (TWO);
  int III_lines = linesTB (THREE);
  int IV_lines = linesTB (FOUR);

  printf ("One =\n%s\nTwo =\n%s\nThree =\n%s\nFour =\n%s\n", one, two, three, four);
  printf ("Number of lines from ONE to FOUR respectively: %d | %d | %d | %d\n", I_lines, II_lines, III_lines, IV_lines);
  assert (linesTB (ONE) == 10 && linesTB (TWO) == 5 && linesTB (THREE) == 11 && linesTB (FOUR) == 2);
  assert (strcmp (one, str) == 0);
  assert (strcmp (two, str2) == 0);
  assert (strcmp (three, str3) == 0);
  assert (strcmp (four, str4) == 0);

  free (one);
  free (two);
  free (three);
  free (four);


  // merge ONE and TWO at end into ONE (TWO is released)
  printf ("STEP 2: Merging ONE and TWO by placing TWO at the end of ONE...\n");
  mergeTB (ONE, 9, TWO);
  printf ("TWO is released\n");
  char *onetwo = dumpTB (ONE);
  assert (strcmp (onetwo, "line 01\n"
                          "line 02\n"
                          "line 03\n"
                          "line 04\n"
                          "line 05\n"
                          "line 06\n"
                          "line 07\n"
                          "line 08\n"
                          "line 09\n"
                          "line 10\n"
                          "string 01\n"
                          "string 02\n"
                          "string 03\n"
                          "string 04\n"
                          "string 05\n") == 0);
  printf ("One now equals=\n%s\n", onetwo);
  assert (linesTB (ONE) == 15);
  free (onetwo);

  // replace all the 0's with bob
  printf ("STEP 3: Replacing all 0's in ONE with bob...\n");
  replaceText (ONE, "0", "bob");
  char *onetwo_2 = dumpTB (ONE);
  assert (strcmp (onetwo_2, "line bob1\n"
                            "line bob2\n"
                            "line bob3\n"
                            "line bob4\n"
                            "line bob5\n"
                            "line bob6\n"
                            "line bob7\n"
                            "line bob8\n"
                            "line bob9\n"
                            "line 1bob\n"
                            "string bob1\n"
                            "string bob2\n"
                            "string bob3\n"
                            "string bob4\n"
                            "string bob5\n") == 0);
  assert (linesTB (ONE) == 15);
  printf ("One now equals =\n%s\n", onetwo_2);
  free (onetwo_2);

  // replace all the string's with H3ll0!
  printf ("STEP 4: Replacing all string's in ONE with H3ll0!...\n");
  replaceText (ONE, "string", "H3ll0!");
  char *onetwo_3 = dumpTB (ONE);
  assert (strcmp (onetwo_3, "line bob1\n"
                            "line bob2\n"
                            "line bob3\n"
                            "line bob4\n"
                            "line bob5\n"
                            "line bob6\n"
                            "line bob7\n"
                            "line bob8\n"
                            "line bob9\n"
                            "line 1bob\n"
                            "H3ll0! bob1\n"
                            "H3ll0! bob2\n"
                            "H3ll0! bob3\n"
                            "H3ll0! bob4\n"
                            "H3ll0! bob5\n") == 0);
  assert (linesTB (ONE) == 15);
  printf ("One now equals =\n%s\n", onetwo_3);
  free (onetwo_3);

  // TB FIVE = cutTB (ONE, 5,12);
  printf ("STEP 5: Cutting ONE from index 5 to 12 into newTB FIVE...\n");
  TB FIVE = cutTB (ONE, 5, 12);
  assert (linesTB (ONE) == 7);
  assert (linesTB (FIVE) == 8);
  char *five = dumpTB (FIVE);
  char *onetwo_4 = dumpTB (ONE);
  assert (strcmp (onetwo_4, "line bob1\n"
                            "line bob2\n"
                            "line bob3\n"
                            "line bob4\n"
                            "line bob5\n"
                            "H3ll0! bob4\n"
                            "H3ll0! bob5\n") == 0);
  assert (strcmp (five, "line bob6\n"
                        "line bob7\n"
                        "line bob8\n"
                        "line bob9\n"
                        "line 1bob\n"
                        "H3ll0! bob1\n"
                        "H3ll0! bob2\n"
                        "H3ll0! bob3\n") == 0);
  printf ("One now equals =\n%s\nFive =\n%s\n", onetwo_4, five);
  free (onetwo_4);
  free (five);

  // paste THREE and FOUR at index 4 into THREE
  printf ("STEP 6: Pasting FOUR into THREE at index 4...\n");
  pasteTB (THREE, 4, FOUR);
  assert (linesTB (THREE) == 13);
  assert (linesTB (FOUR) == 2);
  char *three_6 = dumpTB (THREE);
  char *four_6 = dumpTB (FOUR);
  assert (strcmp (three_6, "A\nBCDEFG\nHIJK\n"
                           "L\n"
                           "abcdefghijklmnopqrstuvwxyz\n"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                           "M\nNOP\n"
                           "QRST\nUVW\nX\nY\nZ\n") == 0);
  assert (strcmp (four_6, "abcdefghijklmnopqrstuvwxyz\n"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n") == 0);
  printf ("Three now equals =\n%s\nFour now equals =\n%s\n", three_6, four_6);
  free (three_6);
  free (four_6);

  // delete part of THREE
  printf ("STEP 7: Deleting from 0 to 4 in THREE...\n");
  deleteTB (THREE, 0, 4);
  assert (linesTB (THREE) == 8);
  char *three_7 = dumpTB (THREE);
  printf ("Three now equals =\n%s\n", three_7);
  assert (strcmp (three_7, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                           "M\nNOP\n"
                           "QRST\nUVW\nX\nY\nZ\n") == 0);
  printf ("Three now equals =\n%s\n", three_7);
  free (three_7);

  // delete part of FOUR
  printf ("STEP 8: Deleting from 1 to 1 in FOUR...\n");
  deleteTB (FOUR, 1, 1);
  assert (linesTB (FOUR) == 1);
  char *four_8 = dumpTB (FOUR);
  assert (strcmp (four_8, "abcdefghijklmnopqrstuvwxyz\n") == 0);
  printf ("Four now equals =\n%s\n", four_8);
  free (four_8);

  // merge THREE and FOUR (FOUR is released) at start
  printf ("STEP 9: Merging FOUR into THREE at start...\n");
  mergeTB (THREE, 0, FOUR);
  printf ("FOUR is released\n");
  assert (linesTB (THREE) == 9);
  char *three_9 = dumpTB (THREE);
  assert (strcmp (three_9, "abcdefghijklmnopqrstuvwxyz\n"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                           "M\nNOP\n"
                           "QRST\nUVW\nX\nY\nZ\n") == 0);
  printf ("Three now equals =\n%s\n", three_9);
  free (three_9);

  // merge ONE and THREE at index 3 (THREE is released)
  printf ("STEP 10: Merging THREE into ONE at 3...\n");
  mergeTB (ONE, 3, THREE);
  printf ("THREE is released\n");
  assert (linesTB (ONE) == 16);
  char *one_10 = dumpTB (ONE);
  assert (strcmp (one_10, "line bob1\n"
                          "line bob2\n"
                          "line bob3\n"
                          "abcdefghijklmnopqrstuvwxyz\n"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                          "M\nNOP\n"
                          "QRST\nUVW\nX\nY\nZ\n"
                          "line bob4\n"
                          "line bob5\n"
                          "H3ll0! bob4\n"
                          "H3ll0! bob5\n") == 0);
  printf ("One now equals =\n%s\n", one_10);
  free (one_10);

  // merge ONE and FIVE at index 1 (FIVE is released)
  printf ("STEP 11: Merging FIVE into ONE at 1...\n");
  mergeTB (ONE, 1, FIVE);
  printf ("FIVE is released\n");
  assert (linesTB (ONE) == 24);
  char *one_11 = dumpTB (ONE);
  assert (strcmp (one_11, "line bob1\n"
                          "line bob6\n"
                          "line bob7\n"
                          "line bob8\n"
                          "line bob9\n"
                          "line 1bob\n"
                          "H3ll0! bob1\n"
                          "H3ll0! bob2\n"
                          "H3ll0! bob3\n"
                          "line bob2\n"
                          "line bob3\n"
                          "abcdefghijklmnopqrstuvwxyz\n"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                          "M\nNOP\n"
                          "QRST\nUVW\nX\nY\nZ\n"
                          "line bob4\n"
                          "line bob5\n"
                          "H3ll0! bob4\n"
                          "H3ll0! bob5\n") == 0);

  printf ("One now equals =\n%s\n", one_11);
  free (one_11);

  // TB SIX = copyTB (ONE, 3, 7);
  printf ("STEP 12: NewTB SIX = copy of ONE from 3 to 7...\n");
  TB SIX = copyTB (ONE, 3, 7);
  assert (linesTB (SIX) == 5);
  assert (linesTB (ONE) == 24);
  char *one_12 = dumpTB (ONE);
  char *six_12 = dumpTB (SIX);
  assert (strcmp (one_12, "line bob1\n"
                          "line bob6\n"
                          "line bob7\n"
                          "line bob8\n"
                          "line bob9\n"
                          "line 1bob\n"
                          "H3ll0! bob1\n"
                          "H3ll0! bob2\n"
                          "H3ll0! bob3\n"
                          "line bob2\n"
                          "line bob3\n"
                          "abcdefghijklmnopqrstuvwxyz\n"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                          "M\nNOP\n"
                          "QRST\nUVW\nX\nY\nZ\n"
                          "line bob4\n"
                          "line bob5\n"
                          "H3ll0! bob4\n"
                          "H3ll0! bob5\n") == 0);
  assert (strcmp (six_12, "line bob8\n"
                          "line bob9\n"
                          "line 1bob\n"
                          "H3ll0! bob1\n"
                          "H3ll0! bob2\n") == 0);
  printf ("One now equals =\n%s\nSix now equals =\n%s\n", one_12, six_12);
  free (one_12);
  free (six_12);

  // swap lines 3 and 21 of ONE
  printf ("STEP 13: Swapping lines 3 and 21 of ONE...\n");
  swapTB (ONE, 3, 21);
  assert (linesTB (ONE) == 24);
  char *one_13 = dumpTB (ONE);
  assert (strcmp (one_13, "line bob1\n"
                          "line bob6\n"
                          "line bob7\n"
                          "line bob5\n"
                          "line bob9\n"
                          "line 1bob\n"
                          "H3ll0! bob1\n"
                          "H3ll0! bob2\n"
                          "H3ll0! bob3\n"
                          "line bob2\n"
                          "line bob3\n"
                          "abcdefghijklmnopqrstuvwxyz\n"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                          "M\nNOP\n"
                          "QRST\nUVW\nX\nY\nZ\n"
                          "line bob4\n"
                          "line bob8\n"
                          "H3ll0! bob4\n"
                          "H3ll0! bob5\n") == 0);
  printf ("One now equals =\n%s\n", one_13);
  free (one_13);

  // release ONE and SIX
  printf ("STEP 14: Releasing remaining TB's...\n");
  releaseTB (ONE);
  releaseTB (SIX);

  printf ("===== BLACK BOX TESTS ARE GOOD! =====\n");
  return EXIT_SUCCESS;
}

