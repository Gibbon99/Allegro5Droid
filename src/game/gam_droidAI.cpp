#include <hdr/game/gam_droidAIShoot.h>
#include <hdr/game/gam_droidAIHealth.h>
#include <hdr/game/gam_droidAIFlee.h>
#include <hdr/game/gam_droidAIResume.h>
#include <hdr/game/gam_droidAIPatrol.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_droids.h>
#include "hdr/game/gam_droidAI.h"

float droidBeenShotValue;
int   collisionExplosionDamage;
int   collisionDamageInflicted;
float badHealthFactor;

//#define AI_DEBUG_TREE 1

std::vector<_aiNode> aiTree;
int                  currentAINode = 0;
bool                 backToAIRoot  = false;

//-----------------------------------------------------------------------------
//
// Setup the nodes for the AI behavioural tree
void ai_setupAITree ()
//-----------------------------------------------------------------------------
{
	_aiNode tempNode;

	tempNode.parentIndex         = -1;            // Root node
	tempNode.nodeType            = AI_NODE_SEQUENCE; // Run until one fails
	tempNode.nodeName            = "ROOT_NODE";      // First node
	tempNode.numChildren         = 0;
	tempNode.childVisitedCounter = 0;
	tempNode.functionResult      = AI_RESULT_INVALID;

	aiTree.push_back (tempNode); // Add root node to start with
//
// See if the droid is ready to shoot at player
//
	ai_addNode (AI_NODE_SELECTOR, "ROOT_NODE", "AI_SHOOT", nullptr);
	ai_addNode (AI_NODE_EXECUTE, "AI_SHOOT", "NOT_SHOOT", (functionPtr) &ai_notShoot);
	ai_addNode (AI_NODE_EXECUTE, "AI_SHOOT", "BULLET_DEST", (functionPtr) &ai_findBulletDest);
	ai_addNode (AI_NODE_EXECUTE, "AI_SHOOT", "FIRE_BULLET", (functionPtr) &ai_shootBullet);

//
// If health is low, find a way to the nearest healing tile and repair
//
	ai_addNode (AI_NODE_SELECTOR, "ROOT_NODE", "AI_HEALTHY", nullptr);
	ai_addNode (AI_NODE_EXECUTE, "AI_HEALTHY", "CHECK_HEALTH", (functionPtr) &ai_isDroidHealthy);
	ai_addNode (AI_NODE_EXECUTE, "AI_HEALTHY", "ON_HEALING", (functionPtr) &ai_onHealingTile);
	ai_addNode (AI_NODE_EXECUTE, "AI_HEALTHY", "REACHED_HEALTH", (functionPtr) &ai_reachedHealingTile);
//
// If health is low, and there are no healing tile, flee from the player
//
	ai_addNode (AI_NODE_SELECTOR, "ROOT_NODE", "AI_FLEE", nullptr);
	ai_addNode (AI_NODE_EXECUTE, "AI_FLEE", "CHECK_FLEE", (functionPtr) &ai_isDroidHealthyFlee);
	ai_addNode (AI_NODE_EXECUTE, "AI_FLEE", "ON_FLEE", (functionPtr) &ai_onFleeTile);
	ai_addNode (AI_NODE_EXECUTE, "AI_FLEE", "REACHED_FLEE", (functionPtr) &ai_reachedFleeTile);

//
// Find a way back to the nearest waypoint to continue patrol
//
	ai_addNode (AI_NODE_SELECTOR, "ROOT_NODE", "AI_RESUME", nullptr);
	ai_addNode (AI_NODE_EXECUTE, "AI_RESUME", "IS_NOT_PATROLLING", (functionPtr) &ai_isNotPatrolling);
	ai_addNode (AI_NODE_EXECUTE, "AI_RESUME", "SET_DEST", (functionPtr) &ai_onResumeDest);
	ai_addNode (AI_NODE_EXECUTE, "AI_RESUME", "FIND_DEST_PATH", (functionPtr) &ai_reachedResumeDest);

//
// Standard patrol waypoints behaviour
//
	ai_addNode (AI_NODE_SEQUENCE, "ROOT_NODE", "AI_PATROL", nullptr);
	ai_addNode (AI_NODE_SELECTOR, "AI_PATROL", "AI_MOVE", nullptr);
	ai_addNode (AI_NODE_EXECUTE, "AI_MOVE", "CAN_MOVE", (functionPtr) &ai_canMove);
	ai_addNode (AI_NODE_EXECUTE, "AI_MOVE", "AI_REVERSE", (functionPtr) &ai_canReverseDirection);
	ai_addNode (AI_NODE_EXECUTE, "AI_PATROL", "MOVE_TO", (functionPtr) &ai_moveDroidToWaypoint);
	ai_addNode (AI_NODE_EXECUTE, "AI_PATROL", "FIND_WP", (functionPtr) &ai_getNextWaypoint);
}

