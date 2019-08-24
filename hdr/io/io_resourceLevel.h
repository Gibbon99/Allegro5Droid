#pragma once

#include <unordered_map>
#include <hdr/system/sys_physics.h>
#include "system/sys_main.h"

extern cpVect drawOffset;

//-----------------------------------------------------------------------------
//
// Loaded from disk per level
//
//-----------------------------------------------------------------------------

typedef struct {
	cpVect start;
	cpVect finish;
} _lineSegment;

//-----------------------------------------------------------------------------
//
// information needed for one lift
//
//-----------------------------------------------------------------------------

typedef struct {
	int tunnel;
	int posX;
	int posY;
} _liftBasic;

//-----------------------------------------------------------------------------
//
// structure to hold information for healing tiles
//
//-----------------------------------------------------------------------------

typedef struct {
	int    pos;                // position in array
	int    currentFrame;       // which frame are we on
	float  frameDelay;         // animation counter
	cpVect worldPos;           // Position in world coords
} _basicHealing;

typedef struct {
	bool        isAlive;
	cpFloat     angle;
	cpVect      worldPos;
	cpVect      travelDirection;
	cpVect      size;
	int         type;
	int         currentAnimFrame;
	float       animDelayCount;
	float       speed;
	int         sourceDroid;
	bool        damageDone;        // Use for disrupter to only damage once
	int         particleIndex;    // Used to remove a particle emitter
	std::string bitmapName;
//  _physicObject	bulletPhysicsObject;
} _bullet;

typedef struct {
	int                       mapVersion;
	int                       numLineSegments;
	int                       numWaypoints;
	int                       numDroids;
	int                       numLifts;
	cpVect                    levelDimensions;
	std::vector<_lineSegment> lineSegments;
	std::vector<cpVect>       wayPoints;
	std::vector<int>          tiles;
	std::vector<int>          droidTypes;
	char                      levelName[20];
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// Above here is loaded from file
	//
	int                       deckNumber;         // Use this to reference by a number
	int                       numEnemiesAlive;
	int                       numClients;         // How many remote players are on this level
	int                       currentAlertLevel;
	std::vector<_liftBasic>   lifts;
//	std::vector<_droid>       droid;

	std::vector<_basicHealing> healing;
//	std::vector<_doorTrigger>  doorTrigger;        // pointer to memory to hold door trigger information

	std::vector<_bullet> bullet;
	bool                 wallPhysicsCreated  = false;
	bool                 droidPhysicsCreated = false;
	std::vector<_physicObject> solidWalls;
	cpShapeFilter        deckShapeFilter;
	cpBitmask            deckCategory;
	cpBitmask            deckMask;
} _levelStruct;

extern std::unordered_map<std::string, _levelStruct> shipLevel;

// Populate the shipdeck structure from a file in memory
bool lvl_loadShipLevel (const std::string fileName);