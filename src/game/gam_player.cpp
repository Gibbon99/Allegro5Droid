#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_keyboard.h>
#include "hdr/game/gam_player.h"

cpVect        previousPlayerWorldPos;
float         playerAcceleration = 0.001f;
float         playerMaxSpeed     = 0.010f;
float         gravity            = 0.002f;
_droid        playerDroid;

// ----------------------------------------------------------------------------
//
// Position the player on the requested lift on the new level
cpVect gam_getLiftWorldPosition (int whichLift, std::string whichLevel)
// ----------------------------------------------------------------------------
{
	int   whichTile, countY, countX, liftCounter;
	int   tilePosX, tilePosY;
	float pixelX, pixelY;

	cpVect returnPosition;

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

//									returnPosition.x = (tilePosX * TILE_SIZE) + pixelX;
//									returnPosition.y = (tilePosY * TILE_SIZE) + pixelY;

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
	playerDroid.worldPos         = cpBodyGetPosition (playerDroid.body);
}

