////////////////////////////////////////////////////////////////////////
// COMP2521 18x1 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01   v1.1    Team Dracula <cs2521@cse.unsw.edu.au>
////////////////////////////////////////////////////////////////////////
// AUTHOR: Catherine Mai
//
////////////////////////////////////////////////////////////////////////
// TODO: Rail implementation
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "HunterView.h"
#include "hunter.h"
#include "Places.h"
#include "Map.h"

// ===== PERSONAL ADTS AND FUNCTIONS ===== //

// Location loop for PLAYER_LORD_GODALMING
static int getGodLocation (HunterView hv) {
  int nextLocation = UNKNOWN_LOCATION;
  LocationID prevLocations[TRAIL_SIZE];
  giveMeTheTrail (hv, PLAYER_LORD_GODALMING, prevLocations);
  if (giveMeTheRound (hv) == 0) {
    nextLocation = abbrevToID ("VI");
  } else if (whereIs (hv, PLAYER_LORD_GODALMING) == ST_JOSEPH_AND_ST_MARYS) {
    assert (prevLocations[0] == abbrevToID ("JM"));
    nextLocation = abbrevToID ("ZA");
  } else if (howHealthyIs (hv, PLAYER_LORD_GODALMING) > 0) {
    if (prevLocations[0] == abbrevToID ("VI")) {
      nextLocation = abbrevToID ("BD");
    } else if (prevLocations[0] == abbrevToID ("BD")) {
      nextLocation = abbrevToID ("KL");
    } else if (prevLocations[0] == abbrevToID ("KL")) {
      nextLocation = abbrevToID ("GA");
    } else if (prevLocations[0] == abbrevToID ("GA")) {
      nextLocation = abbrevToID ("CN");
    } else if (prevLocations[0] == abbrevToID ("CN")) {
      nextLocation = abbrevToID ("BS");
    } else if (prevLocations[0] == abbrevToID ("BS")) {
      nextLocation = abbrevToID ("VR");
    } else if (prevLocations[0] == abbrevToID ("VR")) {
      nextLocation = abbrevToID ("SO");
    } else if (prevLocations[0] == abbrevToID ("SO")) {
      nextLocation = abbrevToID ("SA");
    } else if (prevLocations[0] == abbrevToID ("SA")) {
      nextLocation = abbrevToID ("VA");
    } else if (prevLocations[0] == abbrevToID ("VA")) {
      nextLocation = abbrevToID ("SJ");
    } else if (prevLocations[0] == abbrevToID ("SJ")) {
      nextLocation = abbrevToID ("ZA");
    } else if (prevLocations[0] == abbrevToID ("ZA")) {
      nextLocation = abbrevToID ("VI");
    } else if (prevLocations[0] == abbrevToID ("ZA")) {
      // NOTE: COMING BACK FROM HOSPITAL
      nextLocation = abbrevToID ("VI");
    }
  } else if (howHealthyIs (hv, PLAYER_LORD_GODALMING) <= 0) {
    nextLocation = abbrevToID ("JM");
  }
  return nextLocation;
}

