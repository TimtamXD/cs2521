////////////////////////////////////////////////////////////////////////
// COMP2521 18x1 ... the Fury of Dracula
// testHunterView.c: test the HunterView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-02   v1.1    Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HunterView.h"

int
main (int argc, char *argv[])
{
    do {////////////////////////////////////////////////////////////////
        puts ("Test basic empty initialisation");
        char *trail = "";
        PlayerMessage messages[] = {};
        HunterView hv = newHunterView (trail, messages);

        assert (whoAmI (hv) == PLAYER_LORD_GODALMING);
        assert (giveMeTheRound (hv) == 0);
        assert (howHealthyIs (hv, PLAYER_DR_SEWARD) ==
                GAME_START_HUNTER_LIFE_POINTS);
        assert (howHealthyIs (hv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS);
        assert (giveMeTheScore (hv) == GAME_START_SCORE);
        assert (whereIs (hv, PLAYER_LORD_GODALMING) ==
                UNKNOWN_LOCATION);

        puts ("passed");
        disposeHunterView (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for Dracula trail and basic functions");

        char *trail =
            "GST.... SAO.... HZU.... MBB.... DC?....";
        PlayerMessage messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah" };
        HunterView hv = newHunterView (trail, messages);

        assert (whoAmI (hv) == PLAYER_LORD_GODALMING);
        assert (giveMeTheRound (hv) == 1);
        assert (whereIs (hv, PLAYER_LORD_GODALMING) == STRASBOURG);
        assert (whereIs (hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
        assert (whereIs (hv, PLAYER_VAN_HELSING) == ZURICH);
        assert (whereIs (hv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
        assert (whereIs (hv, PLAYER_DRACULA) == CITY_UNKNOWN);
        assert (howHealthyIs (hv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS);

        puts ("passed");
        disposeHunterView (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for encountering Dracula and hunter history");
        char *trail =
            "GST.... SAO.... HCD.... MAO.... DGE.... "
            "GGED...";
        PlayerMessage messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah",
            "Aha!" };
        HunterView hv = newHunterView (trail, messages);

        assert (whereIs (hv, PLAYER_DRACULA) == GENEVA);
        assert (howHealthyIs (hv, PLAYER_LORD_GODALMING) == 5);
        assert (howHealthyIs (hv, PLAYER_DRACULA) == 30);
        assert (whereIs (hv, PLAYER_LORD_GODALMING) == GENEVA);

        LocationID history[TRAIL_SIZE];
        giveMeTheTrail (hv, PLAYER_DRACULA, history);
        assert (history[0] == GENEVA);
        assert (history[1] == UNKNOWN_LOCATION);

        giveMeTheTrail (hv, PLAYER_LORD_GODALMING, history);
        assert (history[0] == GENEVA);
        assert (history[1] == STRASBOURG);
        assert (history[2] == UNKNOWN_LOCATION);

        giveMeTheTrail (hv, PLAYER_DR_SEWARD, history);
        assert (history[0] == ATLANTIC_OCEAN);
        assert (history[1] == UNKNOWN_LOCATION);

        puts ("passed");
        disposeHunterView (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for Dracula doubling back at sea, "
              "and losing blood points (Hunter View)");
        char *trail =
            "GGE.... SGE.... HGE.... MGE.... DS?.... "
            "GST.... SST.... HST.... MST.... DD1....";
        PlayerMessage messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah",
            "Aha!", "", "", "", "Back I go" };
        HunterView hv = newHunterView (trail, messages);

        assert (whoAmI (hv) == 0);
        assert (whereIs (hv, PLAYER_DRACULA) == DOUBLE_BACK_1);
        assert (howHealthyIs (hv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS - 4);

        LocationID history[TRAIL_SIZE];
        giveMeTheTrail (hv, PLAYER_DRACULA, history);
        assert (history[0] == DOUBLE_BACK_1);
        assert (history[1] == SEA_UNKNOWN);

        puts ("passed");
        disposeHunterView (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Checking Galatz road connections");
        char *trail = "GGA....";
        PlayerMessage messages[] = {"Gone to Galatz"};
        HunterView hv = newHunterView (trail, messages);

        int size, *edges = whereCanTheyGo (
            hv, &size, PLAYER_LORD_GODALMING, true, false, false);
        bool seen[NUM_MAP_LOCATIONS] = {false};
        for (int i = 0; i < size; i++)
            seen[edges[i]] = true;

        assert (size == 5);
        assert (seen[GALATZ]);
        assert (seen[CONSTANTA]);
        assert (seen[BUCHAREST]);
        assert (seen[KLAUSENBURG]);
        assert (seen[CASTLE_DRACULA]);

        puts ("passed");
        free (edges);
        disposeHunterView (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Checking Ionian Sea sea connections");
        char *trail = "GIO....";
        PlayerMessage messages[] = {"Sailing the Ionian"};
        HunterView hv = newHunterView (trail, messages);

        int size, *edges = whereCanTheyGo (
            hv, &size, PLAYER_LORD_GODALMING, false, false, true);
        bool seen[NUM_MAP_LOCATIONS] = {false};
        for (int i = 0; i < size; i++)
            seen[edges[i]] = true;

        assert (size == 7);
        assert (seen[IONIAN_SEA]);
        assert (seen[BLACK_SEA]);
        assert (seen[ADRIATIC_SEA]);
        assert (seen[TYRRHENIAN_SEA]);
        assert (seen[ATHENS]);
        assert (seen[VALONA]);
        assert (seen[SALONICA]);

        puts ("passed");
        free (edges);
        disposeHunterView (hv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Checking Athens rail connections (none)");

        char *trail = "GAT....";
        PlayerMessage messages[] = {"Leaving Athens by train"};
        HunterView hv = newHunterView (trail, messages);

        int size, *edges = whereCanTheyGo (
            hv, &size, PLAYER_LORD_GODALMING, false, true, false);

        assert (size == 1);
        assert (edges[0] == ATHENS);

        puts ("passed");
        free (edges);
        disposeHunterView (hv);
    } while (0);

    return EXIT_SUCCESS;
}
