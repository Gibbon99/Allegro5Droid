#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_doors.h>
#include <hdr/io/io_logFile.h>
#include "hdr/game/gam_pathFind.h"

//#define DEBUG_ASTAR 1

#define USE_VALID_TEST \
	if (false == path[whichPath].inUse) \
		{ \
			printf("Error: Attempting to use a non-inUse path [ %i ].\n", whichPath); \
			return; \
		} \

/*
AStar
Droid requests a new path - is passed a ticket
Ticket is an index into the path array
The path assigned holds the current level and current droid
Each tick the path is expanded by one tile in each direction
Droid keeps checking to see if the path is ready or not
Flag set when the path is found and the compressed waypoints are copied into the path array
 */

//
// All coords stored in tile coords
//
// Translated into world coords only at the end when finding path

std::vector<_nodeList>       path;
int                     numAStarPaths; // Set from script
int                     debugAStarIndex;

//-----------------------------------------------------------------------------
//
// structure to hold information for healing tiles
//
//-----------------------------------------------------------------------------
bool            doaStarPath = true;
ALLEGRO_TIMER     *aStarPathTimer;
int          aStarPathInterval;      // From script


//-----------------------------------------------------------------------------
//
// Find the distance from one tile to the next - Manhatten distance
int gam_AStarFindDistance ( cpVect fromTile, cpVect toTile )
//-----------------------------------------------------------------------------
{
	int costX, costY;

	costX = ( fromTile.x ) - ( toTile.x );
	costY = ( fromTile.y ) - ( toTile.y );

	return abs ( 10 * ( costX + costY ) );
}

//-----------------------------------------------------------------------------
//
// Add a tile to the openNode list
inline void gam_AStarAddTileToOpenNode ( int whichPath, cpVect whichTile, int moveCost, int parent )
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_ASTAR
	con_print ( true, false, "Adding new tile to openNode list" );
#endif

//	USE_VALID_TEST

	_pathNode tempNode;

	tempNode.parent = parent;
	tempNode.tileLocation = whichTile;

	if ( -1 == parent ) // Adding the first node – starting tile
		tempNode.g_movementCost = 0;
	else
		tempNode.g_movementCost = moveCost + path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].g_movementCost;

	tempNode.h_estMoveCost = gam_AStarFindDistance ( whichTile, path[whichPath].destTile );
	tempNode.f_score = tempNode.g_movementCost + tempNode.h_estMoveCost;

	path[whichPath].openNodes.push_back ( tempNode );

#ifdef DEBUG_ASTAR
	con_print ( CON_INFO, false, "Path [ %i ] Size [ %i ] parent [ %i ] moveCost [ %i ] distanceCost [ %i ]", whichPath, path[whichPath].openNodes.size(), parent, tempNode.g_movementCost, tempNode.h_estMoveCost );
#endif

}

//-----------------------------------------------------------------------------
//
// Move a node from the openNode list to the closedNode list
void gam_AStarMoveNodeToClosedList ( int whichPath, int whichNodeIndex )
//-----------------------------------------------------------------------------
{
	_pathNode tempNode;
	std::vector<_pathNode>::iterator itr;
	int indexCount = 0;

	USE_VALID_TEST
/*
	if ( path[whichPath].currentNodePtrClosedList > 100 )
		{
			printf ( "ERROR: Closed list is incrementing too much - Path [ %i ] Droid [ %i ]\n", whichPath, path[whichPath].whichDroid );
			gam_AStarDebugPaths();
			return;
		}
*/

#ifdef DEBUG_ASTAR
	con_print ( true, false, "Move open node [ %i ] to closedNode list", whichNodeIndex );
#endif

	tempNode = path[whichPath].openNodes[whichNodeIndex];
	path[whichPath].closedNodes.push_back ( tempNode );
	path[whichPath].currentNodePtrClosedList = path[whichPath].closedNodes.size() - 1;

#ifdef DEBUG_ASTAR
	printf ( "currentNodePtrClosedList is now [ %i ] - number closedNodes [ %i ]\n", path[whichPath].currentNodePtrClosedList, path[whichPath].closedNodes.size() - 1 );
#endif

	indexCount = 0;
	for ( itr = path[whichPath].openNodes.begin(); itr != path[whichPath].openNodes.end(); itr++ )
		{
			if ( indexCount == whichNodeIndex )
				{
					path[whichPath].openNodes.erase ( itr );
					return;
				}
			indexCount++;
		}
}