//-----------------------------------------------------------------------------
//
// Find the index of the named node
int ai_findNode (std::string nodeName)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	for (auto &it : aiTree)
	{
		if (it.nodeName == nodeName)
		{
			return indexCount;
		}

		indexCount++;
	}

	return -2;
}

//-----------------------------------------------------------------------------
//
// Add a child node to the named parent node
bool ai_addNode (int nodeType, const std::string parentNode, std::string nodeName, functionPtr functionPtr)
//-----------------------------------------------------------------------------
{
	int     nodeIndex;
	_aiNode tempAINode;

	nodeIndex = ai_findNode (parentNode);

	if (-1 == nodeIndex)
	{
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Error: Couldn't find node in tree"));
		return false;
	}

	tempAINode.nodeType            = nodeType;
	tempAINode.nodeName            = nodeName;
	tempAINode.functionName        = functionPtr;
	tempAINode.functionResult      = AI_RESULT_INVALID;
	tempAINode.parentIndex         = nodeIndex;
	tempAINode.childVisitedCounter = 0;
	tempAINode.numChildren         = 0;

	aiTree.push_back (tempAINode);

	aiTree[nodeIndex].numChildren++;
	aiTree[nodeIndex].childIndex.push_back (ai_findNode (nodeName));

	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Node [ %s ] added. Num Children for [ %s ] now [ %i ]", nodeName.c_str (), aiTree[nodeIndex].nodeName.c_str (), aiTree[nodeIndex].numChildren));

	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Child index set to [ %i ]", ai_findNode (nodeName)));

	return true;
}

//-----------------------------------------------------------------------------
//
// Get a text for the current AI_RESULT_ code
std::string ai_getResultText (int whichCode)
//-----------------------------------------------------------------------------
{
	switch (whichCode)
	{
		case AI_RESULT_FAILED:
			return ("AI_RESULT_FAILED");
			break;

		case AI_RESULT_RUNNING:
			return ("AI_RESULT_RUNNING");
			break;

		case AI_RESULT_SUCCESS:
			return ("AI_RESULT_SUCCESS");
			break;

		case AI_RESULT_INVALID:
			return ("AI_RESULT_INVALID");
			break;
	}

	return ("Unknown node");
}

