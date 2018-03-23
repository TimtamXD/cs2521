////////////////////////////////////////////////////////////////////////
// COMP2521 18x1 ... the Fury of Dracula
// DracView.c: the DracView ADT implementation
//
// 2014-07-01   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01   v1.1    Team Dracula <cs2521@cse.unsw.edu.au>
////////////////////////////////////////////////////////////////////////
// AUTHOR: Catherine Mai & Jia Jun (Tim) Luo
// zID: z5112559 & z5115679
// TEAM NAME: ¯\_(?)_/¯
////////////////////////////////////////////////////////////////////////
// ==================== KNOWN ERRORS / TODO list ==================== //
// - TODO: whereIs & lastMove & whatsThere & whereCanIgo & whereCanTheyGo
// are not complete. I am exhausted and booked out on Saturday.
// - Look at GameView.c for more information on known errors
// - NOTE: I could not get Catherine's original DracView.c and Map.c
// compiled and/or completed, so I decided to run it over with
// my own implementation.
// Catherine's original DracView.c & Map.c can be accessed via the following link:
// https://www.dropbox.com/sh/ai7qqn3tsnbfanj/AAAgE_Gz70tomUvhC9nHA17Pa?dl=0
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sysexits.h>

#include "Game.h"
#include "GameView.h"
#include "Globals.h"
#include "DracView.h"

// I included Places.h
#include "Places.h"

// ===== CUSTOM ADTS AND FUNCTIONS BELOW ===== //

/* The Play struct represents the CURRENT state of the player.
 * As such, it only keeps track of the CURRENT details of
 * the player. The past data is lost. A previous implementation
 * of this struct used linked lists and had "Play next" as a
 * pointer to the next node, but the "Dubious, test returned 1"
 * error occured. Fearing too much memory was being used, the structure
 * was changed. Sadly, the same error still occurs.
 * The DracView and HunterView will need to keep track of the
 * 13 rounds for an immature Vampire being placed, and keep track of
 * every 6 rounds for when the Vampire matures, hence looking into the
 * past.
 */

typedef struct play *Play;

struct play {
    int location;
    int trap;               // true or false
    int immVampire;         // true or false
    int draculaEncountered; // true or false
    int HP;
    int ID;
    int turn;               // also acts as counter for immature Vampire (0..13)
};

// Function to create newNodes in linked list
static Play newPlay (int id, int prevHP, int prevLoc) {
    Play new = malloc (sizeof (*new));
    assert (new != NULL);
    new->HP = prevHP;
    new->location = prevLoc;
    new->trap = 0;
    new->immVampire = 0;
    new->draculaEncountered = 0;
    new->turn = 0;
    switch (id) {
      case 0:
        new->ID = 0;
        break;
      case 1:
        new->ID = 1;
        break;
      case 2:
        new->ID = 2;
        break;
      case 3:
        new->ID = 3;
        break;
      case 4:
        new->ID = 4;
        break;
    }
    return new;
}

// This is here because there was something wrong with the compiler before
/*
char *strdup (const char *str) {
    int n = strlen (str) + 1;
    char *dup = malloc (n);
    if (dup) {
      strcpy (dup, str);
    }
    return dup;
}
*/

// ===== ORIGINAL ADTS AND FUNCTIONS ===== //

