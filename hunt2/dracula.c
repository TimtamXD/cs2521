////////////////////////////////////////////////////////////////////////
// COMP2521 18x1 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01   v1.1    Team Dracula <cs2521@cse.unsw.edu.au>
////////////////////////////////////////////////////////////////////////
// AUTHOR: Jia Jun (Tim) Luo
// zID: z5115679
////////////////////////////////////////////////////////////////////////
// TODO: If HP is too low, discard all sea moves
////////////////////////////////////////////////////////////////////////
#include "DracView.h"
#include "Game.h"
#include "dracula.h"
#include "Places.h"
#include "Map.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// ===== PERSONAL FUNCTIONS AND ADTS ===== //

#define MAXPLACE 70
#define MINPLACE 0
#define HEALTH_CONCERN 15
#define SAFE_DISTANCE 4
#define INFINITY 999999999

// returns 1 if location is in trail.
static int assertNotInTrail (DracView dv, LocationID id) {
  int retVal = 0;
  LocationID dracTrail[TRAIL_SIZE];
  giveMeTheTrail(dv, PLAYER_DRACULA, dracTrail);
  for (int i = 0; i < TRAIL_SIZE; i++) {
    if (dracTrail[i] == id) retVal = 1;
  }
  return retVal;
}

// findShortestDistance finds the length of the shortest path between two locations
// BFS
static int findShortestDistance (LocationID id1, LocationID id2) {

  // Run BFS starting from any node - doesnt matter as it is an undirected graph.
  // length = abs (level of finishing point - level of starting point)
  Map map = newMap();

  // Variables
  VList temp = NULL;
  int i, lev, flag = 1;
  int length = 0;
  int level[numV(map) + 1];

  // Initialising
  for (i = 0; i < numV(map); i++) {
    level[i] = -1;
  }
  lev = 0;
  level[1] = lev;

  // BFS
  while (flag) {
    flag = 0;
    for (i = 1; i <= numV(map); i++) {
      if (level[i] == lev) {
        flag = 1;
        temp = map->connections[i];
        while (temp != NULL) {
          if (level[temp->v] != -1) {
            temp = temp->next;
            continue;
          }
          level[temp->v] = lev + 1;
          temp = temp->next;
        }
      }
    }
    lev++;
  }
  length = abs (level[id1] - level[id2]);
  if (length == 0) {
    // same level = length 1
    length = 1;
  }
  disposeMap(map);
  return length;
}

// getMin get's the minimum distance and returns which player dracula is closest
// to
static int getMin (int a, int b, int c, int d) {
  int retVal;
  int min_ab, min_bc, min;
  min_ab = a < b ? a : b;
  min_bc = c < d ? b : c;
  min = min_ab < min_bc ? min_ab : min_bc;
  if (min == a) retVal = PLAYER_LORD_GODALMING;
  if (min == b) retVal = PLAYER_DR_SEWARD;
  if (min == c) retVal = PLAYER_VAN_HELSING;
  if (min == d) retVal = PLAYER_MINA_HARKER;
  return retVal;
}

// Compare all entries in draculasNextLocations to location of the hunter
// with the shortest path between the hunter and draculas
// Return the locationID that has the longest length. It cannot be in
// the Dracula trail. Dracula does not take the sea if his HP falls under 10.
// Also dracula cannot visit St Joseph and St Marys
static int extendLength (DracView dv, LocationID id) {

  // Run BFS
  Map map = newMap();
  VList temp = NULL;
  int i, lev, flag = 1;
  int max = 0;
  int level[numV(map) + 1];
  int nextLocation = UNKNOWN_LOCATION;
  int numberOfPlaces, *dracNext = whereCanIgo (dv, &numberOfPlaces, true, true);

  for (i = 0; i < numV(map); i++) {
    level[i] = -1;
  }
  lev = 0;
  level[1] = lev;

  while (flag) {
    flag = 0;
    for (i = 1; i <= numV(map); i++) {
      if (level[i] == lev) {
        flag = 1;
        temp = map->connections[i];
        while (temp != NULL) {
          if (level[temp->v] != -1) {
            temp = temp->next;
            continue;
          }
          level[temp->v] = lev + 1;
          temp = temp->next;
        }
      }
    }
    lev++;
  }

  if (numberOfPlaces == 0) {
    puts ("There are no more places to go");
    abort();
  } else {
    for (i = 0; i < numberOfPlaces; i++) {
      if (abs (level[dracNext[i]] - level[id]) < max) {
        max = abs (level[dracNext[i]] - level[id]);
        if (!assertNotInTrail (dv, dracNext[i])) nextLocation = dracNext[i];
      }
    }
  }

  assert (nextLocation != ST_JOSEPH_AND_ST_MARYS);
  disposeMap (map);
  return nextLocation;
}

// goToCastle returns the location that leads dracula towards CASTLE_DRACULA
// the fastest. Cannot be in trail.
static int goToCastle (DracView dv) {

  // Run BFS
  Map map = newMap();
  VList temp = NULL;
  int i, lev, flag = 1;
  int min = INFINITY;
  int level[numV(map) + 1];
  int nextLocation = UNKNOWN_LOCATION;
  int numberOfPlaces, *dracNext = whereCanIgo (dv, &numberOfPlaces, true, true);

  for (i = 0; i < numV(map); i++) {
    level[i] = -1;
  }
  lev = 0;
  level[1] = lev;

  while (flag) {
    flag = 0;
    for (i = 1; i <= numV(map); i++) {
      if (level[i] == lev) {
        flag = 1;
        temp = map->connections[i];
        while (temp != NULL) {
          if (level[temp->v] != -1) {
            temp = temp->next;
            continue;
          }
          level[temp->v] = lev + 1;
          temp = temp->next;
        }
      }
    }
    lev++;
  }

  if (numberOfPlaces == 0) {
    puts ("There are no more places to go");
    abort();
  } else {
    for (i = 0; i < numberOfPlaces; i++) {
      if (abs (level[dracNext[i]] - level[CASTLE_DRACULA]) < min) {
        min = abs (level[dracNext[i]] - level[CASTLE_DRACULA]);
        if (!assertNotInTrail (dv, dracNext[i])) nextLocation = dracNext[i];
      }
    }
  }

  assert (nextLocation != ST_JOSEPH_AND_ST_MARYS);
  assert (nextLocation != UNKNOWN_LOCATION);
  disposeMap (map);
  return nextLocation;
}

