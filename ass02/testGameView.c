////////////////////////////////////////////////////////////////////////
// COMP2521 18x1 ... the Fury of Dracula
// testGameView.c: test the GameView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01   v1.1    Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GameView.h"

int
main (int argc, char *argv[])
{
    do {////////////////////////////////////////////////////////////////
        puts ("Test basic empty initialisation");

        char *trail = "";
        PlayerMessage messages[] = {};
        GameView gv = newGameView (trail, messages);

        assert (getCurrentPlayer (gv) == PLAYER_LORD_GODALMING);
        assert (getRound (gv) == 0);
        assert (getHealth (gv, PLAYER_DR_SEWARD) ==
                GAME_START_HUNTER_LIFE_POINTS);
        assert (getHealth (gv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS);
        assert (getScore (gv) == GAME_START_SCORE);
        assert (getLocation (gv, PLAYER_LORD_GODALMING) ==
                UNKNOWN_LOCATION);

        puts ("passed");
        disposeGameView (gv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for Dracula trail and basic functions");

        char *trail =
            "GST.... SAO.... HZU.... MBB.... DC?....";
        PlayerMessage messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah"};
        GameView gv = newGameView (trail, messages);
        
        assert (getCurrentPlayer (gv) == PLAYER_LORD_GODALMING);
        assert (getRound (gv) == 1);
        assert (getLocation (gv, PLAYER_LORD_GODALMING) == STRASBOURG);
        assert (getLocation (gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
        assert (getLocation (gv, PLAYER_VAN_HELSING) == ZURICH);
        assert (getLocation (gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
        assert (getLocation (gv, PLAYER_DRACULA) == CITY_UNKNOWN);
        assert (getHealth (gv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS);

        puts ("passed");
        disposeGameView (gv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for encountering Dracula and hunter history");

        char *trail =
            "GST.... SAO.... HCD.... MAO.... DGE.... "
            "GGED...";
        PlayerMessage messages[] = {
            "Hello", "Rubbish",  "Stuff", "", "Mwahahah",
            "Aha!"};
        GameView gv = newGameView (trail, messages);

        assert (getLocation (gv, PLAYER_DRACULA) == GENEVA);
        assert (getHealth (gv, PLAYER_LORD_GODALMING) == 5);
        assert (getHealth (gv, PLAYER_DRACULA) == 30);
        assert (getLocation (gv, PLAYER_LORD_GODALMING) == GENEVA);
        LocationID history[TRAIL_SIZE];
        getHistory (gv, PLAYER_DRACULA, history);
        assert (history[0] == GENEVA);
        assert (history[1] == UNKNOWN_LOCATION);

        getHistory (gv, PLAYER_LORD_GODALMING, history);
        assert (history[0] == GENEVA);
        assert (history[1] == STRASBOURG);
        assert (history[2] == UNKNOWN_LOCATION);

        getHistory (gv, PLAYER_DR_SEWARD, history);
        assert (history[0] == ATLANTIC_OCEAN);
        assert (history[1] == UNKNOWN_LOCATION);

        puts ("passed");
        disposeGameView (gv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for Dracula doubling back at sea, "
              "and losing blood points (Hunter View)");

        char *trail =
            "GGE.... SGE.... HGE.... MGE.... DS?.... "
            "GST.... SST.... HST.... MST.... DD1....";
        PlayerMessage messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah",
            "Aha!", "", "", "", "Back I go"};
        GameView gv = newGameView (trail, messages);

        assert (getCurrentPlayer (gv) == 0);
        assert (getHealth (gv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS - 4);
        assert (getLocation (gv, PLAYER_DRACULA) == DOUBLE_BACK_1);

        LocationID history[TRAIL_SIZE];
        getHistory (gv, PLAYER_DRACULA, history);
        assert (history[0] == DOUBLE_BACK_1);
        assert (history[1] == SEA_UNKNOWN);

        puts ("passed");
        disposeGameView (gv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for Dracula doubling back at sea, "
              "and losing blood points (Drac View)");

        char *trail =
            "GGE.... SGE.... HGE.... MGE.... DEC.... "
            "GST.... SST.... HST.... MST.... DD1....";
        PlayerMessage messages[] = {
            "Hello", "Rubbish", "Stuff", "", "Mwahahah",
            "Aha!", "", "", "", "Back I go"};
        GameView gv = newGameView (trail, messages);

        assert (getCurrentPlayer (gv) == 0);
        assert (getHealth (gv, PLAYER_DRACULA) ==
                GAME_START_BLOOD_POINTS - 4);
        assert (getLocation (gv, PLAYER_DRACULA) == DOUBLE_BACK_1);

        LocationID history[TRAIL_SIZE];
        getHistory (gv, PLAYER_DRACULA, history);
        assert (history[0] == DOUBLE_BACK_1);
        assert (history[1] == ENGLISH_CHANNEL);

        puts ("passed");
        disposeGameView (gv);
    } while (0);


    do {////////////////////////////////////////////////////////////////
        puts ("Test for connections");
        char *trail = "";
        PlayerMessage messages[] = {};
        GameView gv = newGameView (trail, messages);

        do {
            puts ("Checking Galatz road connections");
            int size, *edges = connectedLocations (gv, &size,
                GALATZ, PLAYER_LORD_GODALMING, 0, true, false, false);

            bool seen[NUM_MAP_LOCATIONS] = {false};
            for (int i = 0; i < size; i++)
                seen[edges[i]] = true;

            assert (size == 5);
            assert (seen[GALATZ]);
            assert (seen[CONSTANTA]);
            assert (seen[BUCHAREST]);
            assert (seen[KLAUSENBURG]);
            assert (seen[CASTLE_DRACULA]);

            free (edges);
        } while (0);

        do {
            puts ("Checking Ionian Sea sea connections");
            int size, *edges = connectedLocations (gv, &size,
                IONIAN_SEA, PLAYER_LORD_GODALMING, 0,
                false, false, true);

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

            free (edges);
        } while (0);

        do {
            puts ("Checking Athens rail connections (none)");
            int size, *edges = connectedLocations (gv, &size,
                ATHENS, PLAYER_LORD_GODALMING, 0,
                false, true, false);

            assert (size == 1);
            assert (edges[0] == ATHENS);

            free (edges);
        } while (0);

        puts ("passed");
        disposeGameView (gv);
    } while (0);

    return EXIT_SUCCESS;
}
