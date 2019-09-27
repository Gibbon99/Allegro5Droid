#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resources.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_droidAI.h>
#include <hdr/game/gam_pathFind.h>
#include <hdr/game/gam_game.h>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_droidAIPatrol.h>
#include <hdr/game/gam_physicActions.h>
#include "hdr/game/gam_droids.h"

std::vector<std::string> droidToSpriteLookup;
float                    droidAnimateSpeed;
float                    explodeAnimateSpeed;

//-----------------------------------------------------------------------------
//
// Process weapon timings
void gam_droidWeaponCharge (const std::string &levelName, int whichDroid, float tickTime)
//-----------------------------------------------------------------------------
{
	if (shipLevel.at (levelName).droid[whichDroid].weaponCanFire)
		return;

	shipLevel.at (levelName).droid[whichDroid].weaponDelay += dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].rechargeTime * tickTime;

	if (shipLevel.at (levelName).droid[whichDroid].weaponDelay > 1.0f)
		{
			shipLevel.at (levelName).droid[whichDroid].weaponDelay   = 0.0f;
			shipLevel.at (levelName).droid[whichDroid].weaponCanFire = true;
		}
}

//------------------------------------------------------------------------
//
// Create the lookup table between droid type and sprite name
void gl_setupDroidToSpriteLookup ()
//------------------------------------------------------------------------
{
	droidToSpriteLookup.reserve (24);

	droidToSpriteLookup.emplace_back ("001");
	droidToSpriteLookup.emplace_back ("123");
	droidToSpriteLookup.emplace_back ("139");
	droidToSpriteLookup.emplace_back ("247");
	droidToSpriteLookup.emplace_back ("249");
	droidToSpriteLookup.emplace_back ("296");
	droidToSpriteLookup.emplace_back ("302");
	droidToSpriteLookup.emplace_back ("329");
	droidToSpriteLookup.emplace_back ("420");
	droidToSpriteLookup.emplace_back ("476");
	droidToSpriteLookup.emplace_back ("493");
	droidToSpriteLookup.emplace_back ("516");
	droidToSpriteLookup.emplace_back ("571");
	droidToSpriteLookup.emplace_back ("598");
	droidToSpriteLookup.emplace_back ("614");
	droidToSpriteLookup.emplace_back ("615");
	droidToSpriteLookup.emplace_back ("629");
	droidToSpriteLookup.emplace_back ("711");
	droidToSpriteLookup.emplace_back ("742");
	droidToSpriteLookup.emplace_back ("751");
	droidToSpriteLookup.emplace_back ("821");
	droidToSpriteLookup.emplace_back ("834");
	droidToSpriteLookup.emplace_back ("883");
	droidToSpriteLookup.emplace_back ("999");
}

//------------------------------------------------------------------------
//
// Return the sprite name for a droidType
std::string gl_getSpriteName (int droidType)
//------------------------------------------------------------------------
{
	if ((droidType < 0) || (droidType > droidToSpriteLookup.size ()))
		{
			return "ERROR - Invalid droidType";
		}

	return droidToSpriteLookup[droidType];
}

//------------------------------------------------------------------------------
//
// Render the droids for this level
void gam_renderDroids (const std::string levelName, float interpolate)
//------------------------------------------------------------------------------
{
	b2Vec2 drawPosition;

	if (shipLevel.at (levelName).droid.empty ())
		{
			return;
		}     // Nothing to render

	for (int index = 0; index != shipLevel.at (levelName).numDroids; index++)
		{
			if (shipLevel.at (levelName).droid[index].currentMode != DROID_MODE_DEAD)
				{
//					if (shipLevel.at (levelName).droid[index].visibleToPlayer)
					{
						if (sys_visibleOnScreen (shipLevel.at (levelName).droid[index].worldPos, SPRITE_SIZE))
							{
								drawPosition = shipLevel.at (levelName).droid[index].worldPos - shipLevel.at (levelName).droid[index].previousWorldPos;
								drawPosition *= interpolate;
								drawPosition += shipLevel.at (levelName).droid[index].previousWorldPos;

								drawPosition = sys_worldToScreen (drawPosition, SPRITE_SIZE);

								switch (shipLevel.at (levelName).droid[index].currentMode)
									{
										case DROID_MODE_NORMAL:
											io_renderTintedSpriteFrame (shipLevel.at (levelName).droid[index].spriteName, shipLevel.at (levelName).droid[index].currentFrame, drawPosition.x, drawPosition.y,
											                            0.0f, 0.0f, 0.0f, shipLevel.at (levelName).droid[index].visibleValue);
										break;

										case DROID_MODE_EXPLODING:
											io_renderSpriteFrame ("explosion", shipLevel.at (levelName).droid[index].currentFrame, drawPosition.x, drawPosition.y);
										break;

										default:
											break;
									}
							}
					}
				}
		}
}