struct dracView {
  int currentRoundNumber;
  int currentTurnNumber;
  int currentPlayerID;
  int currentScore;
  // Following are linked lists recording all history of player's movements
  Play Godalming;
  Play Seward;
  Play Helsing;
  Play Harker;
  Play Dracula;
  // Trail information
  int Gtrail[6];
  int Strail[6];
  int Htrail[6];
  int Mtrail[6];
  int Dtrail[6];
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView (char *pastPlays, PlayerMessage messages[]) {

      DracView new = malloc (sizeof *new);
      if (new == NULL) err (EX_OSERR, "couldn't allocate DracView");

      // Initialise Game
      new->currentRoundNumber = 0;
      new->currentTurnNumber = 0;
      new->currentPlayerID = 0;
      new->currentScore = GAME_START_SCORE;

      // Initialise setup nodes
      new->Godalming = newPlay(0, GAME_START_HUNTER_LIFE_POINTS, NOWHERE);
      new->Seward = newPlay(1, GAME_START_HUNTER_LIFE_POINTS, NOWHERE);
      new->Helsing = newPlay(2, GAME_START_HUNTER_LIFE_POINTS, NOWHERE);
      new->Harker = newPlay(3, GAME_START_HUNTER_LIFE_POINTS, NOWHERE);
      new->Dracula = newPlay(4, GAME_START_BLOOD_POINTS, NOWHERE);

      // Initialise trails
      int i = 0;
      for (i = 0; i < 5; i++) {
        new->Gtrail[i] = NOWHERE;
      }
      for (i = 0; i < 5; i++) {
        new->Strail[i] = NOWHERE;
      }
      for (i = 0; i < 5; i++) {
        new->Htrail[i] = NOWHERE;
      }
      for (i = 0; i < 5; i++) {
        new->Mtrail[i] = NOWHERE;
      }
      for (i = 0; i < 5; i++) {
        new->Dtrail[i] = NOWHERE;
      }

      // Other Variables
      char *copyOfText = strdup (pastPlays);
      char *delim = " ";
      char *token;
      char locationCopy[2];
      int TurnCounter = 0;
      int oldLocation = NOWHERE;
      int newLocation = NOWHERE;
      int doubleBack[5] = {-1};

      for (token = strtok (copyOfText, delim); token; token = strtok (NULL, delim)) {
        switch (token[0]) {
          case 'G':
            locationCopy[0] = token[1];
            locationCopy[1] = token[2];
            oldLocation = new->Godalming->location;
            newLocation = abbrevToID (locationCopy);
            new->currentTurnNumber++;
            new->currentPlayerID = 1;

            // ENCOUNTERS
            if (token[3] == 'T' && token[4] == '.' && token[5] == '.') {
              new->Godalming->HP = new->Godalming->HP - 2; // 2 hp is lost for every trap
              new->Godalming->trap = 1; // trap is encountered and disarmed
              new->Godalming->draculaEncountered = 0;
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == '.') {
              new->Godalming->HP = new->Godalming->HP - 2;
              new->Godalming->trap = 1;
              new->Godalming->draculaEncountered = 0;
              new->Godalming->immVampire = 1; // immature Vampire was encountered and vanquished
            } else if (token[3] == 'T' && token[4] == 'D' && token[5] == '.') {
              new->Godalming->HP = new->Godalming->HP - 6; // -2 for trap -4 for dracula
              new->Godalming->trap = 1;
              new->Godalming->draculaEncountered = 1; // dracula was encountered
              new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
              new->Godalming->HP = new->Godalming->HP - 6;
              new->Godalming->trap = 1;
              new->Godalming->immVampire = 1;
              new->Godalming->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
              new->Godalming->immVampire = 1;
              new->Godalming->draculaEncountered = 0;
            } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
              new->Godalming->HP = new->Godalming->HP - 4;
              new->Godalming->immVampire = 1;
              new->Godalming->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
              new->Godalming->HP = new->Godalming->HP - 4;
              new->Godalming->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            }

            // ENCOUNTERS 2 - IMMATURE VAMPIRE
            // Every 6 rounds, vampire matures.
            // Score is reduced by 13 if vampire is not encountered yet.
            if (new->currentTurnNumber != 0 && new->currentTurnNumber % 6 == 0) {
              if (new->Godalming->immVampire == 0) {
                new->currentScore = new->currentScore - 13;
              } else if (new->Godalming->immVampire == 1) {
                new->Godalming->immVampire = 0;
              }
            }

            // HP adjustments
            if (new->Godalming->HP <= 0) {
              // TODO: Teleports back to hospital
              // Everytime teleport occurs, gamescore = gamescore - 6;
              new->Godalming->HP = 0;
              new->currentScore = new->currentScore - 6;
            }

            // Location adjustments
            if (oldLocation == newLocation && new->Godalming->HP < 9 && new->Godalming->HP > 0) {
              if (new->Godalming->HP >= 6) {
                new->Godalming->HP = 9; // cannot exceed 9
              } else {
                new->Godalming->HP = new->Godalming->HP + 3; // +3 if rested
              }
            }

            // Trail adjustments
            new->Gtrail[5] = new->Gtrail[4];
            new->Gtrail[4] = new->Gtrail[3];
            new->Gtrail[3] = new->Gtrail[2];
            new->Gtrail[2] = new->Gtrail[1];
            new->Gtrail[1] = new->Gtrail[0];
            new->Gtrail[0] = newLocation;

            // Final adjustments
            new->Godalming->location = newLocation;
            new->Godalming->turn = new->currentTurnNumber;
            TurnCounter++;
            break;
          case 'S':
            locationCopy[0] = token[1];
            locationCopy[1] = token[2];
            oldLocation = new->Seward->location;
            newLocation = abbrevToID (locationCopy);
            new->currentTurnNumber++;
            new->currentPlayerID = 1;

            // ENCOUNTERS
            if (token[3] == 'T' && token[4] == '.' && token[5] == '.') {
              new->Seward->HP = new->Seward->HP - 2; // 2 hp is lost for every trap
              new->Seward->trap = 1; // trap is encountered and disarmed
              new->Seward->draculaEncountered = 0;
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == '.') {
              new->Seward->HP = new->Seward->HP - 2;
              new->Seward->trap = 1;
              new->Seward->draculaEncountered = 0;
              new->Seward->immVampire = 1; // immature Vampire was encountered and vanquished
            } else if (token[3] == 'T' && token[4] == 'D' && token[5] == '.') {
              new->Seward->HP = new->Seward->HP - 6; // -2 for trap -4 for dracula
              new->Seward->trap = 1;
              new->Seward->draculaEncountered = 1; // dracula was encountered
              new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
              new->Seward->HP = new->Seward->HP - 6;
              new->Seward->trap = 1;
              new->Seward->immVampire = 1;
              new->Seward->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
              new->Seward->immVampire = 1;
              new->Seward->draculaEncountered = 0;
            } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
              new->Seward->HP = new->Seward->HP - 4;
              new->Seward->immVampire = 1;
              new->Seward->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
              new->Seward->HP = new->Seward->HP - 4;
              new->Seward->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            }

            // ENCOUNTERS 2 - IMMATURE VAMPIRE
            // Every 6 rounds, vampire matures.
            // Score is reduced by 13 if vampire is not encountered yet.
            if (new->currentTurnNumber != 0 && new->currentTurnNumber % 6 == 0) {
              if (new->Seward->immVampire == 0) {
                new->currentScore = new->currentScore - 13;
              } else if (new->Seward->immVampire == 1) {
                new->Seward->immVampire = 0;
              }
            }

            // HP adjustments
            if (new->Seward->HP <= 0) {
              // TODO: Teleports back to hospital
              // Everytime teleport occurs, gamescore = gamescore - 6;
              new->Seward->HP = 0;
              new->currentScore = new->currentScore - 6;
            }

            // Location adjustments
            if (oldLocation == newLocation && new->Seward->HP < 9 && new->Seward->HP > 0) {
              if (new->Seward->HP >= 6) {
                new->Seward->HP = 9; // cannot exceed 9
              } else {
                new->Seward->HP = new->Seward->HP + 3; // +3 if rested
              }
            }

            // Trail adjustments
            new->Strail[5] = new->Strail[4];
            new->Strail[4] = new->Strail[3];
            new->Strail[3] = new->Strail[2];
            new->Strail[2] = new->Strail[1];
            new->Strail[1] = new->Strail[0];
            new->Strail[0] = newLocation;

            // Final adjustments
            new->Seward->location = newLocation;
            new->Seward->turn = new->currentTurnNumber;
            TurnCounter++;
            break;
          case 'H':
            locationCopy[0] = token[1];
            locationCopy[1] = token[2];
            oldLocation = new->Helsing->location;
            newLocation = abbrevToID (locationCopy);
            new->currentTurnNumber++;
            new->currentPlayerID = 1;

            // ENCOUNTERS
            if (token[3] == 'T' && token[4] == '.' && token[5] == '.') {
              new->Helsing->HP = new->Helsing->HP - 2; // 2 hp is lost for every trap
              new->Helsing->trap = 1; // trap is encountered and disarmed
              new->Helsing->draculaEncountered = 0;
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == '.') {
              new->Helsing->HP = new->Helsing->HP - 2;
              new->Helsing->trap = 1;
              new->Helsing->draculaEncountered = 0;
              new->Helsing->immVampire = 1; // immature Vampire was encountered and vanquished
            } else if (token[3] == 'T' && token[4] == 'D' && token[5] == '.') {
              new->Helsing->HP = new->Helsing->HP - 6; // -2 for trap -4 for dracula
              new->Helsing->trap = 1;
              new->Helsing->draculaEncountered = 1; // dracula was encountered
              new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
              new->Helsing->HP = new->Helsing->HP - 6;
              new->Helsing->trap = 1;
              new->Helsing->immVampire = 1;
              new->Helsing->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
              new->Helsing->immVampire = 1;
              new->Helsing->draculaEncountered = 0;
            } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
              new->Helsing->HP = new->Helsing->HP - 4;
              new->Helsing->immVampire = 1;
              new->Helsing->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
              new->Helsing->HP = new->Helsing->HP - 4;
              new->Helsing->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            }