//-----------------------------------------------------------------------------
//
// Is a tile considered 'solid' or not
bool gam_AStarIsTileSolid ( int tileIndex, int whichPath )
//-----------------------------------------------------------------------------
{
	if ( tileIndex < 0 )
		return true;

	int whichTile = shipLevel.at(path[whichPath].whichLevel).tiles[tileIndex];

	switch ( whichTile )
		{
			case DOOR_ACROSS:
			case DOOR_ACROSS_OPEN_1:
			case DOOR_ACROSS_OPEN_2:
			case DOOR_ACROSS_CLOSING_1:
			case DOOR_ACROSS_CLOSING_2:

			case DOOR_UP:
			case DOOR_UP_OPEN_1:
			case DOOR_UP_OPEN_2:
			case DOOR_UP_CLOSING_1:
			case DOOR_UP_CLOSING_2:
				return false;
			break;

			case TERMINAL_BOTTOM:
			case TERMINAL_LEFT:
			case TERMINAL_RIGHT:
			case TERMINAL_TOP:
				return true;
			break;
		}

	if ( whichTile <= NO_PASS_TILE )	// This stops the wall tile being considered passable
		return true;
	else
		return false;

	return true;    // Should never get here
}

//-----------------------------------------------------------------------------
//
// Check if node is in the CLOSED list
bool gam_AStarIsNodeInClosedList ( int whichPath, cpVect whichNode )
//-----------------------------------------------------------------------------
{
	for ( int i = 0; i != (int)path[whichPath].closedNodes.size(); i++ )
		{
			if ( ( whichNode.x == path[whichPath].closedNodes[i].tileLocation.x ) &&
			     ( whichNode.y == path[whichPath].closedNodes[i].tileLocation.y ) )
				return true;
		}
	return false;
}

//-----------------------------------------------------------------------------
//
// Check if node is in the OPEN list
bool gam_AStarIsNodeInOpenList ( int whichPath, cpVect whichNode )
//-----------------------------------------------------------------------------
{
	for ( int i = 0; i != (int)path[whichPath].openNodes.size(); i++ )
		{
			if ( ( whichNode.x == path[whichPath].openNodes[i].tileLocation.x ) &&
			     ( whichNode.y == path[whichPath].openNodes[i].tileLocation.y ) )
				return true;
		}
	return false;
}