//-----------------------------------------------------------------------------
//
//Setup initial values for enemy droids
void gam_initDroidValues (const std::string levelName)
//-----------------------------------------------------------------------------
{
	_droid tempDroid;
	int    wayPointCount = 1;

	// TODO - clear this out on game over and reset vector to 0
	shipLevel.at (levelName).droid.reserve (static_cast<unsigned long>(shipLevel.at (levelName).numDroids));

	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
		{

//		tempDroid.isAlive = true;
			tempDroid.droidType         = shipLevel.at (levelName).droidTypes[i];
			tempDroid.currentHealth     = dataBaseEntry[tempDroid.droidType].maxHealth;
			tempDroid.wayPointIndex     = rand () % (shipLevel.at (levelName).numWaypoints - 1);
			tempDroid.wayPointDirection = WAYPOINT_DOWN;
			tempDroid.spriteName        = gl_getSpriteName (tempDroid.droidType);
			tempDroid.currentFrame      = 0;

//		tempDroid.body = nullptr;
/*
		if (sprites.empty ())
		{
			tempDroid.numberOfFrames = 9;
		}
		else
		{
			tempDroid.numberOfFrames = sprites.at ("001").numFrames;
		}
*/
			tempDroid.frameDelay       = 1.0f;
			tempDroid.frameAnimCounter = 1.0f;
			tempDroid.currentSpeed     = 0.0f;
			tempDroid.acceleration     = 0.4f; // TODO dataBaseEntry[tempDroid.droidType].accelerate;
			tempDroid.worldPos         = shipLevel.at (levelName).wayPoints[tempDroid.wayPointIndex];    // In pixels

			tempDroid.previousWorldPos = tempDroid.worldPos;
			tempDroid.middlePosition   = shipLevel.at (levelName).wayPoints[i];        // TODO - do maths for this

			tempDroid.destinationCoords = tempDroid.worldPos; //shipLevel.at(levelName).wayPoints[tempDroid.wayPointIndex];
//			ai_getNextWaypoint ( i, levelName );
			tempDroid.destDirection     = sys_getDirection (tempDroid.destinationCoords, tempDroid.worldPos);
			tempDroid.velocity          = {0.0, 0.0};

			tempDroid.currentMode = DROID_MODE_NORMAL;      // TODO - set when the modes are done

			tempDroid.mass = static_cast<int>(strtol (dataBaseEntry[tempDroid.droidType].weight.c_str (), nullptr, 10));  // This value plus the players base value
			tempDroid.mass += static_cast<int>(strtol (dataBaseEntry[0].weight.c_str (), nullptr, 10));

			tempDroid.mass             = 3.0f;   // TODO Update config file
			tempDroid.ignoreCollisions = false;

			tempDroid.targetIndex       = -1;                // Which droid shot this droid
			tempDroid.beenShotByPlayer  = false;
			tempDroid.beenShotCountdown = 0;

			tempDroid.collisionCount = 0;             // how many collision have occured to ignore them
			tempDroid.hasCollided    = false;
			tempDroid.collidedWith   = -1;               // Who did the droid hit

			tempDroid.ignoreCollisionsCounter = IGNORE_COLLISION_TIME;

			tempDroid.playerDroidTypeDBIndex = "db_" + gl_getSpriteName (tempDroid.droidType);
			tempDroid.currentMode            = DROID_MODE_NORMAL;
			tempDroid.chanceToShoot          = 0.0f;
			tempDroid.visibleToPlayer        = true;
			tempDroid.weaponCanFire          = true;
			tempDroid.weaponDelay            = 0.0f;

			tempDroid.witnessShooting = false;
			tempDroid.witnessTransfer = false;

			tempDroid.witnessShootingCountDown = 0.0f;
			tempDroid.witnessTransferCountDown = 0.0f;

			// AI variables
//		int ai_currentState;
			tempDroid.ai_moveMode = AI_MODE_WAYPOINT;

			// Pathfinding
			tempDroid.aStarPathIndex        = -1;            // Index into which path to use
			tempDroid.numberPathNodes       = -1;
			tempDroid.currentAStarIndex     = -1;         // Index into aStarWaypoints array
			tempDroid.aStarDestinationFound = false;
			tempDroid.aStarInitDone         = false;
			tempDroid.previousWaypoints     = {};

			tempDroid.onFleeTile    = false;
			tempDroid.foundFleeTile = false;

			tempDroid.isNotPatrolling  = false;        // Used to enter resume branch
			tempDroid.onResumeDestTile = false;
			tempDroid.destSet          = false;                // Does the droid have a destination

			tempDroid.onHealingTile    = false;
			tempDroid.foundHealingTile = false;

			shipLevel.at (levelName).droid.push_back (tempDroid);
		}
}

