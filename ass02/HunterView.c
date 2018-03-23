////////////////////////////////////////////////////////////////////////
// COMP2521 18x1 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01   v1.1    Team Dracula <cs2521@cse.unsw.edu.au>
////////////////////////////////////////////////////////////////////////
// AUTHOR: Jia Jun (Tim) Luo
// zID: z5115679
// TEAM NAME: ¯\_(?)_/¯
////////////////////////////////////////////////////////////////////////
// ==================== KNOWN ERRORS / TODO list ==================== //
// - TODO: whereCanIgo & whereCanTheyGo are not complete
// - Look at GameView.c for more information on known errors
// - CHANGES:
//    - whereIs and giveMeTheTrail were changed to effectively
//      reflect the situations where the location of dracula
//      were either known or unknown.
//    - HunterView now has lastKnownDracLocation variable. If dracula is
//      encountered, it will store the location. Added DracLocationCounter.
//      After 6 rounds, this counter will reset and lastKnownDracLocation will
//      clear.
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
#include "HunterView.h"

// I included Places.h
#include "Places.h"

// ===== CUSTOM ADTS AND FUNCTIONS BELOW ===== //

/* The Play struct represents the CURRENT state of the player.
 * As such, it only keeps track of the CURRENT details of
 * the player. The past data is lost.
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

struct hunterView {
    int currentRoundNumber;
    int currentTurnNumber;
    int currentPlayerID;
    int currentScore;
    int lastKnownDracLocation;
    int DracLocationCounter;
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

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView (char *pastPlays, PlayerMessage messages[]) {

    HunterView new = malloc (sizeof *new);
    if (new == NULL) err (EX_OSERR, "couldn't allocate HunterView");

    // Initialise Game
    new->currentRoundNumber = 0;
    new->currentTurnNumber = 0;
    new->currentPlayerID = 0;
    new->currentScore = GAME_START_SCORE;
    new->lastKnownDracLocation = NOWHERE;
    new->DracLocationCounter = 0;

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
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
          } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
            new->Godalming->HP = new->Godalming->HP - 6;
            new->Godalming->trap = 1;
            new->Godalming->immVampire = 1;
            new->Godalming->draculaEncountered = 1;
            new->DracLocationCounter = 1;
            new->lastKnownDracLocation = newLocation;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
            new->Godalming->immVampire = 1;
            new->Godalming->draculaEncountered = 0;
          } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
            new->Godalming->HP = new->Godalming->HP - 4;
            new->Godalming->immVampire = 1;
            new->Godalming->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
            new->Godalming->HP = new->Godalming->HP - 4;
            new->Godalming->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
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
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
          } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
            new->Seward->HP = new->Seward->HP - 6;
            new->Seward->trap = 1;
            new->Seward->immVampire = 1;
            new->Seward->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
            new->Seward->immVampire = 1;
            new->Seward->draculaEncountered = 0;
          } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
            new->Seward->HP = new->Seward->HP - 4;
            new->Seward->immVampire = 1;
            new->Seward->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
            new->Seward->HP = new->Seward->HP - 4;
            new->Seward->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
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
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
          } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
            new->Helsing->HP = new->Helsing->HP - 6;
            new->Helsing->trap = 1;
            new->Helsing->immVampire = 1;
            new->Helsing->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
            new->Helsing->immVampire = 1;
            new->Helsing->draculaEncountered = 0;
          } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
            new->Helsing->HP = new->Helsing->HP - 4;
            new->Helsing->immVampire = 1;
            new->Helsing->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
            new->Helsing->HP = new->Helsing->HP - 4;
            new->Helsing->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
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
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10; // dracula also loses 10 hp
          } else if (token[3] == 'T' && token[4] == 'V' && token[5] == 'D') {
            new->Harker->HP = new->Harker->HP - 6;
            new->Harker->trap = 1;
            new->Harker->immVampire = 1;
            new->Harker->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'V' && token[4] == '.' && token[5] == '.') {
            new->Harker->immVampire = 1;
            new->Harker->draculaEncountered = 0;
          } else if (token[3] == 'V' && token[4] == 'D' && token[5] == '.') {
            new->Harker->HP = new->Harker->HP - 4;
            new->Harker->immVampire = 1;
            new->Harker->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
            new->Dracula->HP = new->Dracula->HP - 10;
          } else if (token[3] == 'D' && token[4] == '.' && token[5] == '.') {
            new->Harker->HP = new->Harker->HP - 4;
            new->Harker->draculaEncountered = 1;
            new->lastKnownDracLocation = newLocation;
            new->DracLocationCounter = 1;
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
      // Ending adjustments
      if (TurnCounter == 5) {
        new->currentRoundNumber++;
        TurnCounter = 0;
      }
      if (new->lastKnownDracLocation != UNKNOWN_LOCATION) new->DracLocationCounter++;
      if (new->DracLocationCounter == 7) {
        new->DracLocationCounter = 0;
        new->lastKnownDracLocation = UNKNOWN_LOCATION;
      }
      //printf ("%d\n", new->DracLocationCounter);
    }

    return new;
}

// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView (HunterView toBeDeleted) {
  free (toBeDeleted->Godalming);
  free (toBeDeleted->Seward);
  free (toBeDeleted->Helsing);
  free (toBeDeleted->Harker);
  free (toBeDeleted->Dracula);
  free (toBeDeleted);
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound (HunterView hv) {
    return hv->currentRoundNumber;
}

// Get the id of current player
PlayerID whoAmI (HunterView hv) {
    return hv->currentPlayerID;
}

// Get the current score
int giveMeTheScore (HunterView hv) {
    return hv->currentScore;
}

// Get the current health points for a given player
int howHealthyIs (HunterView hv, PlayerID player) {
  int health = 0;
  switch (player) {
    case 0:
      health = hv->Godalming->HP;
      break;
    case 1:
      health = hv->Seward->HP;
      break;
    case 2:
      health = hv->Helsing->HP;
      break;
    case 3:
      health = hv->Harker->HP;
      break;
    case 4:
      health = hv->Dracula->HP;
      break;
  }
  return health;
}

// Get the current location id of a given player
LocationID whereIs (HunterView hv, PlayerID player) {
  int locationID;
  switch (player) {
    case 0:
      locationID = hv->Godalming->location;
      break;
    case 1:
      locationID = hv->Seward->location;
      break;
    case 2:
      locationID = hv->Helsing->location;
      break;
    case 3:
      locationID = hv->Harker->location;
      break;
    case 4:
      if (hv->DracLocationCounter == 1) {
        locationID = hv->Dracula->location;
      } else if (PLACES[hv->Dracula->location].type == SEA && hv->DracLocationCounter == 0) {
        locationID = SEA_UNKNOWN;
      } else if (PLACES[hv->Dracula->location].type == LAND && hv->DracLocationCounter == 0) {
        locationID = CITY_UNKNOWN;
      } else if (PLACES[hv->Dracula->location].type == UNKNOWN && hv->DracLocationCounter == 0) {
        locationID = UNKNOWN_LOCATION;
      } else if (hv->Dracula->location == CASTLE_DRACULA) {
        locationID = CASTLE_DRACULA;
      } else {
        locationID = hv->Dracula->location;
      }
  }
  return locationID;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail (HunterView hv, PlayerID player, LocationID trail[TRAIL_SIZE]) {
  switch (player) {
    case 0:
      trail[0] = hv->Gtrail[0];
      trail[1] = hv->Gtrail[1];
      trail[2] = hv->Gtrail[2];
      trail[3] = hv->Gtrail[3];
      trail[4] = hv->Gtrail[4];
      trail[5] = hv->Gtrail[5];
      break;
    case 1:
      trail[0] = hv->Strail[0];
      trail[1] = hv->Strail[1];
      trail[2] = hv->Strail[2];
      trail[3] = hv->Strail[3];
      trail[4] = hv->Strail[4];
      trail[5] = hv->Strail[5];
      break;
    case 2:
      trail[0] = hv->Htrail[0];
      trail[1] = hv->Htrail[1];
      trail[2] = hv->Htrail[2];
      trail[3] = hv->Htrail[3];
      trail[4] = hv->Htrail[4];
      trail[5] = hv->Htrail[5];
      break;
    case 3:
      trail[0] = hv->Mtrail[0];
      trail[1] = hv->Mtrail[1];
      trail[2] = hv->Mtrail[2];
      trail[3] = hv->Mtrail[3];
      trail[4] = hv->Mtrail[4];
      trail[5] = hv->Mtrail[5];
      break;
    case 4:
      // TODO: BUGGY
      trail[0] = hv->Dtrail[0];
      trail[1] = hv->Dtrail[1];
      trail[2] = hv->Dtrail[2];
      trail[3] = hv->Dtrail[3];
      trail[4] = hv->Dtrail[4];
      trail[5] = hv->Dtrail[5];
      break;
  }
}

//// Functions that query the map to find information about connectivity
// TODO:
// What are my possible next moves (locations)
LocationID * whereCanIgo (HunterView hv, int *numLocations, bool road, bool rail, bool sea) {
    // REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numLocations = 0;
    return NULL;
}

// TODO:
// What are the specified player's next possible moves
LocationID * whereCanTheyGo (HunterView hv, int *numLocations, PlayerID player, bool road, bool rail, bool sea) {
    // REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    *numLocations = 0;
    return NULL;
}

