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
#include "hdr/game/gam_droids.h"

std::vector<std::string> droidToSpriteLookup;

//------------------------------------------------------------------------
//
// Create the lookup table between droid type and sprite name
void gl_setupDroidToSpriteLookup()
//------------------------------------------------------------------------
{
	droidToSpriteLookup.reserve(24);

	droidToSpriteLookup.emplace_back("001");
	droidToSpriteLookup.emplace_back("123");
	droidToSpriteLookup.emplace_back("139");
	droidToSpriteLookup.emplace_back("247");
	droidToSpriteLookup.emplace_back("249");
	droidToSpriteLookup.emplace_back("296");
	droidToSpriteLookup.emplace_back("302");
	droidToSpriteLookup.emplace_back("329");
	droidToSpriteLookup.emplace_back("420");
	droidToSpriteLookup.emplace_back("476");
	droidToSpriteLookup.emplace_back("493");
	droidToSpriteLookup.emplace_back("516");
	droidToSpriteLookup.emplace_back("571");
	droidToSpriteLookup.emplace_back("598");
	droidToSpriteLookup.emplace_back("614");
	droidToSpriteLookup.emplace_back("615");
	droidToSpriteLookup.emplace_back("629");
	droidToSpriteLookup.emplace_back("711");
	droidToSpriteLookup.emplace_back("742");
	droidToSpriteLookup.emplace_back("751");
	droidToSpriteLookup.emplace_back("821");
	droidToSpriteLookup.emplace_back("834");
	droidToSpriteLookup.emplace_back("883");
	droidToSpriteLookup.emplace_back("999");
}

//------------------------------------------------------------------------
//
// Return the sprite name for a droidType
std::string gl_getSpriteName(int droidType)
//------------------------------------------------------------------------
{
	if ((droidType < 0) || (droidType > droidToSpriteLookup.size()))
	{
		return "ERROR - Invalid droidType";
	}

	return droidToSpriteLookup[droidType];
}

//------------------------------------------------------------------------------
//
// Animate the droid
void gam_animateThisLevel(const std::string levelName)
//------------------------------------------------------------------------------
{
	if (levelName.empty())
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Invalid or empty levelName passed to function [ %s ]", __LINE__));
		return;
	}

	for (int index = 0; index != shipLevel.at(levelName).numDroids; index++)
	{
		if ((shipLevel.at(levelName).droid[index].currentMode == DROID_MODE_NORMAL) ||
		    (shipLevel.at(levelName).droid[index].currentMode == DROID_MODE_EXPLODING))
		{
			switch (shipLevel.at(levelName).droid[index].currentMode)
			{
				case DROID_MODE_NORMAL:
					shipLevel.at(levelName).droid[index].frameDelay += 1.0f / 3.0f;
					break;

				case DROID_MODE_EXPLODING:
					shipLevel.at(levelName).droid[index].frameDelay += 1.0f / 1.0f;
					break;

				default:
					break;
			}


			if (shipLevel.at(levelName).droid[index].frameDelay > 1.0f)
			{
				shipLevel.at(levelName).droid[index].frameDelay = 0.0f;
				shipLevel.at(levelName).droid[index].currentFrame++;

				switch (shipLevel.at(levelName).droid[index].currentMode)
				{
					case DROID_MODE_NORMAL:
						if (shipLevel.at(levelName).droid[index].currentFrame ==
						    sprites.at(shipLevel.at(levelName).droid[index].spriteName).numFrames)
						{
							shipLevel.at(levelName).droid[index].currentFrame = 0;
						}
						break;

					case DROID_MODE_EXPLODING:
						if (shipLevel.at(levelName).droid[index].currentFrame == sprites.at("explosion").numFrames)
						{
							shipLevel.at(levelName).droid[index].currentMode = DROID_MODE_DEAD;
//												evt_sendEvent ( MAIN_LOOP_EVENT, MAIN_LOOP_EVENT_REMOVE_DROID_PHYSICS, index, 0, 0, levelName );
						}
						break;

					default:
						break;
				}

			}
		}
	}
}

