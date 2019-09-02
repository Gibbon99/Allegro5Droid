#pragma once

#include <unordered_map>
#include <hdr/system/sys_physics.h>
#include "system/sys_main.h"

extern b2Vec2 drawOffset;
extern b2ChainShape     solidWallChain;
typedef struct cpVect{float x,y;} cpVect;

//-----------------------------------------------------------------------------
//
// Loaded from disk per level
//
//-----------------------------------------------------------------------------

typedef struct
{
	cpVect start;
	cpVect finish;
}             _lineSegment;

//-----------------------------------------------------------------------------
//
// information needed for one lift
//
//-----------------------------------------------------------------------------

typedef struct
{
	int tunnel;
	int posX;
	int posY;
}             _liftBasic;

//-----------------------------------------------------------------------------
//
// structure to hold information for healing tiles
//
//-----------------------------------------------------------------------------

typedef struct
{
	int    pos;                // position in array
	int    currentFrame;       // which frame are we on
	float  frameDelay;         // animation counter
	b2Vec2 worldPos;           // Position in world coords
}             _basicHealing;

typedef struct
{
	int         currentMode;                // What is the droid doing; transfer, healing, terminal etc
	int         droidType;
	int         currentHealth;
	int         wayPointIndex;
	int         wayPointDirection;
	std::string spriteName;
	int         currentFrame;
	int         numberOfFrames;
	float       frameDelay;

	float currentSpeed;
	float acceleration;

	b2Vec2 worldPos;
	b2Vec2 previousWorldPos;
	b2Vec2 middlePosition;             // TODO set middle position for droids

	b2Vec2 destinationCoords;          // This is the line segment end point
	b2Vec2 destDirection;              // Which way is the droid heading
	b2Vec2 velocity;                   // Current speed

	int overTile;                   // which tile is the droid on

	b2BodyDef     bodyDef;                      // Used for physics and collisions
	b2CircleShape shape;
	b2FixtureDef  fixtureDef;
	b2Body        *body;
	float         mass;                       // Used for collision response

	bool ignoreCollisions;           // Ignoring collisions for the time period
//	bool            isExploding;

	int   targetIndex;                // Which droid shot this droid
	bool  beenShotByPlayer;
	float beenShotCountdown;

	int   collisionCount;             // how many collision have occured to ignore them
	bool  hasCollided;
	int   collidedWith;               // Who did the droid hit
	float ignoreCollisionsCounter;

	std::string playerDroidTypeDBIndex;     // What sort of droid is the player

	float chanceToShoot;

	bool visibleToPlayer;

	//
// Weapon
	bool  weaponCanFire;
	float weaponDelay;

	bool witnessShooting;
	bool witnessTransfer;

	float witnessShootingCountDown;
	float witnessTransferCountDown;

	// AI variables

	int    ai_moveMode;
	int    ai_currentState;
	// Pathfinding
	int    aStarPathIndex;            // Index into which path to use
	int    numberPathNodes;
	int    currentAStarIndex;         // Index into aStarWaypoints array
	bool   aStarDestinationFound;
	bool   aStarInitDone;
	b2Vec2 previousWaypoints;

	bool onFleeTile;
	bool foundFleeTile;

	bool isNotPatrolling;        // Used to enter resume branch
	bool onResumeDestTile;
	bool destSet;                // Does the droid have a destination

	bool onHealingTile;
	bool foundHealingTile;
	/*
b2Vec2 acceleration;

b2Vec2 screenPos;


//
// States
bool isStopped;
bool isAlive;

// Animation
float currentFrameDelay;
int currentFrame;



// AI variables
int ai_currentState;
int ai_nextState;
float ai_noActionCounter;
float ai_noActionCount;

b2Vec2 originPosition;     // Remember this to return to
bool foundOriginPath;
bool returnToOrigin;
bool foundOriginPosition;

float healingCount;
bool foundWPTile;        // Go here after healing

b2Vec2 destinationTile;        // Where is the droid heading
bool resumeDestFound;        // Set from running thread - aStar found destination
bool resumeThreadStarted;    // Is the thread started and running

*/

} _droid;

typedef struct
{
	bool        isAlive;
	float     angle;
	b2Vec2      worldPos;
	b2Vec2      previousWorldPos;
	b2Vec2      travelDirection;
	b2Vec2      size;
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

typedef struct
{
	int                       mapVersion;
	int                       numLineSegments;
	int                       numWaypoints;
	int                       numDroids;
	int                       numLifts;
	b2Vec2                    levelDimensions;
	std::vector<_lineSegment> lineSegments;
	std::vector<b2Vec2>       wayPoints;
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
	std::vector<_droid>       droid;

	std::vector<_basicHealing> healing;
//	std::vector<_doorTrigger>  doorTrigger;        // pointer to memory to hold door trigger information

	std::vector<_bullet>       bullet;
	bool                       wallPhysicsCreated  = false;
	bool                       droidPhysicsCreated = false;
} _levelStruct;

extern std::unordered_map<std::string, _levelStruct> shipLevel;

// Populate the shipdeck structure from a file in memory
bool lvl_loadShipLevel(const std::string fileName);

// Return the levelName from the passed in deckNumber
std::string lvl_returnLevelNameFromDeck(int deckNumber);

void lvl_showWayPoints(const std::string levelName);

// Return the deckNumber for the passed in level string
int lvl_getDeckNumber(const std::string levelName);

// Return the current level name
std::string lvl_getCurrentLevelName();

// Set the new level name - check it matches against the existing level names
void lvl_setCurrentLevelName(std::string newName);

// Change to a new level
void lvl_chengeToLevel(std::string newLevelName);