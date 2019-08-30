#pragma once

#include "system/sys_main.h"

const int	AI_NODE_SEQUENCE 		= 1;	// Continue until one fails
const int	AI_NODE_SELECTOR 		= 2;	// Stop on success
const int	AI_NODE_EXECUTE 		= 3;	// Run this node for a result
const int	AI_NODE_ALWAYS_SUCCEED	= 4;	// Always return AI_RESULT_SUCCEED;

const int	AI_RESULT_INVALID		= 0;
const int	AI_RESULT_SUCCESS		= 1;
const int	AI_RESULT_FAILED		= 2;
const int	AI_RESULT_RUNNING		= 3;

const int	TILE_TYPE_HEALING		= 1;
const int	TILE_TYPE_NEAREST_WP 	= 2;
const int	TILE_TYPE_FLEE			= 3;

#define		AI_PATROL			   	0
#define		AI_PATHFIND			   	1	// path find to a location
#define		AI_ATTACK			   	2
#define		AI_FLEE				   	3
#define		AI_PATHFIND_START	   	4
#define		AI_PATHFIND_END			5
#define		AI_MODE_CONTINUE_PATH	6
//#define		AI_WAYPOINT_UP		   	6
//#define		AI_WAYPOINT_DOWN	   	7
#define		AI_NO_ACTION		   	8
#define     AI_FOLLOW_PATH			9     // Follow the path found from aStar
#define     AI_CHECK_HEALTH			10
#define     AI_EVAL_NEXT_OPTION  	11

#define     AI_STATE_PATROL			0
#define     AI_STATE_HEALTH			1

#define 	AI_MODE_WAYPOINT		0
#define 	AI_MODE_ASTAR			1

#define WAYPOINT_UP         0x01
#define WAYPOINT_DOWN         0x02

#define IGNORE_COLLISION_TIME   5

typedef struct
{
	std::string			nodeName;
	int				nodeType;
	int				functionResult;
	int				parentIndex;
	bool			hasChildren;
	int				numChildren;
	int				childVisitedCounter;
	std::vector<int>		childIndex;
	functionPtr		functionName;
} _aiNode;

extern	std::vector<_aiNode>		aiTree;
extern bool					backToAIRoot;


//--------------------------------------------------------------------
//
// Functions for aiTree ( Behaviour Tree )
//
//--------------------------------------------------------------------

// Setup the nodes for the AI behavioural tree
void ai_setupAITree();

// Walk the aiTree
void ai_walkTree(std::string startNode, int whichDroid, std::string whichLevel);

// Debug the aiTree
void ai_debugTree(std::string startNode);

// Add a child node to the named parent node
bool ai_addNode(int nodeType, std::string parentNode, std::string nodeName, functionPtr functionPtr);

extern float	wayPointDestinationSize;
extern float	minChangeDirectionSpeed;
extern bool 	useDroidAcceleration;

extern float		witnessShootValue;
extern float		witnessTransferValue;

extern float		ai_beenShot;
extern float		ai_healthAmount;
extern float		ai_playerVisible;
extern float		ai_witnessShoot;
extern float		ai_witnessTransfer;
extern float		ai_greenFactor;
extern float		ai_yellowFactor;
extern float		ai_redFactor;

extern float		droidBeenShotValue;
extern int          collisionExplosionDamage;
extern int          collisionDamageInflicted;
extern float        badHealthFactor;

// Process the AI for each enemy droid
void drd_processDroidAI ( std::string levelName);

// Clear the hasCollided flag before running physics
void drd_clearHadCollidedFlag();

cpVect ai_findNearestTile ( int whichDroid, int tileType, std::string levelName );