//------------------------------------------------------------------------------
//
// Render the droids for this level
void gam_renderDroids(const std::string levelName, float interpolate)
//------------------------------------------------------------------------------
{
	b2Vec2 drawPosition;

	if (shipLevel.at(levelName).droid.empty())
	{
		return;
	}     // Nothing to render

	for (int index = 0; index != shipLevel.at(levelName).numDroids; index++)
	{
		if (shipLevel.at(levelName).droid[index].currentMode != DROID_MODE_DEAD)
		{

//					if (shipLevel.at (levelName).droid[index].visibleToPlayer)
//			if (sys_visibleOnScreen(shipLevel.at(levelName).droid[index].worldPos, SPRITE_SIZE))
			{
				drawPosition = shipLevel.at(levelName).droid[index].worldPos - shipLevel.at(levelName).droid[index].previousWorldPos;
				drawPosition *= interpolate;
				drawPosition += shipLevel.at(levelName).droid[index].previousWorldPos;

				drawPosition = sys_worldToScreen(drawPosition, SPRITE_SIZE);

//							drawPosition = shipLevel.at (levelName).droid[index].worldPos;

//							drawPosition.y = (int) drawPosition.y;   // Remove the fraction portion to stop blurring in Y direction

				switch (shipLevel.at(levelName).droid[index].currentMode)
				{
					case DROID_MODE_NORMAL:

						io_renderTintedSpriteFrame(shipLevel.at(levelName).droid[index].spriteName, io_getFrame(), drawPosition.x, drawPosition.y, 0, 0, 0);

//										gl_renderSprite (shipLevel.at (levelName).droid[index].spriteName, SPRITE_TYPE_DROID, glm::vec2{drawPosition.x,drawPosition.y}, 0,
//										                 shipLevel.at (levelName).droid[index].currentFrame, glm::vec3{ 1.0f, 1.0f, 0.0f});
						break;

					case DROID_MODE_EXPLODING:
//										gl_renderSprite ("explosion", SPRITE_TYPE_DROID, glm::vec2{drawPosition.x, drawPosition.y}, 0,
//										                 shipLevel.at (levelName).droid[index].currentFrame, glm::vec3{ 1.0f, 1.0f, 0.0f});
						break;

					default:
						break;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//Setup initial values for enemy droids
void gam_initDroidValues(const std::string levelName)
//-----------------------------------------------------------------------------
{
	_droid tempDroid;
	int    wayPointCount = 1;

	// TODO - clear this out on game over and reset vector to 0
	shipLevel.at(levelName).droid.reserve(static_cast<unsigned long>(shipLevel.at(levelName).numDroids));

	for (int i = 0; i != shipLevel.at(levelName).numDroids; i++)
	{

//		tempDroid.isAlive = true;
		tempDroid.droidType         = shipLevel.at(levelName).droidTypes[i];
		tempDroid.currentHealth     = dataBaseEntry[tempDroid.droidType].maxHealth;
		tempDroid.wayPointIndex     = rand() % (shipLevel.at(levelName).numWaypoints - 1);
		tempDroid.wayPointDirection = WAYPOINT_DOWN;
		tempDroid.spriteName        = gl_getSpriteName(tempDroid.droidType);
		tempDroid.currentFrame      = 0;
		if (sprites.empty())
		{
			tempDroid.numberOfFrames = 9;
		}
		else
		{
			tempDroid.numberOfFrames = sprites.at("001").numFrames;
		}

		tempDroid.frameDelay   = 0.0f;
		tempDroid.currentSpeed = 0.0f;
		tempDroid.acceleration = 0.4f; // TODO dataBaseEntry[tempDroid.droidType].accelerate;
		tempDroid.worldPos     = shipLevel.at(levelName).wayPoints[tempDroid.wayPointIndex];    // In pixels

		tempDroid.previousWorldPos = tempDroid.worldPos;
		tempDroid.middlePosition   = shipLevel.at(levelName).wayPoints[i];        // TODO - do maths for this

		tempDroid.destinationCoords = tempDroid.worldPos; //shipLevel.at(levelName).wayPoints[tempDroid.wayPointIndex];
//			ai_getNextWaypoint ( i, levelName );
		tempDroid.destDirection     = sys_getDirection(tempDroid.destinationCoords, tempDroid.worldPos);
		tempDroid.velocity          = {0.0, 0.0};

		tempDroid.overTile    = 0;
		tempDroid.currentMode = DROID_MODE_NORMAL;      // TODO - set when the modes are done

		tempDroid.mass = static_cast<int>(strtol(dataBaseEntry[tempDroid.droidType].weight.c_str(), nullptr, 10));  // This value plus the players base value
		tempDroid.mass += static_cast<int>(strtol(dataBaseEntry[0].weight.c_str(), nullptr, 10));


		tempDroid.mass = 3.0f;   // TODO Update config file


		tempDroid.ignoreCollisions = false;
//		tempDroid.isExploding = false;

		tempDroid.targetIndex       = -1;                // Which droid shot this droid
		tempDroid.beenShotByPlayer  = false;
		tempDroid.beenShotCountdown = 0;

		tempDroid.collisionCount = 0;             // how many collision have occured to ignore them
		tempDroid.hasCollided    = false;
		tempDroid.collidedWith   = -1;               // Who did the droid hit


		tempDroid.ignoreCollisionsCounter = IGNORE_COLLISION_TIME;

		tempDroid.playerDroidTypeDBIndex = "001";     // What sort of droid is the player
//		tempDroid.droidTransferedIntoIndex = 0;

		tempDroid.currentMode = DROID_MODE_NORMAL;

		tempDroid.chanceToShoot = 0.0f;

		tempDroid.visibleToPlayer = true;

		//
// Weapon
		tempDroid.weaponCanFire = false;
		tempDroid.weaponDelay   = 0.0f;

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


		shipLevel.at(levelName).droid.push_back(tempDroid);
	}
}


//-----------------------------------------------------------------------------
//
// Kill an enemy droid
// TODO Remove enemy physics objects  - may need to be done in post-callback collision - Use event in mainLoop
// like for player and droid routine
void gam_destroyDroid(int whichLevel, int whichDroid)
//-----------------------------------------------------------------------------
{
	switch (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentMode)
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
			shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentFrame   = 0;
			shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentMode    = DROID_MODE_EXPLODING;
			shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].numberOfFrames = sprites.at("explosion").numFrames;

			if (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].aStarPathIndex > -1)
			{
				gam_AStarRemovePath(shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].aStarPathIndex);
			}
//			shipLevel.at ( lvl_returnLevelNameFromDeck ( whichLevel )).droid[whichDroid].isExploding = true;

//	shipLevel.at( lvl_returnLevelNameFromDeck ( whichLevel)).droid[whichDroid].currentFrameDelay = 0.0f;
//	shipLevel.at( lvl_returnLevelNameFromDeck ( whichLevel)).droid[whichDroid].isStopped = true;
			// TODO gam_addToScore ( dataBaseEntry[shipLevel.at( lvl_returnLevelNameFromDeck ( whichLevel)).droid[whichDroid].droidType].score );

			/* TODO

			par_addEmitter ( shipLevel.at( lvl_returnLevelNameFromDeck ( whichLevel)).droid[whichDroid].worldPos, PARTICLE_TYPE_EXPLOSION, -1 );
		*/

//
// TODO: Need extra checks here
//
			shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).numEnemiesAlive = shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).numDroids;

			for (int i = 0; i != shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).numDroids; i++)
			{
				if (!shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[i].currentMode == DROID_MODE_NORMAL ||
				    shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[i].currentMode == DROID_MODE_EXPLODING)
				{
					shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).numEnemiesAlive--;
				}
			}

			if (0 == shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).numEnemiesAlive)
			{
// TODO 		gam_powerDownLevel ( whichLevel, true );
				log_logMessage(LOG_LEVEL_INFO, sys_getString("No enemies left on deck. [ %i ]", whichLevel));
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
void gam_damageToDroid(int whichLevel, int whichDroid, int damageSource, int sourceDroid)
//---------------------------------------------------------------
{
	switch (damageSource)
	{
		case DAMAGE_BULLET:
			if (-1 == sourceDroid) // Player bullet
			{

				printf("Droid [ %i ] hit by player bullet.\n", whichDroid);


				if (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentMode ==
				    DROID_MODE_EXPLODING)
				{
					return;
				}

				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].targetIndex       = sourceDroid;    // Set player as the target
				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].beenShotByPlayer  = true;
				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].beenShotCountdown = droidBeenShotValue;

				//
				// Need to work out bullet damage when using non firing droid
				//
//						if ( dataBaseEntry[gui_get3DTextureIndex (playerDroid.playerDroidTypeDBIndex)].canShoot )
//							shipLevel.at ( lvl_returnLevelNameFromDeck ( whichLevel )).droid[whichDroid].currentHealth -= dataBaseEntry[gui_get3DTextureIndex (playerDroid.playerDroidTypeDBIndex)].bulletDamage;
//						else
//							shipLevel.at ( lvl_returnLevelNameFromDeck ( whichLevel )).droid[whichDroid].currentHealth -= dataBaseEntry[0].bulletDamage;

				if (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth <= 0)
				{
					gam_destroyDroid(whichLevel, whichDroid);
				}
				else
				{
//								evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );
				}
			}
			else // hit by another droid bullet
			{
				if (sourceDroid == whichDroid)
				{
					return;
				}

				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].targetIndex      = sourceDroid;    // Set this droid as the target
				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].beenShotByPlayer = false;
				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth -= dataBaseEntry[shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[sourceDroid].droidType].bulletDamage;