            // ENCOUNTERS 2 - IMMATURE VAMPIRE
            // Every 6 rounds, vampire matures.
            // Score is reduced by 13 if vampire is not encountered yet.
            if (new->currentTurnNumber != 0 && new->currentTurnNumber % 6 == 0) {
              if (new->Helsing->immVampire == 0) {
                new->currentScore = new->currentScore - 13;
              } else if (new->Helsing->immVampire == 1) {
                new->Helsing->immVampire = 0;
              }
            }

            // HP adjustments
            if (new->Helsing->HP <= 0) {
              // TODO: Teleports back to hospital
              // Everytime teleport occurs, gamescore = gamescore - 6;
              new->Helsing->HP = 0;
              new->currentScore = new->currentScore - 6;
            }

            // Location adjustments
            if (oldLocation == newLocation && new->Helsing->HP < 9 && new->Helsing->HP > 0) {
              if (new->Helsing->HP >= 6) {
                new->Helsing->HP = 9; // cannot exceed 9
              } else {
                new->Helsing->HP = new->Helsing->HP + 3; // +3 if rested
              }
            }

            // Trail adjustments
            new->Htrail[5] = new->Htrail[4];
            new->Htrail[4] = new->Htrail[3];
            new->Htrail[3] = new->Htrail[2];
            new->Htrail[2] = new->Htrail[1];
            new->Htrail[1] = new->Htrail[0];
            new->Htrail[0] = newLocation;