//-----------------------------------------------------------------------------
//
// This removes the droid from the current game - sets status to dead, and removes physics body
void gam_removeDroid (int whichDroid)
//-----------------------------------------------------------------------------
{
	shipLevel.at (lvl_getCurrentLevelName ()).droid[whichDroid].currentMode = DROID_MODE_DEAD;

	free (droidPhysics[whichDroid].userData);
	droidPhysics[whichDroid].userData = nullptr;
	droidPhysics[whichDroid].body->SetLinearVelocity ({0, 0});
	sys_getPhysicsWorld ()->DestroyBody (droidPhysics[whichDroid].body);
	droidPhysics[whichDroid].body = nullptr;
}

//-----------------------------------------------------------------------------
//
// Kill an enemy droid
// TODO Remove enemy physics objects  - may need to be done in post-callback collision - Use event in mainLoop
// like for player and droid routine
void gam_destroyDroid (int whichDroid)
//-----------------------------------------------------------------------------
{
	std::string tempCurrentLevel;

	tempCurrentLevel = lvl_getCurrentLevelName ();

	switch (shipLevel.at (tempCurrentLevel).droid[whichDroid].currentMode)
		{
			case DROID_MODE_EXPLODING:
				return;
			break;

			case DROID_MODE_NORMAL:
/*
				if ( shipLevel.at ( lvl_returnLevelNameFromDeck ( whichLevel )).droid[whichDroid].droidType < 6 )
					evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, "explode1", 0, 0, );
				else
					evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_EXPLODE_2, 0, 0, glm::vec2 (), glm::vec2 (), "" );
*/
				shipLevel.at (tempCurrentLevel).droid[whichDroid].currentFrame = 0;
			shipLevel.at (tempCurrentLevel).droid[whichDroid].currentMode    = DROID_MODE_EXPLODING;
//			shipLevel.at (tempCurrentLevel).droid[whichDroid].numberOfFrames = sprites.at ("explosion").numFrames;
			droidPhysics[whichDroid].body->SetLinearVelocity ({0, 0});
			droidPhysics[whichDroid].body->SetType (b2_staticBody);

			if (shipLevel.at (tempCurrentLevel).droid[whichDroid].aStarPathIndex > -1)
				{
					gam_AStarRemovePath (shipLevel.at (tempCurrentLevel).droid[whichDroid].aStarPathIndex);
				}

			// TODO gam_addToScore ( dataBaseEntry[shipLevel.at( lvl_returnLevelNameFromDeck ( whichLevel)).droid[whichDroid].droidType].score );

			/* TODO

			par_addEmitter ( shipLevel.at( lvl_returnLevelNameFromDeck ( whichLevel)).droid[whichDroid].worldPos, PARTICLE_TYPE_EXPLOSION, -1 );
		*/

//
// TODO: Need extra checks here
//
			shipLevel.at (tempCurrentLevel).numEnemiesAlive = shipLevel.at (tempCurrentLevel).numDroids;

			for (int i = 0; i != shipLevel.at (tempCurrentLevel).numDroids; i++)
				{
					if (!(shipLevel.at (tempCurrentLevel).droid[i].currentMode == DROID_MODE_NORMAL) || (shipLevel.at (tempCurrentLevel).droid[i].currentMode == DROID_MODE_EXPLODING))
						{
							shipLevel.at (tempCurrentLevel).numEnemiesAlive--;
						}
				}

			if (0 == shipLevel.at (tempCurrentLevel).numEnemiesAlive)
				{
// TODO 		gam_powerDownLevel ( whichLevel, true );
					log_logMessage (LOG_LEVEL_INFO, sys_getString ("No enemies left on deck. [ %s ]", tempCurrentLevel.c_str ()));
				}

			return;
			break;

			default:

				return;
		}

}