//						evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );

				if (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth <= 0)
				{
					gam_destroyDroid(whichLevel, whichDroid);
				}
			}

			break;

		case DAMAGE_EXPLOSION:

			if (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentMode ==
			    DROID_MODE_EXPLODING)
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
				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth -= collisionExplosionDamage;

				if (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth <= 0)
				{
					gam_destroyDroid(whichLevel, whichDroid);
				}
			}
			break;

		case DAMAGE_COLLISION:
			if (-1 == sourceDroid)
			{
//						gam_doDamageToPlayer ( DAMAGE_COLLISION, whichDroid );

				printf("Droid health [ %i ]\n", shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth);

				shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth -= collisionDamageInflicted;
				if (shipLevel.at(lvl_returnLevelNameFromDeck(whichLevel)).droid[whichDroid].currentHealth <= 0)
				{
					gam_destroyDroid(whichLevel, whichDroid);
				}
			}
			break;
	}
}


//-----------------------------------------------------------------------------
//
// process ignore collosions
void gam_processIgnoreCollisions(const std::string whichLevel, int whichDroid)
//-----------------------------------------------------------------------------
{
	if (shipLevel.at(whichLevel).droid[whichDroid].currentMode == DROID_MODE_DEAD)
	{
		return;
	}

	if (shipLevel.at(whichLevel).droid[whichDroid].collisionCount < (rand() % 5) + 3)
	{
		return;
	}

	shipLevel.at(whichLevel).droid[whichDroid].ignoreCollisions = true;

	shipLevel.at(whichLevel).droid[whichDroid].ignoreCollisionsCounter -= 1.0f * (1.0f / 30.0f);

	if (shipLevel.at(whichLevel).droid[whichDroid].ignoreCollisionsCounter < 0.0f)
	{
		shipLevel.at(whichLevel).droid[whichDroid].ignoreCollisionsCounter = IGNORE_COLLISION_TIME;
		shipLevel.at(whichLevel).droid[whichDroid].ignoreCollisions        = false;
		shipLevel.at(whichLevel).droid[whichDroid].collisionCount          = 0;
		shipLevel.at(whichLevel).droid[whichDroid].hasCollided             = false;
	}
}

//-----------------------------------------------------------------------------
//
// Process weapon timings
void gam_droidWeaponCharge(int whichDroid, const std::string levelName)
//-----------------------------------------------------------------------------
{
	if (shipLevel.at(levelName).droid[whichDroid].weaponCanFire)
	{
		return;
	}

	shipLevel.at(levelName).droid[whichDroid].weaponDelay +=
			dataBaseEntry[shipLevel.at(levelName).droid[whichDroid].droidType].rechargeTime * (1.0f * (1.0f / 30.0f));

	if (shipLevel.at(levelName).droid[whichDroid].weaponDelay > 1.0f)
	{
		shipLevel.at(levelName).droid[whichDroid].weaponDelay   = 0.0f;
		shipLevel.at(levelName).droid[whichDroid].weaponCanFire = true;
	}
}