            // Final adjustments
            new->Helsing->location = newLocation;
            new->Helsing->turn = new->currentTurnNumber;
            TurnCounter++;
            break;
          case 'M':
            locationCopy[0] = token[1];
            locationCopy[1] = token[2];
            oldLocation = new->Harker->location;
            newLocation = abbrevToID (locationCopy);
            new->currentTurnNumber++;
            new->currentPlayerID = 1;

            // ENCOUNTERS
            if (token[3] == 'T' && token[4] == '.' && token[5] == '.') {
              new->Harker->HP = new->Harker->HP - 2; // 2 hp is lost for every trap
              new->Harker->trap = 1; // trap is encountered and disarmed
              new->Harker->draculaEncountered = 0;
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == '.') {
              new->Harker->HP = new->Harker->HP - 2;
              new->Harker->trap = 1;
              new->Harker->draculaEncountered = 0;
              new->Harker->immVampire = 1; // immature Vampire was encountered and vanquished
            } else if (token[3] == 'T' && token[4] == 'D' && token[5] == '.') {
              new->Harker->HP = new->Harker->HP - 6; // -2 for trap -4 for dracula
              new->Harker->trap = 1;
              new->Harker->draculaEncountered = 1; // dracula was encountered
              new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
            } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
              new->Harker->HP = new->Harker->HP - 6;
              new->Harker->trap = 1;
              new->Harker->immVampire = 1;
              new->Harker->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
              new->Harker->immVampire = 1;
              new->Harker->draculaEncountered = 0;
            } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
              new->Harker->HP = new->Harker->HP - 4;
              new->Harker->immVampire = 1;
              new->Harker->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
              new->Harker->HP = new->Harker->HP - 4;
              new->Harker->draculaEncountered = 1;
              new->Dracula->HP = new->Dracula->HP - 10;
            }

            // ENCOUNTERS 2 - IMMATURE VAMPIRE
            // Every 6 rounds, vampire matures.
            // Score is reduced by 13 if vampire is not encountered yet.
            if (new->currentTurnNumber != 0 && new->currentTurnNumber % 6 == 0) {
              if (new->Harker->immVampire == 0) {
                new->currentScore = new->currentScore - 13;
              } else if (new->Harker->immVampire == 1) {
                new->Harker->immVampire = 0;
              }
            }

            // HP adjustments
            if (new->Harker->HP <= 0) {
              // TODO: Teleports back to hospital
              // Everytime teleport occurs, gamescore = gamescore - 6;
              new->Harker->HP = 0;
              new->currentScore = new->currentScore - 6;
            }

            // Location adjustments
            if (oldLocation == newLocation && new->Harker->HP < 9 && new->Harker->HP > 0) {
              if (new->Harker->HP >= 6) {
                new->Harker->HP = 9; // cannot exceed 9
              } else {
                new->Harker->HP = new->Harker->HP + 3; // +3 if rested
              }
            }

            // Trail adjustments
            new->Mtrail[5] = new->Mtrail[4];
            new->Mtrail[4] = new->Mtrail[3];
            new->Mtrail[3] = new->Mtrail[2];
            new->Mtrail[2] = new->Mtrail[1];
            new->Mtrail[1] = new->Mtrail[0];
            new->Mtrail[0] = newLocation;

            // Final adjustments
            new->Harker->location = newLocation;
            new->Harker->turn = new->currentTurnNumber;
            TurnCounter++;
            break;
          case 'D':
            new->currentTurnNumber++;
            new->currentPlayerID = 0;
            locationCopy[0] = token[1];
            locationCopy[1] = token[2];
            new->Dracula->location = abbrevToID (locationCopy);

            // Location adjustments 1
            if (token[1] == 'C' && token[2] == '?') {
              new->Dracula->location = CITY_UNKNOWN;
            } else if (token[1] == 'S' && token[2] == '?') {
              new->Dracula->location = SEA_UNKNOWN;
            } else if (token[1] == 'H' && token[2] == 'I') {
              new->Dracula->location = HIDE;
            } else if (token[1] == 'D') {
              switch (token[2]) {
                case '1':
                  new->Dracula->location = DOUBLE_BACK_1;
                  if (doubleBack[0] == SEA_UNKNOWN || PLACES[doubleBack[0]].type == SEA) {
                    new->Dracula->HP = new->Dracula->HP - 2;
                  }
                  break;
                case '2':
                  new->Dracula->location = DOUBLE_BACK_2;
                  if (doubleBack[1] == SEA_UNKNOWN || PLACES[doubleBack[1]].type == SEA) {
                    new->Dracula->HP = new->Dracula->HP - 2;
                  }
                  break;
                case '3':
                  new->Dracula->location = DOUBLE_BACK_3;
                  if (doubleBack[2] == SEA_UNKNOWN || PLACES[doubleBack[2]].type == SEA) {
                    new->Dracula->HP = new->Dracula->HP - 2;
                  }
                  break;
                case '4':
                  new->Dracula->location = DOUBLE_BACK_4;
                  if (doubleBack[3] == SEA_UNKNOWN || PLACES[doubleBack[3]].type == SEA) {
                    new->Dracula->HP = new->Dracula->HP - 2;
                  }
                  break;
                case '5':
                  new->Dracula->location = DOUBLE_BACK_5;
                  if (doubleBack[4] == SEA_UNKNOWN || PLACES[doubleBack[4]].type == SEA) {
                    new->Dracula->HP = new->Dracula->HP - 2;
                  }
                  break;
              }
            } else if (token[1] == 'T' && token[2] == 'P') {
              new->Dracula->location = TELEPORT;
            }

            // Location adjustments 2
            if (token[3] == 'T' && token[4] == '.') {
              new->Dracula->trap = 1;
            } else if (token[3] == 'V' && token[4] == '.') {
              new->Dracula->immVampire = 1;
            } else if (token[3] == 'T' && token[4] == 'V') {
              new->Dracula->trap = 1;
              new->Dracula->immVampire = 1;
            }

            // Location adjustments 3
            if (token[5] == 'M' && token[6] == '.') {
              new->Dracula->trap = -1; // trap left trail
            } else if (token[5] == 'V' && token[6] == '.') {
              new->Dracula->immVampire = 2; // 2 means maturity
              new->currentScore = new->currentScore - 13; // gamescore - 13
            } else if (token[5] == 'M' && token[6] == 'V') {
              new->Dracula->trap = -1;
              new->Dracula->immVampire = 2;
              new->currentScore = new->currentScore - 13;
            }

            // Double Back Array Adjustments
            doubleBack[4] = doubleBack[3];
            doubleBack[3] = doubleBack[2];
            doubleBack[2] = doubleBack[1];
            doubleBack[1] = doubleBack[0];
            doubleBack[0] = new->Dracula->location;

            // Trail adjustments
            new->Dtrail[5] = new->Dtrail[4];
            new->Dtrail[4] = new->Dtrail[3];
            new->Dtrail[3] = new->Dtrail[2];
            new->Dtrail[2] = new->Dtrail[1];
            new->Dtrail[1] = new->Dtrail[0];
            new->Dtrail[0] = new->Dracula->location;

            // HP adjustments
            // Check if Dracula is at SEA
            if (new->Dracula->location == SEA_UNKNOWN || PLACES[new->Dracula->location].type == SEA) {
              new->Dracula->HP = new->Dracula->HP - 2;
            }
            // If Dracula's HP reaches 0, game over.
            if (new->Dracula->HP <= 0) {
              puts ("GAME OVER. HUNTERS HAVE WON.");
            }
            // If Dracula's at Castle Dracula, he gains 10 blood points.
            if (new->Dracula->location == CASTLE_DRACULA) {
              if (new->Dracula->HP >= 30) {
                new->Dracula->HP = 40;
              } else {
                new->Dracula->HP = new->Dracula->HP + 10;
              }
            }

            // Score adjustments
            new->currentScore--; // End of Dracula's turn means score - 1
            new->Dracula->turn = new->currentTurnNumber;
            TurnCounter++;
            break;
        }
        if (TurnCounter == 5) {
          new->currentRoundNumber++;
          TurnCounter = 0;
        }
      }

      return new;
}

// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView (DracView toBeDeleted) {
  free (toBeDeleted->Godalming);
  free (toBeDeleted->Seward);
  free (toBeDeleted->Helsing);
  free (toBeDeleted->Harker);
  free (toBeDeleted->Dracula);
  free (toBeDeleted);
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound (DracView dv) {
    return dv->currentRoundNumber;
}

// Get the current score
int giveMeTheScore (DracView dv) {
    return dv->currentScore;
}

// Get the current health points for a given player
int howHealthyIs (DracView dv, PlayerID player) {
    int health = 0;
    switch (player) {
      case 0:
        health = dv->Godalming->HP;
        break;
      case 1:
        health = dv->Seward->HP;
        break;
      case 2:
        health = dv->Helsing->HP;
        break;
      case 3:
        health = dv->Harker->HP;
        break;
      case 4:
        health = dv->Dracula->HP;
        break;
    }
    return health;
}

// Get the current location id of a given player
LocationID whereIs (DracView dv, PlayerID player) {
  int locationID;
  switch (player) {
    case 0:
      locationID = dv->Godalming->location;
      break;
    case 1:
      locationID = dv->Seward->location;
      break;
    case 2:
      locationID = dv->Helsing->location;
      break;
    case 3:
      locationID = dv->Harker->location;
      break;
    case 4:
      locationID = dv->Dracula->location;
  }
  return locationID;
}