// ===== ORIGINAL FUNCTIONS AND ADTS ===== //

void decideDraculaMove (DracView dv) {

    // Variables
    int bestLocation = UNKNOWN_LOCATION;

    // Begin:
    if (giveMeTheRound (dv) == 0) {
      bestLocation = rand() % (MAXPLACE + 1 - MINPLACE) + MINPLACE;
      if (bestLocation == ST_JOSEPH_AND_ST_MARYS || bestLocation == CASTLE_DRACULA) {
        bestLocation = bestLocation + 1;
      }
    } else {

      // 1. Get current location of everyone

      // 2. Use whereCanIgo and whereCanTheyGo to figure out possible moves

      // 3. BFS
      // - Find length between current Dracula's position vs all current Hunter's positions.
      int distBetDracGod = findShortestDistance (whereIs (dv, PLAYER_DRACULA),
      whereIs (dv, PLAYER_LORD_GODALMING));
      int distBetDracSew = findShortestDistance (whereIs (dv, PLAYER_DRACULA),
      whereIs (dv, PLAYER_DR_SEWARD));
      int distBetDracHel = findShortestDistance (whereIs (dv, PLAYER_DRACULA),
      whereIs (dv, PLAYER_VAN_HELSING));
      int distBetDracHar = findShortestDistance (whereIs (dv, PLAYER_DRACULA),
      whereIs (dv, PLAYER_MINA_HARKER));

      // 4. Continuation...
      // - Get the shortest length and try to extend that by comparing all
      // - whereCanIgo locations to that specific hunter. Get the location that
      // - extendes the overall length between all hunters.
      if (whereIs (dv, PLAYER_DRACULA) != CASTLE_DRACULA) {
        if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
         == PLAYER_LORD_GODALMING) {
           if (howHealthyIs (dv, PLAYER_DRACULA > HEALTH_CONCERN)) {
             bestLocation = extendLength (dv, whereIs (dv, PLAYER_LORD_GODALMING));
           } else {
             bestLocation = goToCastle(dv);
           }
        } else if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
         == PLAYER_DR_SEWARD) {
           if (howHealthyIs (dv, PLAYER_DRACULA > HEALTH_CONCERN)) {
             bestLocation = extendLength (dv, whereIs (dv, PLAYER_DR_SEWARD));
           } else {
             bestLocation = goToCastle(dv);
           }
        } else if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
         == PLAYER_VAN_HELSING) {
           if (howHealthyIs (dv, PLAYER_DRACULA > HEALTH_CONCERN)) {
             bestLocation = extendLength (dv, whereIs (dv, PLAYER_VAN_HELSING));
           } else {
             bestLocation = goToCastle(dv);
           }
        } else if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
         == PLAYER_MINA_HARKER) {
           if (howHealthyIs (dv, PLAYER_DRACULA > HEALTH_CONCERN)) {
             bestLocation = extendLength (dv, whereIs (dv, PLAYER_MINA_HARKER));
           } else {
             bestLocation = goToCastle(dv);
           }
        }
      } else {
        if (distBetDracGod > SAFE_DISTANCE && distBetDracSew > SAFE_DISTANCE &&
        distBetDracHel > SAFE_DISTANCE && distBetDracHar > SAFE_DISTANCE &&
        howHealthyIs (dv, PLAYER_DRACULA) < GAME_START_BLOOD_POINTS) {
          bestLocation = CASTLE_DRACULA;
        } else {
          if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
           == PLAYER_LORD_GODALMING) {
            bestLocation = extendLength (dv, whereIs (dv, PLAYER_LORD_GODALMING));
          } else if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
           == PLAYER_DR_SEWARD) {
            bestLocation = extendLength (dv, whereIs (dv, PLAYER_DR_SEWARD));
          } else if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
           == PLAYER_VAN_HELSING) {
            bestLocation = extendLength (dv, whereIs (dv, PLAYER_VAN_HELSING));
          } else if (getMin (distBetDracGod, distBetDracSew, distBetDracHel, distBetDracHar)
           == PLAYER_MINA_HARKER) {
            bestLocation = extendLength (dv, whereIs (dv, PLAYER_MINA_HARKER));
          }
        }
      }
    }
    registerBestPlay(idToAbbrev(bestLocation), "Leave me alone you dogs.");
}

/* NOTE: These next few lines are just for submission's sake.
void decideDraculaMove (DracView dv) {
    // NOTE: Dodgy implementation:
    int bestLocation = UNKNOWN_LOCATION;
    if (giveMeTheRound (dv) == 0) {
      bestLocation = rand() % (MAXPLACE + 1 - MINPLACE) + MINPLACE;
      if (bestLocation == ST_JOSEPH_AND_ST_MARYS || bestLocation == CASTLE_DRACULA) {
        bestLocation = bestLocation + 1;
      }
    } else {
      int drac04, *drac04E = whereCanIgo (dv, &drac04, PLAYER_DRACULA,
        true, true);
      bestLocation = drac04E[0];
    }
    registerBestPlay(idToAbbrev(bestLocation), "Leave me alone you dogs.");
}
*/