//-----------------------------------------------------------------------------
//
// Generate a new node
//
// Return FALSE if the tile is solid and no new node can be generated
// Return FALSE if the node is already used on the CLOSED list
bool gam_AStarGenerateNewNode ( int whichPath, int whichDirection )
//-----------------------------------------------------------------------------
{
//	USE_VALID_TEST

	std::string 		directionStr;
	_pathNode 	tempNode;
	int 		moveTileCost;

	if ( path[whichPath].pathReady )
		return true;

#ifdef DEBUG_ASTAR
	con_print ( true, false, "---" );
	con_print ( true, false, "Generate a new node based on the node with the current lowest cost." );
#endif

	switch ( whichDirection )
		{
			case NODE_LEFT:
				tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x - 1;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y;
			moveTileCost = 10;
			directionStr = "LEFT";
			break;

			case NODE_UP:
				tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y - 1;
			moveTileCost = 10;
			directionStr = "UP";
			break;

			case NODE_RIGHT:
				tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x + 1;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y;
			moveTileCost = 10;
			directionStr = "RIGHT";
			break;

			case NODE_DOWN:
				tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y + 1;
			moveTileCost = 10;
			directionStr = "DOWN";
			break;
		}
#ifdef DEBUG_ASTAR
	con_print ( CON_INFO, false, "Parent [ %i ] Direction [ %s ]", path[whichPath].currentNodePtrClosedList, directionStr.c_str() );
#endif

	//
	// Change to whole numbers for matching later on
	//
	tempNode.tileLocation.x = ( int ) tempNode.tileLocation.x;
	tempNode.tileLocation.y = ( int ) tempNode.tileLocation.y;

#ifdef DEBUG_ASTAR
	con_print ( true, false, "See if the node is solid or not" );
#endif

	if ( true == gam_AStarIsTileSolid ( ( tempNode.tileLocation.y * ( shipLevel.at(path[whichPath].whichLevel).levelDimensions.x ) +
	                                      tempNode.tileLocation.x ), whichPath ) )
		{
			return false; // Solid - don't use
		}
	//
	// See if we have found a path to the destination tile
	if ( ( tempNode.tileLocation.x == path[whichPath].destTile.x ) && ( tempNode.tileLocation.y == path[whichPath].destTile.y ) )
		{
			gam_AStarAddTileToOpenNode ( whichPath, tempNode.tileLocation, moveTileCost, path[whichPath].currentNodePtrClosedList );
			gam_AStarMoveNodeToClosedList ( whichPath, path[whichPath].openNodes.size() );
			path[whichPath].pathReady = true;
			return true;
		}
	//
	// See if the node is on the closed list or not
	if ( gam_AStarIsNodeInClosedList ( whichPath, tempNode.tileLocation ))
		{
			return false; // Already in closed list - do not use
		}
	//
	// If it isn't on the open list - add it
	if ( !gam_AStarIsNodeInOpenList ( whichPath, tempNode.tileLocation ))
		{
			gam_AStarAddTileToOpenNode ( whichPath, tempNode.tileLocation, moveTileCost, path[whichPath].currentNodePtrClosedList );
			return false;
		}
	//
	// The node is already on the open list - see if the runningCost to this node is better
	int nodeIndex = 0;
	size_t openNodeSize;
	size_t closedNodeSize;

	nodeIndex = path[whichPath].currentNodePtrClosedList;
	openNodeSize = path[whichPath].openNodes.size ();
	closedNodeSize = path[whichPath].closedNodes.size ();

	if ( nodeIndex <= closedNodeSize )
		{
			if ( path[whichPath].openNodes[openNodeSize].g_movementCost < path[whichPath].closedNodes[nodeIndex].g_movementCost )
				{
#ifdef DEBUG_ASTAR
					con_print (CON_INFO, false, "New node is better than the old one");
#endif
					gam_AStarAddTileToOpenNode (whichPath, path[whichPath].openNodes[openNodeSize].tileLocation, moveTileCost, path[whichPath].currentNodePtrClosedList);
				}
		}

	return true;
}

//------------------------------------------------------------
//
// Draw debug graphic
void gam_AStarDebugDraw ( cpVect lineStart, cpVect lineFinish, int whichPath, int count )
//------------------------------------------------------------
{
//	lineStart = sys_worldToScreen ( lineStart, 2 );
//	lineFinish = sys_worldToScreen ( lineFinish, 2 );

//	gl_drawLine (glm::vec3{lineStart.x, lineStart.y, 0}, glm::vec3{lineFinish.x, lineFinish.y, 0}, "colorLine", vec4 (1, 0, 0, 1));

/*
	lineColor = al_map_rgb ( 0, 0, colorIntensity * count );
	al_draw_filled_rounded_rectangle ( lineStart.x - 8, lineStart.y - 8, lineStart.x + 8, lineStart.y + 8, 2, 2, lineColor );

	lineColor = al_map_rgb ( 0, 0, 0 );
	al_draw_filled_rounded_rectangle ( lineFinish.x - 4, lineFinish.y - 4, lineFinish.x + 4, lineFinish.y + 4, 2, 2, lineColor );
 */
}

//------------------------------------------------------------
//
// show waypoint lines
void gam_AStarDebugWayPoints ( int whichPath )
//------------------------------------------------------------
{
//	if ( false == path[whichPath].wayPointsReady )
//		return;

	if (-1 == whichPath)
		return;

	if (path.empty())
		return;

	if (path[whichPath].wayPoints.empty())
		return;

	cpVect lineStart;
	cpVect lineFinish;

	int		i;

	for ( i = 0; i != (int)path[whichPath].wayPoints.size() - 1; i++ )
		{
			lineStart = path[whichPath].wayPoints[i];
			lineFinish = path[whichPath].wayPoints[i + 1];

			gam_AStarDebugDraw ( lineStart, lineFinish, whichPath, i );
		}

	lineStart = path[whichPath].wayPoints[i];
	lineFinish = path[whichPath].wayPoints[i + 1];

	gam_AStarDebugDraw ( lineStart, lineFinish, whichPath, i );
}