// Location loop for PLAYER_MINA_HARKER
static int getHarLocation (HunterView hv) {
  int nextLocation = UNKNOWN_LOCATION;
  LocationID prevLocations[TRAIL_SIZE];
  giveMeTheTrail (hv, PLAYER_MINA_HARKER, prevLocations);
  if (giveMeTheRound (hv) == 0) {
    nextLocation = abbrevToID ("SR");
  } else if (whereIs (hv, PLAYER_MINA_HARKER) == ST_JOSEPH_AND_ST_MARYS) {
    assert (prevLocations[0] == abbrevToID ("JM"));
    nextLocation = abbrevToID ("ZA");
  } else if (howHealthyIs (hv, PLAYER_MINA_HARKER) > 0) {
    if (prevLocations[0] == abbrevToID ("SR")) {
      nextLocation = abbrevToID ("SN");
    } else if (prevLocations[0] == abbrevToID ("SN")) {
      nextLocation = abbrevToID ("LS");
    } else if (prevLocations[0] == abbrevToID ("LS")) {
      nextLocation = abbrevToID ("CA");
    } else if (prevLocations[0] == abbrevToID ("CA")) {
      nextLocation = abbrevToID ("GR");
    } else if (prevLocations[0] == abbrevToID ("GR")) {
      nextLocation = abbrevToID ("AL");
    } else if (prevLocations[0] == abbrevToID ("AL")) {
      nextLocation = abbrevToID ("BA");
    } else if (prevLocations[0] == abbrevToID ("BA")) {
      nextLocation = abbrevToID ("SR");
    } else if (prevLocations[0] == abbrevToID ("ZA")) {
      // NOTE: COMING BACK FROM HOSPITAL
      nextLocation = abbrevToID ("MU");
    } else if (prevLocations[0] == abbrevToID ("MU")) {
      nextLocation = abbrevToID ("VE");
    } else if (prevLocations[0] == abbrevToID ("VE")) {
      nextLocation = abbrevToID ("FL");
    } else if (prevLocations[0] == abbrevToID ("FL")) {
      nextLocation = abbrevToID ("GO");
    } else if (prevLocations[0] == abbrevToID ("GO")) {
      nextLocation = abbrevToID ("MR");
    } else if (prevLocations[0] == abbrevToID ("MR")) {
      nextLocation = abbrevToID ("TO");
    } else if (prevLocations[0] == abbrevToID ("TO")) {
      nextLocation = abbrevToID ("SR");
    }
  } else if (howHealthyIs (hv, PLAYER_MINA_HARKER) <= 0) {
    nextLocation = abbrevToID ("JM");
  }
  return nextLocation;
}

// Location loop for PLAYER_VAN_HELSING
static int getHelLocation (HunterView hv) {
  int nextLocation = UNKNOWN_LOCATION;
  LocationID prevLocations[TRAIL_SIZE];
  giveMeTheTrail (hv, PLAYER_VAN_HELSING, prevLocations);
  if (giveMeTheRound (hv) == 0) {
    nextLocation = abbrevToID ("MU");
  } else if (whereIs (hv, PLAYER_VAN_HELSING) == ST_JOSEPH_AND_ST_MARYS) {
    assert (prevLocations[0] == abbrevToID ("JM"));
    nextLocation = abbrevToID ("ZA");
  } else if (howHealthyIs (hv, PLAYER_VAN_HELSING > 0)) {
    if (prevLocations[0] == abbrevToID ("MU")) {
      nextLocation = abbrevToID ("ZU");
    } else if (prevLocations[0] == abbrevToID ("ZU")) {
      nextLocation = abbrevToID ("ST");
    } else if (prevLocations[0] == abbrevToID ("ST")) {
      nextLocation = abbrevToID ("CO");
    } else if (prevLocations[0] == abbrevToID ("CO")) {
      nextLocation = abbrevToID ("AM");
    } else if (prevLocations[0] == abbrevToID ("AM")) {
      nextLocation = abbrevToID ("BU");
    } else if (prevLocations[0] == abbrevToID ("BU")) {
      nextLocation = abbrevToID ("LE");
    } else if (prevLocations[0] == abbrevToID ("LE")) {
      nextLocation = abbrevToID ("NA");
    } else if (prevLocations[0] == abbrevToID ("NA")) {
      nextLocation = abbrevToID ("BO");
    } else if (prevLocations[0] == abbrevToID ("BO")) {
      nextLocation = abbrevToID ("TO");
    } else if (prevLocations[0] == abbrevToID ("TO")) {
      nextLocation = abbrevToID ("MR");
    } else if (prevLocations[0] == abbrevToID ("MR")) {
      nextLocation = abbrevToID ("GO");
    } else if (prevLocations[0] == abbrevToID ("GO")) {
      nextLocation = abbrevToID ("FL");
    } else if (prevLocations[0] == abbrevToID ("FL")) {
      nextLocation = abbrevToID ("VE");
    } else if (prevLocations[0] == abbrevToID ("VE")) {
      nextLocation = abbrevToID ("MU");
    } else if (prevLocations[0] == abbrevToID ("ZA")) {
      // NOTE: COMING BACK FROM HOSPITAL
      nextLocation = abbrevToID ("MU");
    }
  } else if (howHealthyIs (hv, PLAYER_VAN_HELSING) <= 0) {
    nextLocation = abbrevToID ("JM");
  }
  return nextLocation;
}

