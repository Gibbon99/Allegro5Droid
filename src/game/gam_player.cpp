#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/game/gam_lifts.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_droids.h>
#include <hdr/io/io_resources.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_physicActions.h>
#include <hdr/game/gam_bullet.h>
#include "hdr/game/gam_player.h"

b2Vec2 previousPlayerWorldPos;
float  playerAcceleration;      // From script
float  playerMaxSpeed;          // From script
float  gravity;                 // From script
_droid playerDroid;

// ----------------------------------------------------------------------------
//
// Setup player startup values
void gam_initPlayerValues ()
// ----------------------------------------------------------------------------
{
	playerDroid.droidType        = 9; //shipLevel.at (lvl_getCurrentLevelName ()).droidTypes[0];
	playerDroid.currentHealth    = dataBaseEntry[playerDroid.droidType].maxHealth;
	playerDroid.spriteName       = gl_getSpriteName (playerDroid.droidType);
	playerDroid.currentFrame     = 0;
//	playerDroid.numberOfFrames   = sprites.at ("001").numFrames;
	playerDroid.frameDelay       = 1.0f;
	playerDroid.frameAnimCounter = 1.0f;
	playerDroid.currentSpeed     = 0.0f;
	playerDroid.acceleration     = 0.4f; // TODO dataBaseEntry[playerDroid.droidType].accelerate;
	playerDroid.bulletName       = bul_getBulletName (playerDroid.droidType);
}

// ----------------------------------------------------------------------------
//
// Position the player on the requested lift on the new level
b2Vec2 gam_getLiftWorldPosition (int whichLift, std::string whichLevel)
// ----------------------------------------------------------------------------
{
	int   whichTile, countY, countX, liftCounter;
	int   tilePosX, tilePosY;
	float pixelX, pixelY;

	b2Vec2 returnPosition;

	liftCounter = 0;

	for (countY = 0; countY != shipLevel.at (whichLevel).levelDimensions.y; countY++)
	{
		for (countX = 0; countX != shipLevel.at (whichLevel).levelDimensions.x; countX++)
		{
			whichTile = shipLevel.at (whichLevel).tiles[(countY * shipLevel.at (whichLevel).levelDimensions.x) + countX];

			if (LIFT_TILE == whichTile)
			{
				if (liftCounter == whichLift)
				{
					tilePosX = countX * TILE_SIZE; //countX - ((screenWidth / TILE_SIZE) / 2);
					tilePosY = countY * TILE_SIZE; //countY - ((screenHeight / TILE_SIZE) / 2);
					tilePosY += TILE_SIZE;

					pixelX = TILE_SIZE / 2;
					pixelY = -TILE_SIZE / 2;

					returnPosition.x = tilePosX + pixelX;
					returnPosition.y = tilePosY + pixelY;

					return returnPosition;
				}
				else
				{
					liftCounter++;
				}
			}
		}
	}

	log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to find lift tile on level [ %s ]", whichLevel.c_str ()));
	return returnPosition;
}

//-----------------------------------------------------------------------------
//
// Process player movement
void gam_processPlayerMovement ()
//-----------------------------------------------------------------------------
{
	if (keyBinding[gameLeft].currentlyPressed)
	{

		playerDroid.velocity.x -= playerAcceleration;
		if (playerDroid.velocity.x < -playerMaxSpeed)
		{
			playerDroid.velocity.x = -playerMaxSpeed;
		}
	}

	else if (keyBinding[gameRight].currentlyPressed)
	{
		playerDroid.velocity.x += playerAcceleration;
		if (playerDroid.velocity.x > playerMaxSpeed)
		{
			playerDroid.velocity.x = playerMaxSpeed;
		}
	}

	if (keyBinding[gameUp].currentlyPressed)
	{
		playerDroid.velocity.y -= playerAcceleration;
		if (playerDroid.velocity.y < -playerMaxSpeed)
		{
			playerDroid.velocity.y = -playerMaxSpeed;
		}
	}

	else if (keyBinding[gameDown].currentlyPressed)
	{
		playerDroid.velocity.y += playerAcceleration;
		if (playerDroid.velocity.y > playerMaxSpeed)
		{
			playerDroid.velocity.y = playerMaxSpeed;
		}
	}

//
// Do gravity slowdown when no key is pressed
	if (!keyBinding[gameLeft].currentlyPressed)
	{
		if (playerDroid.velocity.x < 0.0f)
		{
			playerDroid.velocity.x += gravity;
			if (playerDroid.velocity.x > 0.0f)
			{
				playerDroid.velocity.x = 0.0f;
			}
		}
	}

	if (!keyBinding[gameRight].currentlyPressed)
	{
		if (playerDroid.velocity.x > 0.0f)
		{
			playerDroid.velocity.x -= gravity;
			if (playerDroid.velocity.x < 0.0f)
			{
				playerDroid.velocity.x = 0.0f;
			}
		}
	}

	if (!keyBinding[gameUp].currentlyPressed)
	{
		if (playerDroid.velocity.y < 0.0f)
		{
			playerDroid.velocity.y += gravity;
			if (playerDroid.velocity.y > 0.0f)
			{
				playerDroid.velocity.y = 0.0f;
			}
		}
	}

	if (!keyBinding[gameDown].currentlyPressed)
	{
		if (playerDroid.velocity.y > 0.0f)
		{
			playerDroid.velocity.y -= gravity;
			if (playerDroid.velocity.y < 0.0f)
			{
				playerDroid.velocity.y = 0.0f;
			}
		}
	}

	previousPlayerWorldPos = playerDroid.worldPos;
	playerDroid.worldPos = playerDroid.body->GetPosition ();     // GetPosition is in meters
	playerDroid.worldPos.x *= pixelsPerMeter;                   // Change to pixels
	playerDroid.worldPos.y *= pixelsPerMeter;
}

//----------------------------------------------------------------------------
//
// Start all the actions resulting from pressing the action key
void gam_processActionKey ()
//----------------------------------------------------------------------------
{
	if (playerDroid.overLiftTile)
	{
		if ((!keyBinding[gameLeft].currentlyPressed) && (!keyBinding[gameRight].currentlyPressed) && (!keyBinding[gameDown].currentlyPressed) && (!keyBinding[gameUp].currentlyPressed))
		{
			gam_performLiftAction ();
			keyBinding[gameAction].currentlyPressed = false;
		}
//		return;
	}

	if ((keyBinding[gameLeft].currentlyPressed) || (keyBinding[gameRight].currentlyPressed) || (keyBinding[gameDown].currentlyPressed) || (keyBinding[gameUp].currentlyPressed))
	{
		gam_addPhysicAction (PHYSIC_EVENT_TYPE_NEW_BULLET, 0, 0, 0, -1, {0, 0});
		keyBinding[gameAction].currentlyPressed = false;
		return;
	}
}