//------------------------------------------------------------
//
// Show path structure information
void gam_AStarDebugPaths()
//------------------------------------------------------------
{
	log_logMessage(LOG_LEVEL_INFO, sys_getString("Number of paths [ %i ]", path.size() ));

	for ( int i = 0; i != (int)path.size(); i++ )
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString("inUse [ %s ] Droid [ %i ] Path [ %i ] - Waypoints Ready [ %s ] - Size foundNodes [ %i ] ",
			            path[i].inUse ? "true" : "false", path[i].whichDroid, i,
			            path[i].wayPointsReady ? "true" : "false", path[i].foundPath.size() ));
		}
}

//------------------------------------------------------------
//
// Clear coord memory and flag ready for reuse
void gam_AStarRemovePath ( int whichPath )
//------------------------------------------------------------
{
	USE_VALID_TEST

	if ( !path[whichPath].inUse )
		{
			printf ( "Error: Path not in use - can not remove. Path [ %i ]\n", whichPath );
			return;
		}

	shipLevel.at(path[whichPath].whichLevel).droid[path[whichPath].whichDroid].aStarPathIndex = -1;

	path[whichPath].pathReady = false;
	path[whichPath].wayPointsReady = false;
	path[whichPath].currentNodePtrClosedList = 0;
	path[whichPath].whichDroid = -1;
	path[whichPath].whichLevel = "";
	path[whichPath].openNodes.clear();
	path[whichPath].closedNodes.clear();
	path[whichPath].wayPoints.clear();
	path[whichPath].foundPath.clear();
	path[whichPath].inUse = false;
}

//------------------------------------------------------------
//
// Return how many waypoints there are
// Return -1 if thread is still running
int gam_AStarGetNumWaypoints ( int whichPath )
//------------------------------------------------------------
{
	//
	// Make sure the path passed is ok to use
	if ( ( whichPath > (int)path.size() ) || ( whichPath < 0 ) )
		return -1;

	if ( !path[whichPath].wayPointsReady || !path[whichPath].inUse )
		return -1;

	return static_cast<int>(path[whichPath].wayPoints.size() - 1);
}

//------------------------------------------------------------
//
// Check if the current thread pathfinding is finished
bool gam_AStarIsPathReady ( int whichPath )
//------------------------------------------------------------
{
	//
	// Make sure the path passed is ok to use
	if ( ( whichPath > (int)path.size() ) || ( whichPath < 0 ) )
		return -1;

	if ( !path[whichPath].inUse )
		return -1;

	return path[whichPath].wayPointsReady;
}

//-------------------------------------------------------------------
//
// Extract the path from the list of closed nodes
void gam_AStarExtractPath ( int whichPath )
//-------------------------------------------------------------------
{
	int nodeParent = -1;
	_pathNode tempNode;

	//
	// Store the destination tile
	tempNode.tileLocation = path[whichPath].destTile;
	path[whichPath].foundPath.push_back ( tempNode );
	//
	// Get the next tile along
	tempNode.tileLocation = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList - 1].tileLocation;
	path[whichPath].foundPath.push_back ( tempNode );
	//
	// What's the parent
	nodeParent = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList - 1].parent;
	//
	// While we haven't reached the starting tile ( no parent )
	while ( -1 != nodeParent )
		{
			tempNode = path[whichPath].closedNodes[nodeParent];
			path[whichPath].foundPath.push_back ( tempNode );

			nodeParent = path[whichPath].closedNodes[nodeParent].parent;

			if ( ( tempNode.tileLocation.x == path[whichPath].startTile.x ) && ( tempNode.tileLocation.y == path[whichPath].startTile.y ) )
				{
					path[whichPath].foundPath.push_back ( tempNode );
					return;
				}
		}
}

//-----------------------------------------------------------------------------
//
// Compress the waypoints
void gam_AStarCompressWaypoints ( int whichPath )
//-----------------------------------------------------------------------------
{
	int current = 1;
	std::vector<cpVect>		newPoints;

	newPoints.push_back ( path[whichPath].wayPoints[0] );

	for ( int i = 0; i != (int)path[whichPath].wayPoints.size() - 1; i++ )
		{
			if ( ( path[whichPath].wayPoints[current - 1].x != path[whichPath].wayPoints[current + 1].x ) &&
			     ( path[whichPath].wayPoints[current - 1].y != path[whichPath].wayPoints[current + 1].y ) )
				{
					newPoints.push_back ( path[whichPath].wayPoints[current] );
					current++;
				}
			else
				{
					current++;
				}
		}

	newPoints.push_back ( path[whichPath].wayPoints[current] );

	//
	// Copy back into structure
	path[whichPath].wayPoints.clear();
	for ( int i = 0; i != (int)newPoints.size() - 1; i++ )
		{
			path[whichPath].wayPoints.push_back ( newPoints[i] );

#ifdef DEBUG_ASTAR
			printf ( "%i %3.2f %3.2f\n", i, path[whichPath].wayPoints[i].x, path[whichPath].wayPoints[i].y );
#endif
		}
}