// Get the most recent move of a given player
// Returns the start location and end location of that move
// Since Dracula moves last, the end location for other players is always known,
//   but the start location may be UNKNOWN_LOCATION (for a hunter's first move)
// The start and end locations can be the same
void lastMove (DracView dv, PlayerID player, LocationID *start, LocationID *end) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return;
}

// Find out what minions I (Dracula) has placed at the specified location
//   (minions are traps and immature vampires)
// Places counts in the vars referenced by the 3rd and 4th parameters
// If where is not a place where minions can be left (e.g. at sea,
//   or NOWHERE), then set both counts to zero
void whatsThere (DracView dv, LocationID where, int *numTraps, int *numVamps) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
//   for the given player, including Dracula (if he asks about his own trail)
// For any player if the move does not exist yet (i.e, the start of the game),
//   the value should be UNKNOWN_LOCATION (-1)
// For example after 2 turns the array may have the contents
//   {29, 12, -1, -1, -1, -1}
// This would mean in the first move the player started on location 12
//   then moved to the current location of 29
// If Dracula asks about his own trail, he should get precise information
//   about where he has been (real locations, not double-back, etc)
void giveMeTheTrail (DracView dv, PlayerID player, LocationID trail[TRAIL_SIZE]) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID * whereCanIgo (DracView dv, int *numLocations, bool road, bool sea) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}

// What are the specified player's next possible moves
LocationID * whereCanTheyGo (DracView dv, int *numLocations, PlayerID player, bool road, bool rail, bool sea) {
    // TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numLocations = 0;
    return NULL;
}