//-----------------------------------------------------------------------------
//
// Test to see if all the child nodes have been visited for this node
//
bool ai_visitedAllChildNodes (int whichNode)
//-----------------------------------------------------------------------------
{
	if (-1 == whichNode)
		return false;

	if (aiTree[whichNode].childVisitedCounter == aiTree[whichNode].numChildren)
	{
		aiTree[whichNode].childVisitedCounter = 0;

		if (-1 == aiTree[currentAINode].parentIndex)
		{
			aiTree[0].functionResult = aiTree[whichNode].functionResult;
			aiTree[0].functionResult = AI_RESULT_INVALID;
		}
		else
		{
			aiTree[aiTree[whichNode].parentIndex].functionResult = aiTree[whichNode].functionResult;
			aiTree[whichNode].functionResult                     = AI_RESULT_INVALID;
		}

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//
// Check if this is the root node, and see if it has finished processing
//
bool ai_checkRootNode (int whichNode)
//-----------------------------------------------------------------------------
{
	if (whichNode > 0)
		return false;

	if (whichNode == -1)
		return true;

	if (aiTree[0].childVisitedCounter == aiTree[0].numChildren)
	{
		aiTree[0].childVisitedCounter = 0;

		switch (aiTree[0].nodeType)
		{
			case AI_NODE_SEQUENCE:
				if ((aiTree[0].functionResult == AI_RESULT_FAILED) || (aiTree[0].functionResult == AI_RESULT_RUNNING))
				{
					aiTree[0].functionResult = AI_RESULT_INVALID;
					backToAIRoot = true;
					return true;
				}
				else
				{
					return false;
				}

				break;

			case AI_NODE_SELECTOR:
				if ((aiTree[0].functionResult == AI_RESULT_SUCCESS) || (aiTree[0].functionResult == AI_RESULT_RUNNING))
				{
					aiTree[0].functionResult = AI_RESULT_INVALID;
					backToAIRoot = true;
					return true;
				}
				else
				{
					return false;
				}

				break;

			default:
				log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Error: Invalid nodeType passed to ai_checkRootNode"));
				break;
		}
	}
	else
		return false;

	//
	// Keep compiler happy
	return false;
}

//-----------------------------------------------------------------------------
//
// Walk the aiTree
void ai_walkTree (std::string startNode, int whichDroid, std::string whichLevel)
//-----------------------------------------------------------------------------
{
	int whileFunctionResult = AI_RESULT_INVALID;

	if (true == backToAIRoot)
		return;

	if (-1 == currentAINode)
	{
		backToAIRoot = true;
		return;
	}

	currentAINode = ai_findNode (startNode);

	if (-2 == currentAINode)
	{
		//			con_print(true, false, "Error: Couldn't find node [ %s ]", startNode.c_str());
		return;
	}

#ifdef AI_DEBUG_TREE
	log_logMessage(LOG_LEVEL_INFO, sys_getString("[ %i ] - Entering node [ %i ] [ %s ] - Status [ %s ] childCount [ %i ] numChild [ %i ]",
				whichDroid,
				currentAINode,
				aiTree[currentAINode].nodeName.c_str (),
				ai_getResultText ( aiTree[currentAINode].functionResult ).c_str (),
				aiTree[currentAINode].childVisitedCounter,
				aiTree[currentAINode].numChildren ));
#endif

	if (true == ai_visitedAllChildNodes (currentAINode))
	{
		if (aiTree[currentAINode].parentIndex < 0)
			return;
		else
			ai_walkTree (aiTree[aiTree[currentAINode].parentIndex].nodeName, whichDroid, whichLevel);

		return;
	}

	if (true == ai_checkRootNode (currentAINode))
		return;

	switch (aiTree[currentAINode].nodeType)
	{

		case AI_NODE_ALWAYS_SUCCEED:
			if (aiTree[currentAINode].parentIndex < 0)
			{
				aiTree[currentAINode].functionResult = AI_RESULT_INVALID;
				backToAIRoot = true;
				return;
			}

			aiTree[currentAINode].functionResult                     = AI_RESULT_SUCCESS;
			aiTree[aiTree[currentAINode].parentIndex].functionResult = AI_RESULT_SUCCESS;
			ai_walkTree (aiTree[aiTree[currentAINode].parentIndex].nodeName, whichDroid, whichLevel);
			return;
			break;

		case AI_NODE_SEQUENCE: // stop on failure

			if ((aiTree[currentAINode].functionResult == AI_RESULT_FAILED) || (aiTree[currentAINode].functionResult == AI_RESULT_RUNNING))
			{
				aiTree[currentAINode].childVisitedCounter = 0;

				if (aiTree[currentAINode].parentIndex < 0)
				{
					aiTree[currentAINode].functionResult = AI_RESULT_INVALID;
					backToAIRoot = true;
					return;
				}

				aiTree[aiTree[currentAINode].parentIndex].functionResult = aiTree[currentAINode].functionResult;
				aiTree[currentAINode].functionResult                     = AI_RESULT_INVALID;
				ai_walkTree (aiTree[aiTree[currentAINode].parentIndex].nodeName, whichDroid, whichLevel);
				return;
			}

			break;

		case AI_NODE_SELECTOR:

			if ((aiTree[currentAINode].functionResult == AI_RESULT_SUCCESS) || (aiTree[currentAINode].functionResult == AI_RESULT_RUNNING))
			{
				aiTree[currentAINode].childVisitedCounter = 0;

				if (aiTree[currentAINode].parentIndex < 0)
				{
					aiTree[currentAINode].functionResult = AI_RESULT_INVALID;
					backToAIRoot = true;
					return;
				}

				aiTree[aiTree[currentAINode].parentIndex].functionResult = aiTree[currentAINode].functionResult;
				aiTree[currentAINode].functionResult                     = AI_RESULT_INVALID;
				ai_walkTree (aiTree[aiTree[currentAINode].parentIndex].nodeName, whichDroid, whichLevel);
				return;
			}

			break;
	}

	if (aiTree[aiTree[currentAINode].childIndex[aiTree[currentAINode].childVisitedCounter]].nodeType == AI_NODE_EXECUTE)
	{
#ifdef AI_DEBUG_TREE
		log_logMessage(LOG_LEVEL_INFO, sys_getString(
			"[ %i ] - In node [ %s ] - run function node [ %s ]",
			whichDroid,
			aiTree[currentAINode].nodeName.c_str (),
			aiTree[aiTree[currentAINode].childIndex[aiTree[currentAINode].childVisitedCounter]].nodeName.c_str () ));
#endif
		int childIndex = aiTree[currentAINode].childVisitedCounter;
		whileFunctionResult = aiTree[aiTree[currentAINode].childIndex[childIndex]].functionName (whichDroid, whichLevel); // Run and return result

		if (aiTree[currentAINode].nodeType == AI_NODE_ALWAYS_SUCCEED)
			aiTree[currentAINode].functionResult = AI_RESULT_SUCCESS;
		else
			aiTree[currentAINode].functionResult = whileFunctionResult;

		aiTree[currentAINode].childVisitedCounter++;
		ai_walkTree (aiTree[currentAINode].nodeName, whichDroid, whichLevel);
		return;
	}
	else
	{
		int childIndex = aiTree[currentAINode].childVisitedCounter;
		aiTree[currentAINode].childVisitedCounter++;

		int childPtr = aiTree[currentAINode].childIndex[childIndex];
		ai_walkTree (aiTree[childPtr].nodeName, whichDroid, whichLevel);
		return;
	}

	return;
}

//-----------------------------------------------------------------------------
//
// Debug the aiTree
void ai_debugTree (std::string startNode)
//-----------------------------------------------------------------------------
{
	if (backToAIRoot == true)
		return;

	currentAINode = ai_findNode (startNode);

	if (-1 == currentAINode)
	{
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Error: Couldn't locate start AI node"));
		return;
	}

	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Index for [ %s ] is [ %i ]", startNode.c_str (), currentAINode));

	if ((aiTree[currentAINode].childVisitedCounter == aiTree[currentAINode].numChildren) && (aiTree[currentAINode].numChildren != 0))
	{
		if ((aiTree[currentAINode].childVisitedCounter == aiTree[currentAINode].numChildren) && (aiTree[currentAINode].parentIndex == -1))
		{
			log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Finished walking tree. Exit"));
			aiTree[currentAINode].childVisitedCounter = 0;
			backToAIRoot  = true;
			currentAINode = 0;
			startNode == "ROOT_NODE";
			return;
		}

		// Done processing all these children - back to parent
		//
		// Process the status of each of the children
		//
		aiTree[currentAINode].childVisitedCounter = 0;
		aiTree[aiTree[currentAINode].parentIndex].childVisitedCounter++;
		ai_debugTree (aiTree[aiTree[currentAINode].parentIndex].nodeName);
	}

	if (false == backToAIRoot)
	{
		if (aiTree[currentAINode].numChildren == 0)
		{
			//
			// Got an end node - no child nodes attached
			//
			log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Parent [ %s ] Child [ %s ]", aiTree[aiTree[currentAINode].parentIndex].nodeName.c_str (), aiTree[currentAINode].nodeName.c_str ()));
			//
			// Mark this child as having been visited
			//
			aiTree[aiTree[currentAINode].parentIndex].childVisitedCounter++;
			ai_debugTree (aiTree[aiTree[currentAINode].parentIndex].nodeName);
		}
		else
		{
			// Node has a child nodes attached
			// Go into those
			//
			ai_debugTree (aiTree[aiTree[currentAINode].childIndex[aiTree[currentAINode].childVisitedCounter]].nodeName);
		}
	}

	return;
}

//-----------------------------------------------------------------------------
//
// Find the nearest tile type
b2Vec2 ai_findNearestTile (int whichDroid, int tileType, std::string levelName)
//-----------------------------------------------------------------------------
{
	float  lowestDistance, distance;
	int    lowestIndex;
	b2Vec2 tempLocation;

	lowestDistance = 9999;

	switch (tileType)
	{
		case TILE_TYPE_HEALING:

			if (0 == shipLevel.at (levelName).healing.size ())
			{
				tempLocation.x = -1;
				return tempLocation; // No healing tiles on this level
			}

			if (1 == shipLevel.at (levelName).healing.size ())
			{
				tempLocation = shipLevel.at (levelName).healing[0].worldPos;
				return tempLocation; // Only one tile on level
			}

			//
			// More than one tile - find the nearest one - manhatten distance
			lowestDistance = 9999;

			for (int i = 0; i != shipLevel.at (levelName).healing.size (); i++)
			{
				distance = b2Distance (shipLevel.at (levelName).healing[i].worldPos, shipLevel.at (levelName).droid[whichDroid].worldPos);
				//
				// TODO: Remove double check
				// More than 2 tiles away
				if ((distance / TILE_SIZE) > 2)
				{
					if ((distance < lowestDistance) && (distance > TILE_SIZE * 2))
					{
						lowestDistance = distance;
						lowestIndex    = i;
					}
				}
			}

			if (lowestIndex > shipLevel.at (levelName).healing.size ())
			{
				tempLocation.x = -1;    // Couldn't find one within distance
			}
			else
			{
				tempLocation = shipLevel.at (levelName).healing[lowestIndex].worldPos;
			}

			return tempLocation;
			break;

		case TILE_TYPE_NEAREST_WP:

			lowestDistance = 9999;

			for (int i = 0; i != shipLevel.at (levelName).numWaypoints; i++)
			{
				tempLocation.x = shipLevel.at (levelName).wayPoints[i].x;
				tempLocation.y = shipLevel.at (levelName).wayPoints[i].y;

				distance = b2Distance (tempLocation, shipLevel.at (levelName).droid[whichDroid].worldPos);
				//
				// More than 2 tiles away
				if ((distance < lowestDistance) && (distance > (TILE_SIZE * 2)))
				{
					lowestDistance = distance;
					lowestIndex    = i;
				}
			}

			if (0 == lowestIndex)
				lowestIndex = 1;

			tempLocation.x                                           = shipLevel.at (levelName).wayPoints[lowestIndex].x;
			tempLocation.y                                           = shipLevel.at (levelName).wayPoints[lowestIndex].y;
			//
			// Found the index for the closest waypoint
			//
			shipLevel.at (levelName).droid[whichDroid].wayPointIndex = lowestIndex;

			return tempLocation;
			break;

		case TILE_TYPE_FLEE:

			int destX = 0;
			int destY = 0;

			if (shipLevel.at (levelName).droid[whichDroid].worldPos.x <= playerDroid.worldPos.x)    // Flee to the left
			{
				for (int i = 0; i != (int) shipLevel.at (levelName).levelDimensions.x * (int) shipLevel.at (levelName).levelDimensions.y; i++)
				{
					destX++;
					if (destX == (shipLevel.at (levelName).levelDimensions.x))
					{
						destX = 0;
						destY++;
					}

					/*

					//
					// Find the first available passable tile
					if ( false == gam_AStarIsTileSolid ((destY * shipLevel.at(levelName).levelDimensions.x) + destX ))
					{
						tempLocation.x = destX * TILE_SIZE;
						tempLocation.y = destY * TILE_SIZE;
						return tempLocation;
					}

					 */

				}
			}
			else    // Flee to the right
			{
				destX = shipLevel.at (levelName).levelDimensions.x - 5;
				destY = shipLevel.at (levelName).levelDimensions.y - 5;

				for (int i = (int) (shipLevel.at (levelName).levelDimensions.x - 5) * (int) (shipLevel.at (levelName).levelDimensions.y - 5); i > 0; i--)
				{
					destX--;
					if (destX == (0))
					{
						destX = shipLevel.at (levelName).levelDimensions.x;
						destY--;
					}

					/*
					 *
					 *

					//
					// Find the first available passable tile
					if ( false == gam_AStarIsTileSolid ((destY * shipLevel.at(levelName).levelDimensions.x) + destX ))
					{
						tempLocation.x = destX * TILE_SIZE;
						tempLocation.y = destY * TILE_SIZE;
						return tempLocation;
					}

					 */
				}
			}
			break;
	}
	log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Error: Reached invalid end of function (ai_findNearestTile)."));
	tempLocation.x = -1;
	tempLocation.y = -1;
	return tempLocation;    // Keep compiler happy
}

//-----------------------------------------------------------------------------
//
// Work out the best AI state for this Droid to be in
void ai_findBestState (int whichDroid, const std::string levelName)
//-----------------------------------------------------------------------------
{
	//
	// Health is most important
	if (shipLevel.at (levelName).droid[whichDroid].currentHealth < 30)
	{
		shipLevel.at (levelName).droid[whichDroid].ai_currentState = AI_STATE_HEALTH;
		shipLevel.at (levelName).droid[whichDroid].ai_moveMode     = -1;
		return;
	}

	//
	// Default state is to patrol
	shipLevel.at (levelName).droid[whichDroid].ai_currentState = AI_STATE_PATROL;
}

//-----------------------------------------------------------------------------
//
// Clear the hasCollided flag before running physics
void ai_clearHadCollidedFlag (const std::string levelName)
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
	{
		shipLevel.at (levelName).droid[i].hasCollided = false;
		if (shipLevel.at (levelName).droid[i].collisionCount > 10)
		{
			shipLevel.at (levelName).droid[i].ignoreCollisions        = true;
			shipLevel.at (levelName).droid[i].ignoreCollisionsCounter = IGNORE_COLLISION_TIME * float (rand () % 3);
			shipLevel.at (levelName).droid[i].collisionCount          = 0;
		}
	}
}

//-----------------------------------------------------------------------------
//
// Process the AI for each enemy droid
void ai_processDroidAI (const std::string levelName)
//-----------------------------------------------------------------------------
{
	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
	{
		if (shipLevel.at (levelName).droid[i].currentMode == DROID_MODE_NORMAL)
		{
					gam_findChanceToShoot ( i, levelName );

			backToAIRoot = false;
			ai_walkTree ("ROOT_NODE", i, levelName);

//					gam_processIgnoreCollisions ( levelName, i );

//					gam_droidWeaponCharge ( i, levelName );

			sys_updateDroidPosition (levelName, i);
		}
	}
}