//-----------------------------------------------------------------------------
//
// Turn the AStar nodes into world position coords
void gam_AStarConvertToCoords ( int whichPath )
//-----------------------------------------------------------------------------
{
	cpVect tempWaypoint;

	for ( int i = 0; i != (int)path[whichPath].foundPath.size(); i++ )
		{
			tempWaypoint.x = path[whichPath].foundPath[i].tileLocation.x * TILE_SIZE;
			tempWaypoint.y = path[whichPath].foundPath[i].tileLocation.y * TILE_SIZE;

			tempWaypoint.x += TILE_SIZE / 2;
			tempWaypoint.y += TILE_SIZE / 2;

			path[whichPath].wayPoints.push_back ( tempWaypoint );

			tempWaypoint.x /= TILE_SIZE;
			tempWaypoint.y /= TILE_SIZE;
		}


	if ( path[whichPath].wayPoints.size() > 4 )
		gam_AStarCompressWaypoints ( whichPath );

}

//-----------------------------------------------------------------------------
//
// Find the node with the lowest totalCost
int gam_AStarFindLowestCostNode ( int whichPath )
//-----------------------------------------------------------------------------
{
	int lowestCost;
	int lowestNodeIndexArray;

	lowestCost = 50000;
	lowestNodeIndexArray = 0;

//	USE_VALID_TEST

	if ( path[whichPath].openNodes.empty())
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString("ERROR: No open nodes to search in gam_AStarFindLowestCostNode - path [ %i ]", whichPath ));
			return -1;
		}

#ifdef DEBUG_ASTAR
	con_print ( CON_INFO, false, "Search the open nodes and find the one with the lowest total cost" );
#endif

	for ( int i = 0; i != (int)path[whichPath].openNodes.size(); i++ )
		{
			if ( path[whichPath].openNodes[i].f_score < lowestCost )
				{
					lowestCost = path[whichPath].openNodes[i].f_score;
					lowestNodeIndexArray = i;
				}
		}
	return lowestNodeIndexArray;
}

//-------------------------------------------------------------------
//
// A thread to run the aStar searching function
void gam_AStarSearchThread ( int whichPathArg )
//-------------------------------------------------------------------
{
	int currentNodeIndex = 0;

#ifdef DEBUG_ASTAR
	con_print ( true, false, "searchThread - whichPath [ %i ]", whichPathArg );
#endif

	if ( !path[whichPathArg].inUse )
		return;

	if ( !path[whichPathArg].pathReady )
		{
			// Look for the lowest F cost node on the open list - this becomes the current node
			currentNodeIndex = gam_AStarFindLowestCostNode ( whichPathArg );
			if ( currentNodeIndex < 0 )
				return;

			// Move it to the closed node list
			gam_AStarMoveNodeToClosedList ( whichPathArg, currentNodeIndex );

			// For each of the surrounding nodes
			gam_AStarGenerateNewNode ( whichPathArg, NODE_RIGHT );
			gam_AStarGenerateNewNode ( whichPathArg, NODE_LEFT );
			gam_AStarGenerateNewNode ( whichPathArg, NODE_UP );
			gam_AStarGenerateNewNode ( whichPathArg, NODE_DOWN );
		}
	else
		{
			gam_AStarExtractPath ( whichPathArg );
			gam_AStarConvertToCoords ( whichPathArg );
			path[whichPathArg].wayPointsReady = true;
		}
}

//-------------------------------------------------------------------
//
// Process all the currently active paths for the current level
void gam_AStarProcessPaths(const std::string levelName)
//-------------------------------------------------------------------
{
	for ( int i = 0; i != (int)path.size(); i++ )
		{
			if ( path[i].inUse && (levelName == path[i].whichLevel ) )
				{
					if ( !path[i].wayPointsReady )
						{
							gam_AStarSearchThread ( i );
						}
				}
		}
}

