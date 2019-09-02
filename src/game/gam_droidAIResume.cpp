#include <hdr/game/gam_droidAI.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_pathFind.h>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_droidAIPatrol.h>
#include "hdr/game/gam_droidAIResume.h"

//#define DEBUG_RESUME 1

//-----------------------------------------------------------------------------
//
// Checks to see if the droid needs to return to patrolling
int ai_isNotPatrolling ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	if ( shipLevel.at( levelName ).droid[whichDroid].isNotPatrolling )
		{
			return AI_RESULT_FAILED;
		}
	return AI_RESULT_SUCCESS;
}

//-----------------------------------------------------------------------------
// Reset resume flags for when found destination tile or changing level
void ai_resetResumeFlags ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	shipLevel.at( levelName ).droid[whichDroid].isNotPatrolling = false;
	shipLevel.at( levelName ).droid[whichDroid].onResumeDestTile = false;
	shipLevel.at( levelName ).droid[whichDroid].aStarInitDone = false;

	shipLevel.at( levelName ).droid[whichDroid].destSet = false;
	shipLevel.at( levelName ).droid[whichDroid].aStarInitDone = false;
	shipLevel.at( levelName ).droid[whichDroid].onHealingTile = false;
	shipLevel.at( levelName ).droid[whichDroid].foundHealingTile = false;
	shipLevel.at( levelName ).droid[whichDroid].aStarDestinationFound = false;
}

//-----------------------------------------------------------------------------
//
// Has droid made it to the resume destination
int ai_onResumeDest ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	if ( true == shipLevel.at( levelName ).droid[whichDroid].onResumeDestTile )
		{
			ai_resetResumeFlags(whichDroid, levelName);
			return AI_RESULT_RUNNING;
		}
	else
		{
//		con_print(true, true, "Droid [ %i ] still returning to resume", whichDroid);
			return AI_RESULT_FAILED;
		}
}

//-----------------------------------------------------------------------------
//
// Work out the AStar path to the destination
int ai_reachedResumeDest ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	b2Vec2 nearestWP;
	b2Vec2 droidWorldPosTiles;

	if ( !shipLevel.at( levelName ).droid[whichDroid].destSet )
		{
			if ( !shipLevel.at( levelName ).droid[whichDroid].aStarInitDone )
				{
					nearestWP = ai_findNearestTile ( whichDroid, TILE_TYPE_NEAREST_WP, levelName );

					nearestWP.y += (TILE_SIZE / 2);

					nearestWP.x = ( int ) nearestWP.x / TILE_SIZE;
					nearestWP.y = ( int ) nearestWP.y / TILE_SIZE;

					// TODO - check this
					/*
					if ( true == gam_AStarIsTileSolid ( ( nearestWP.y * shipLevel.at( levelName ).levelDimensions.x ) + nearestWP.x ) )
					{
						printf("ERROR: Found a solid tile for nearest waypoint.\n");
						return AI_RESULT_FAILED;
					}
		*/

					droidWorldPosTiles.x = ( int ) shipLevel.at( levelName ).droid[whichDroid].worldPos.x / TILE_SIZE;
					droidWorldPosTiles.y = ( ( int ) shipLevel.at( levelName ).droid[whichDroid].worldPos.y + ( SPRITE_SIZE / 2 ) ) / TILE_SIZE;

					shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex = gam_AStarRequestNewPath ( droidWorldPosTiles, nearestWP, whichDroid, levelName );
					if (shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex < 0)
						{
							log_logMessage(LOG_LEVEL_ERROR, sys_getString("Error: Couldn't get a valid path index,"));
							return AI_RESULT_FAILED;
						}

					shipLevel.at( levelName ).droid[whichDroid].aStarInitDone = true;
				}

			if ( gam_AStarIsPathReady ( shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex ))
				{
					shipLevel.at( levelName ).droid[whichDroid].destSet = true;

					shipLevel.at( levelName ).droid[whichDroid].currentAStarIndex = gam_AStarGetNumWaypoints ( shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex );
					//
					// Point to first entry
					shipLevel.at( levelName ).droid[whichDroid].currentAStarIndex++;
					shipLevel.at( levelName ).droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					ai_getNextWaypoint ( whichDroid, levelName );
					return AI_RESULT_RUNNING;
				}
			return AI_RESULT_FAILED; // Now start moving
		}

	if ( shipLevel.at( levelName ).droid[whichDroid].ai_moveMode == AI_PATHFIND_END )
		{
#ifdef DEBUG_RESUME
			con_print ( true, true, "Returned to RESUME location [ %i ]", whichDroid );
#endif
			return AI_RESULT_SUCCESS;
		}
	else
		{
			if ( AI_RESULT_SUCCESS == ai_moveDroidToWaypoint ( whichDroid, levelName ) )
				{
					shipLevel.at( levelName ).droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					if ( AI_RESULT_FAILED == ai_getNextWaypoint ( whichDroid, levelName ) )
						{
							shipLevel.at( levelName ).droid[whichDroid].ai_moveMode = AI_MODE_WAYPOINT;
							shipLevel.at( levelName ).droid[whichDroid].onResumeDestTile = true;
							shipLevel.at( levelName ).droid[whichDroid].destSet = false;
							shipLevel.at( levelName ).droid[whichDroid].aStarInitDone = false;
							gam_AStarRemovePath ( shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex );
						}
				}
			return AI_RESULT_RUNNING;
		}

	return AI_RESULT_RUNNING;
}

