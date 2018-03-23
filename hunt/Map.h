////////////////////////////////////////////////////////////////////////
// COMP2521 18x1 ... the Fury of Dracula
// Map.h: an interface to a Map data type
//
// 2017-11-30   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>

#ifndef MAP_H_
#define MAP_H_

#include "Places.h"

typedef struct edge {
    LocationID start;
    LocationID end;
    TransportID type;
} edge;

// From Map.c
typedef struct vNode *VList;
struct vNode {
    LocationID  v;    // ALICANTE, etc
    TransportID type; // ROAD, RAIL, BOAT
    VList       next; // link to next node
};

typedef struct map *Map;
struct map {
    int nV; // #vertices
    int nE; // #edges
    VList connections[NUM_MAP_LOCATIONS]; // array of lists
};

// operations on graphs
Map newMap (void);
void disposeMap (Map);
void showMap (Map);
int numV (Map);
int numE (Map, TransportID);

#endif // !defined(MAP_H_)