//-------------------------------------------------------------------
//
// Start a thread to process the path
void gam_AStarStartPath ( int whichPath )
//-------------------------------------------------------------------
{
#ifdef DEBUG_ASTAR
	con_print ( CON_INFO, false, "gam_AStarStartPathThread -> Path ID [ %i ] Droid [ %i ]", whichPath, path[whichPath].whichDroid );
#endif


}

//------------------------------------------------------------
//
// Allocate new structure if required or reuse existing one
// Start thread process
int gam_AStarRequestNewPath ( cpVect start, cpVect destination, int whichDroid, const std::string whichLevel )
//------------------------------------------------------------
{
	_nodeList 	        tempPath;
	int 		        whichPathIndex;
	cpFloat 		    distanceTest;

	static bool initDone = false;

	distanceTest = cpvdist ( start, destination );
	if ( distanceTest < 2 )	// 2 Tiles
		{
			printf ( "ERROR: Start and destination are too close together.\n" );
			return -1;
		}
	//
	// Setup a number of paths ready to use
	if ( !initDone )
		{
			for ( int i = 0; i != numAStarPaths; i++ )
				{
					tempPath.inUse = false;
					tempPath.wayPointsReady = false;
					tempPath.pathReady = false;
					tempPath.whichDroid = -1;
					tempPath.whichLevel = "";
					tempPath.pathReady = false;
					path.push_back ( tempPath );
				}
			initDone = true;
		}
	//
	// Find an un-used path and start it
	for ( int i = 0; i != (int)path.size(); i++ )
		{
			if ( !path[i].inUse )
				{
					path[i].pathReady = false;
					path[i].wayPointsReady = false;
					path[i].startTile.x = ( int ) start.x;
					path[i].startTile.y = ( int ) start.y;
					path[i].destTile.x = ( int ) destination.x;
					path[i].destTile.y = ( int ) destination.y;
					path[i].currentNodePtrClosedList = -1;
					path[i].whichDroid = whichDroid;
					path[i].whichLevel = whichLevel;
					path[i].openNodes.reserve ( 10 );
					path[i].closedNodes.reserve ( 10 );
					path[i].inUse = true;
					gam_AStarAddTileToOpenNode ( i, path[i].startTile, 0, -1 );
#ifdef DEBUG_ASTAR
					con_print ( CON_INFO, true, "Requesting new path start [ %3.2f %3.2f ] Dest [ %3.2f %3.2f ]", start.x, start.y, destination.x, destination.y );
#endif
					return i;	// Pass index (ticket) back to the requesting droid
				}
		}
	//
	// All the current paths are in use
	// Add a new path to the current vector
#ifdef DEBUG_ASTAR
	con_print ( true, false, "INFO: All current path slots are in use - adding new vector item." );
#endif
	tempPath.pathReady = false;
	tempPath.wayPointsReady = false;
	tempPath.startTile.x = ( int ) start.x;
	tempPath.startTile.y = ( int ) start.y;
	tempPath.destTile.x = ( int ) destination.x;
	tempPath.destTile.y = ( int ) destination.y;
	tempPath.currentNodePtrClosedList = -1;
	tempPath.whichDroid = whichDroid;
	tempPath.whichLevel = whichLevel;
	tempPath.openNodes.reserve ( 10 );
	tempPath.closedNodes.reserve ( 10 );
	tempPath.inUse = true;

	path.push_back ( tempPath );
	whichPathIndex = static_cast<int>(path.size() - 1);

	gam_AStarAddTileToOpenNode ( whichPathIndex, path[whichPathIndex].startTile, 0, -1 );

	return whichPathIndex; // Return ticket
}

//------------------------------------------------------------
//
// Reset all path entries
void gam_resetAllPaths()
//------------------------------------------------------------
{
	for ( int i = 0; i != (int)path.size(); i++ )
		gam_AStarRemovePath ( i );
}




// ----------------------------------------------------------------------------
//
// Animate healing tiles called from timer callback
// Does healing tiles on all levels
int gam_aStarPathTimerCallback ( int interval, void *param )
// ----------------------------------------------------------------------------
{
	if ( !doaStarPath )
		return interval;

	for ( auto &levelItr : shipLevel )
		{
			gam_AStarProcessPaths (levelItr.first );
		}

	return interval;
}

// ----------------------------------------------------------------------------
//
// Set the state of the aStar path finding timer
void gam_setaStarState ( bool newState )
// ----------------------------------------------------------------------------
{
	doaStarPath = newState;
}