static int getSewLocation (HunterView hv) {
  int nextLocation = UNKNOWN_LOCATION;
  LocationID prevLocations[TRAIL_SIZE];
  giveMeTheTrail (hv, PLAYER_DR_SEWARD, prevLocations);
  if (giveMeTheRound (hv) == 0) {
    nextLocation = abbrevToID ("AO");
  } else if (whereIs (hv, PLAYER_DR_SEWARD) == ST_JOSEPH_AND_ST_MARYS) {
    assert (prevLocations[0] == abbrevToID ("JM"));
    nextLocation = abbrevToID ("SJ");
  } else if (howHealthyIs (hv, PLAYER_DR_SEWARD) > 0) {
    if (prevLocations[0] == abbrevToID ("AO")) {
      nextLocation = abbrevToID ("GW");
    } else if (prevLocations[0] == abbrevToID ("GW")) {
      nextLocation = abbrevToID ("DU");
    } else if (prevLocations[0] == abbrevToID ("DU")) {
      nextLocation = abbrevToID ("IR");
    } else if (prevLocations[0] == abbrevToID ("IR")) {
      nextLocation = abbrevToID ("LV");
    } else if (prevLocations[0] == abbrevToID ("LV")) {
      nextLocation = abbrevToID ("MN");
    } else if (prevLocations[0] == abbrevToID ("MN")) {
      nextLocation = abbrevToID ("ED");
    } else if (prevLocations[0] == abbrevToID ("ED")) {
      nextLocation = abbrevToID ("NS");
    } else if (prevLocations[0] == abbrevToID ("NS")) {
      nextLocation = abbrevToID ("HA");
    } else if (prevLocations[0] == abbrevToID ("HA")) {
      nextLocation = abbrevToID ("BR");
    } else if (prevLocations[0] == abbrevToID ("BR")) {
      nextLocation = abbrevToID ("PR");
    } else if (prevLocations[0] == abbrevToID ("PR")) {
      nextLocation = abbrevToID ("NU");
    } else if (prevLocations[0] == abbrevToID ("NU")) {
      nextLocation = abbrevToID ("MU");
    } else if (prevLocations[0] == abbrevToID ("MU")) {
      nextLocation = abbrevToID ("ZA");
    } else if (prevLocations[0] == abbrevToID ("ZA")) {
      nextLocation = abbrevToID ("SJ");
    } else if (prevLocations[0] == abbrevToID ("SJ")) {
      nextLocation = abbrevToID ("VA");
    } else if (prevLocations[0] == abbrevToID ("VA")) {
      nextLocation = abbrevToID ("AT");
    } else if (prevLocations[0] == abbrevToID ("AT")) {
      nextLocation = abbrevToID ("IO");
    } else if (prevLocations[0] == abbrevToID ("IO")) {
      nextLocation = abbrevToID ("TS");
    } else if (prevLocations[0] == abbrevToID ("TS")) {
      nextLocation = abbrevToID ("CG");
    } else if (prevLocations[0] == abbrevToID ("CG")) {
      nextLocation = abbrevToID ("MS");
    } else if (prevLocations[0] == abbrevToID ("MS")) {
      nextLocation = abbrevToID ("AL");
    } else if (prevLocations[0] == abbrevToID ("AL")) {
      nextLocation = abbrevToID ("GR");
    } else if (prevLocations[0] == abbrevToID ("GR")) {
      nextLocation = abbrevToID ("CA");
    } else if (prevLocations[0] == abbrevToID ("CA")) {
      nextLocation = abbrevToID ("LS");
    } else if (prevLocations[0] == abbrevToID ("LS")) {
      nextLocation = abbrevToID ("AO");
    }
  } else if (howHealthyIs (hv, PLAYER_DR_SEWARD) <= 0) {
    nextLocation = abbrevToID ("JM");
  }
  return nextLocation;
}

// ===== ORIGINAL ADTS AND FUNCTIONS ===== //

void decideHunterMove (HunterView hv) {

    int bestLocation = UNKNOWN_LOCATION;
    int currentPlayerID = whoAmI(hv);
      switch (currentPlayerID) {
        case PLAYER_LORD_GODALMING:
          bestLocation = getGodLocation (hv);
          break;
        case PLAYER_MINA_HARKER:
          bestLocation = getHarLocation (hv);
          break;
        case PLAYER_VAN_HELSING:
          bestLocation = getHelLocation (hv);
          break;
        case PLAYER_DR_SEWARD:
          bestLocation = getSewLocation (hv);
          break;
      }

    registerBestPlay (idToAbbrev(bestLocation), "You can run but you can't hide");
}