//---------------------------------------------------------------
//
// Do damage to a droid
//
// damageSource can be either a bullet, explosion or collision
void gam_damageToDroid (int targetDroid, int damageSource, int sourceDroid, int eventSource)
//---------------------------------------------------------------
{
	std::string tempCurrentLevel;

	tempCurrentLevel = lvl_getCurrentLevelName ();

	if (shipLevel.at (tempCurrentLevel).droid[targetDroid].currentMode == DROID_MODE_EXPLODING)
		return;

	switch (damageSource)
		{
			case DAMAGE_BULLET:
				if (-1 == sourceDroid) // Player bullet
					{
						printf ("Droid [ %i ] hit by player bullet.\n", targetDroid);

						if (shipLevel.at (tempCurrentLevel).droid[targetDroid].currentMode == DROID_MODE_EXPLODING)
							{
								return;
							}

						shipLevel.at (tempCurrentLevel).droid[targetDroid].targetIndex       = sourceDroid;    // Set player as the target
						shipLevel.at (tempCurrentLevel).droid[targetDroid].beenShotByPlayer  = true;
						shipLevel.at (tempCurrentLevel).droid[targetDroid].beenShotCountdown = droidBeenShotValue;

						//
						// Need to work out bullet damage when using non firing droid
						//
						if (dataBaseEntry[shipLevel.at (lvl_getCurrentLevelName ()).droid[targetDroid].droidType].canShoot)
							shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth -= 20; //dataBaseEntry[playerDroid.droidType].bulletDamage;
						else
							shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth -= 20; //dataBaseEntry[0].bulletDamage;

						if (shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth <= 0)
							{
								gam_destroyDroid (targetDroid);
							}
						else
							{
//								evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );
							}
					}
				else // hit by another droid bullet
					{
						if (sourceDroid == targetDroid)
							{
								return;
							}

						shipLevel.at (tempCurrentLevel).droid[targetDroid].targetIndex      = sourceDroid;    // Set this droid as the target
						shipLevel.at (tempCurrentLevel).droid[targetDroid].beenShotByPlayer = false;
						shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth -= 20; // TODO dataBaseEntry[shipLevel.at (tempCurrentLevel).droid[sourceDroid].droidType].bulletDamage;
//						evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );

						if (shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth <= 0)
							{
								gam_destroyDroid (targetDroid);
							}
					}

//					printf("Remove bullet index [ %i ]\n", eventSource);

		gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, eventSource, {0, 0});
			break;

			case DAMAGE_EXPLOSION:

				if (shipLevel.at (tempCurrentLevel).droid[targetDroid].currentMode == DROID_MODE_EXPLODING)
					{
						return;
					}

			if (-1 == sourceDroid)
				{
					//
					// Player is colliding with exploding sprite
//					gam_doDamageToPlayer ( DAMAGE_EXPLOSION, whichDroid );
				}
			else
				{
					//
					// Enemy Droid is colliding with another one exploding
//					evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );
					shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth -= collisionExplosionDamage;

					if (shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth <= 0)
						{
							gam_destroyDroid (targetDroid);
						}
				}
			break;

			case DAMAGE_COLLISION:
				if (-1 == sourceDroid)
					{
//						gam_doDamageToPlayer ( DAMAGE_COLLISION, whichDroid );
						shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth -= collisionDamageInflicted;
						if (shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth <= 0)
							{
								gam_destroyDroid (targetDroid);
							}
					}
			break;
		}
	//
	// Work out animation speed based on current health
	float healthPercent = shipLevel.at (tempCurrentLevel).droid[targetDroid].currentHealth / (float) dataBaseEntry[shipLevel.at (tempCurrentLevel).droid[targetDroid].droidType].maxHealth;
	if (healthPercent < 0.0f)
		healthPercent = 0.1f;
	if (healthPercent > 1.0f)
		healthPercent = 1.0f;

	shipLevel.at (tempCurrentLevel).droid[targetDroid].frameDelay = healthPercent;
}

//-----------------------------------------------------------------------------
//
// process ignore collosions
void gam_processIgnoreCollisions (const std::string whichLevel, int whichDroid)
//-----------------------------------------------------------------------------
{
	if (shipLevel.at (whichLevel).droid[whichDroid].currentMode == DROID_MODE_DEAD)
		{
			return;
		}

	if (shipLevel.at (whichLevel).droid[whichDroid].collisionCount < (rand () % 5) + 3)
		{
			return;
		}

	shipLevel.at (whichLevel).droid[whichDroid].ignoreCollisions = true;

	shipLevel.at (whichLevel).droid[whichDroid].ignoreCollisionsCounter -= 1.0f * (1.0f / 30.0f);

	if (shipLevel.at (whichLevel).droid[whichDroid].ignoreCollisionsCounter < 0.0f)
		{
			shipLevel.at (whichLevel).droid[whichDroid].ignoreCollisionsCounter = IGNORE_COLLISION_TIME;
			shipLevel.at (whichLevel).droid[whichDroid].ignoreCollisions        = false;
			shipLevel.at (whichLevel).droid[whichDroid].collisionCount          = 0;
			shipLevel.at (whichLevel).droid[whichDroid].hasCollided             = false;
		}
}
