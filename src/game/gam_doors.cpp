#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_render.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_droids.h>
#include <hdr/system/sys_audio.h>
#include "hdr/game/gam_doors.h"

std::vector<_doorTrigger> doorTriggers;
std::vector<_doorTrigger> doorBulletSensor;

float doorAnimSpeed = 1.0f;         // From script

// ----------------------------------------------------------------------------
//
// Check door trigger areas against sprite positions
void gam_doorCheckTriggerAreas ()
// ----------------------------------------------------------------------------
{
	int i = 0;
	int j = 0;

	if (0 == doorTriggers.size ())
		return;        // no doors on this level to process

	for (auto &doorItr : doorTriggers)
	{
		if ((previousPlayerWorldPos.x > doorItr.topLeft.x) &&
		    (previousPlayerWorldPos.y > doorItr.topLeft.y) &&
		    (previousPlayerWorldPos.x < doorItr.botRight.x) &&
		    (previousPlayerWorldPos.y < doorItr.botRight.y))

		{    // player sprite is inside a trigger area
			doorItr.inUse = true;
		}
		else
		{
			doorItr.inUse = false;    // this will reset all the doors the player is not in
		}
	}
	//
	// now check all the enemy sprites against the doors
	//
	for (auto &doorItr : doorTriggers)
	{
		for (j = 0; j != shipLevel[lvl_getCurrentLevelName ()].numDroids; j++)
		{
			if (shipLevel.at (lvl_getCurrentLevelName ()).droid[j].currentMode == DROID_MODE_NORMAL)
			{
				if ((shipLevel[lvl_getCurrentLevelName ()].droid[j].worldPos.x + (SPRITE_SIZE / 2) > doorItr.topLeft.x) &&
				    (shipLevel[lvl_getCurrentLevelName ()].droid[j].worldPos.y + (SPRITE_SIZE / 2) > doorItr.topLeft.y) &&
				    (shipLevel[lvl_getCurrentLevelName ()].droid[j].worldPos.x + (SPRITE_SIZE / 2) < doorItr.botRight.x) &&
				    (shipLevel[lvl_getCurrentLevelName ()].droid[j].worldPos.y + (SPRITE_SIZE / 2) < doorItr.botRight.y))
				{
					doorItr.inUse = true;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
//
// Process all the doors that are currently inUse
void gam_doorProcessActions ()
// ----------------------------------------------------------------------------
{
	int i;
//	int doorDelayTime = 0;

	if (doorTriggers.empty())
		return;

	for (i = 0; i < doorTriggers.size (); i++)
	{
		if (doorTriggers[i].inUse)
		{
			doorTriggers[i].frameDelay -= doorAnimSpeed * (1.0f / TICKS_PER_SECOND);
			if (doorTriggers[i].frameDelay > 0.0f)
			{
				doorTriggers[i].frameDelay = 1.0f;
				switch (doorTriggers[i].currentFrame)
				{
					case DOOR_ACROSS_CLOSED:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPEN_1;
						evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_ONCE, "doorOpen");
						//gam_playDoorSound(i);
						break;
					case DOOR_ACROSS_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPEN_2;
						break;
					case DOOR_ACROSS_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPENED;
						break;
					case DOOR_ACROSS_OPENED:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPENED;
						break;
					case DOOR_ACROSS_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPENED;
						break;
					case DOOR_ACROSS_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_1;
						break;

					case DOOR_UP_CLOSED:
						doorTriggers[i].currentFrame = DOOR_UP_OPEN_1;
						evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_ONCE, "doorOpen");
						//gam_playDoorSound(i);
						break;
					case DOOR_UP_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_UP_OPEN_2;
						break;
					case DOOR_UP_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_UP_OPENED;
						break;
					case DOOR_UP_OPENED:
						doorTriggers[i].currentFrame = DOOR_UP_OPENED;
						break;
					case DOOR_UP_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_UP_OPENED;
						break;
					case DOOR_UP_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_1;
						break;

				}    // end of switch statement
				shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[i].tileIndex] = doorTriggers[i].currentFrame;
				gam_changeDoorFilters (doorTriggers[i].currentFrame, i);
			}    // end of nextFrame test
		}    // end of inUse is true text
		else
		{    // trigger is not in use
			doorTriggers[i].frameDelay -= doorAnimSpeed * (1.0f / TICKS_PER_SECOND);
			if (doorTriggers[i].frameDelay > 0.0f)
			{
				doorTriggers[i].frameDelay = 1.0f;
				switch (doorTriggers[i].currentFrame)
				{
					case DOOR_ACROSS_OPENED:
//						gam_playDoorSound(i);
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_1;
						evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_ONCE, "doorClose");
						break;
					case DOOR_ACROSS_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_2;
						break;
					case DOOR_ACROSS_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSED;
						break;

					case DOOR_ACROSS_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_1;
						break;
					case DOOR_ACROSS_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_2;
						break;

					case DOOR_UP_OPENED:
//						gam_playDoorSound(i);
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_1;
						evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_ONCE, "doorClose");
						break;
					case DOOR_UP_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_2;
						break;
					case DOOR_UP_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSED;
						break;

					case DOOR_UP_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_1;
						break;
					case DOOR_UP_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_2;
						break;
				}
				shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[i].tileIndex] = doorTriggers[i].currentFrame;
				gam_changeDoorFilters (doorTriggers[i].currentFrame, i);
			}
		}
	}    // end of for each door loop
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render current door frames onto map
void gam_renderDoorFrames ()
//----------------------------------------------------------------------------------------------------------------------
{
	al_set_target_bitmap (gam_getCompleteLevel ());

	for (const auto &doorIndex : doorTriggers)
	{
		if (sys_visibleOnScreen (doorIndex.renderPosition, TILE_SIZE))
		{
			gam_drawSingleTile (doorIndex.renderPosition.x, doorIndex.renderPosition.y, doorIndex.currentFrame);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change the collision filters for a door
// Used by bullet to see if it passes through an open door
void gam_changeDoorFilters (int doorState, int whichDoor)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Fixture *fixture = doorBulletSensor[whichDoor].body->GetFixtureList ();
	//
	// Get the existing filter
	b2Filter  filter   = fixture->GetFilterData ();

	switch (doorState)
	{
		case DOOR_ACROSS_OPENED:
		case DOOR_UP_OPENED:
			filter.categoryBits = PHYSIC_TYPE_DOOR_OPEN;
			break;

		default:
			filter.categoryBits = PHYSIC_TYPE_DOOR_CLOSED;
			break;
	}
	//
	// Set the updated category
	fixture->SetFilterData (filter);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a door bullet sensor
void gam_createDoorBulletSensor (unsigned long whichDoor)
//----------------------------------------------------------------------------------------------------------------------
{
	doorBulletSensor[whichDoor].bodyDef.type = b2_staticBody;
	doorBulletSensor[whichDoor].bodyDef.position.Set (doorBulletSensor[whichDoor].worldPosition.x / pixelsPerMeter, doorBulletSensor[whichDoor].worldPosition.y / pixelsPerMeter);
	doorBulletSensor[whichDoor].body = sys_getPhysicsWorld ()->CreateBody (&doorBulletSensor[whichDoor].bodyDef);

	doorBulletSensor[whichDoor].userData            = new _userData;
	doorBulletSensor[whichDoor].userData->userType  = PHYSIC_TYPE_DOOR_BULLET;
	doorBulletSensor[whichDoor].userData->dataValue = (int) whichDoor;
	doorBulletSensor[whichDoor].body->SetUserData (doorBulletSensor[whichDoor].userData);

	doorBulletSensor[whichDoor].shape.SetAsBox (doorBulletSensor[whichDoor].height / pixelsPerMeter, doorBulletSensor[whichDoor].width / pixelsPerMeter);
	doorBulletSensor[whichDoor].fixtureDef.shape    = &doorBulletSensor[whichDoor].shape;
	doorBulletSensor[whichDoor].fixtureDef.isSensor = false;

	doorBulletSensor[whichDoor].fixtureDef.filter.categoryBits = PHYSIC_TYPE_DOOR_CLOSED;       // Fix door physics not working on start
	doorBulletSensor[whichDoor].fixtureDef.filter.maskBits     = PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY;

	doorBulletSensor[whichDoor].body->CreateFixture (&doorBulletSensor[whichDoor].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Debug door states and current frame
void gam_debugDoors()
//----------------------------------------------------------------------------------------------------------------------
{
	int counter = 0;

	for (auto &doorItr : doorTriggers)
	{
		printf("Door [ %i ] currentFrame : %i InUse : %i \n", counter, doorItr.currentFrame, doorItr.inUse);
		counter++;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory for door triggers
void gam_clearAllDoors ()
//----------------------------------------------------------------------------------------------------------------------
{
	doorTriggers.clear ();

	for (auto &doorBulletItr : doorBulletSensor)
	{
		if (doorBulletItr.userData != nullptr)
			delete (doorBulletItr.userData);
		if (doorBulletItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (doorBulletItr.body);
	}
	doorBulletSensor.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the doors for this level and prepare a sensor object
void gam_doorTriggerSetup (const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	int i           = 0;
	int currentTile = 0;
	int sourceX     = 0;
	int sourceY     = 0;

	_doorTrigger tempDoorTrigger;
	_doorTrigger tempDoorBulletSensor;

	if (!doorTriggers.empty ())  // Empty out array
	{
		gam_clearAllDoors ();
	}

	sourceX = 0.0f;
	sourceY = 0.0f;
	for (i  = 0; i != shipLevel.at (levelName).levelDimensions.x * shipLevel.at (levelName).levelDimensions.y; i++)
	{
		currentTile = shipLevel.at (levelName).tiles[i];
		if (currentTile < 0)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Tile in doorTriggerSetup is invalid - Tile [ %i ]", i));
		}

		switch (currentTile)
		{
			case DOOR_ACROSS_CLOSED:
			case DOOR_ACROSS_OPEN_1:
			case DOOR_ACROSS_OPEN_2:
			case DOOR_ACROSS_OPENED:
			case DOOR_ACROSS_CLOSING_1:
			case DOOR_ACROSS_CLOSING_2:
			{
				tempDoorTrigger.height = TILE_SIZE;
				tempDoorTrigger.width  = TILE_SIZE * 2;

				tempDoorTrigger.topLeft.x = (sourceX * TILE_SIZE) - (TILE_SIZE / 2);
				tempDoorTrigger.topLeft.y = (sourceY * TILE_SIZE) - (TILE_SIZE);

				tempDoorTrigger.topRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);
				tempDoorTrigger.topRight.y = (sourceY * TILE_SIZE) - (TILE_SIZE);

				tempDoorTrigger.botLeft.x = (sourceX * TILE_SIZE) - (TILE_SIZE / 2);
				tempDoorTrigger.botLeft.y = (sourceY * TILE_SIZE) + (TILE_SIZE * 2);

				tempDoorTrigger.botRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);
				tempDoorTrigger.botRight.y = (sourceY * TILE_SIZE) + (TILE_SIZE * 2);

				tempDoorTrigger.worldPosition.x  = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.worldPosition.y  = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.renderPosition.x = sourceX * TILE_SIZE;
				tempDoorTrigger.renderPosition.y = sourceY * TILE_SIZE;
				tempDoorTrigger.tileIndex        = i;
				tempDoorTrigger.frameDelay       = 1.0f;
				tempDoorTrigger.inUse            = false;
				tempDoorTrigger.currentFrame     = DOOR_ACROSS_CLOSED;
				doorTriggers.push_back (tempDoorTrigger);

				tempDoorBulletSensor.height          = TILE_SIZE * 0.5f;
				tempDoorBulletSensor.width           = TILE_SIZE / 4;
				tempDoorBulletSensor.worldPosition.x = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorBulletSensor.worldPosition.y = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorBulletSensor.currentFrame    = DOOR_ACROSS_CLOSED;
				tempDoorBulletSensor.direction       = DIRECTION_ACROSS;
				doorBulletSensor.push_back (tempDoorBulletSensor);

				gam_createDoorBulletSensor (doorBulletSensor.size () - 1);
				break;
			}

			case DOOR_UP_CLOSED:
			case DOOR_UP_OPEN_1:
			case DOOR_UP_OPEN_2:
			case DOOR_UP_OPENED:
			case DOOR_UP_CLOSING_1:
			case DOOR_UP_CLOSING_2:
			{
				tempDoorTrigger.height = TILE_SIZE * 2;
				tempDoorTrigger.width  = TILE_SIZE;

				tempDoorTrigger.topLeft.x = (sourceX * TILE_SIZE) - TILE_SIZE;
				tempDoorTrigger.topLeft.y = (sourceY * TILE_SIZE) - (TILE_SIZE / 2);

				tempDoorTrigger.topRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE * 2);
				tempDoorTrigger.topRight.y = (sourceY * TILE_SIZE) - (TILE_SIZE / 2);

				tempDoorTrigger.botLeft.x = (sourceX * TILE_SIZE) - TILE_SIZE;
				tempDoorTrigger.botLeft.y = (sourceY * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);

				tempDoorTrigger.botRight.x = (sourceX * TILE_SIZE) + (TILE_SIZE * 2);
				tempDoorTrigger.botRight.y = (sourceY * TILE_SIZE) + (TILE_SIZE) + (TILE_SIZE / 2);

				tempDoorTrigger.worldPosition.x  = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.worldPosition.y  = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.renderPosition.x = sourceX * TILE_SIZE;
				tempDoorTrigger.renderPosition.y = sourceY * TILE_SIZE;
				tempDoorTrigger.tileIndex        = i;
				tempDoorTrigger.frameDelay       = 1.0f;
				tempDoorTrigger.inUse            = false;
				tempDoorTrigger.currentFrame     = DOOR_UP_CLOSED;
				doorTriggers.push_back (tempDoorTrigger);

				tempDoorBulletSensor.height          = TILE_SIZE / 4;
				tempDoorBulletSensor.width           = TILE_SIZE * 0.5f;
				tempDoorBulletSensor.worldPosition.x = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorBulletSensor.worldPosition.y = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorBulletSensor.currentFrame    = DOOR_UP_CLOSED;
				tempDoorBulletSensor.direction       = DIRECTION_UP;
				doorBulletSensor.push_back (tempDoorBulletSensor);

				gam_createDoorBulletSensor (doorBulletSensor.size () - 1);
				break;
			}

			default:
				break;
		}
		sourceX++;
		if (sourceX == (int) shipLevel.at (levelName).levelDimensions.x)
		{
			sourceX = 0;
			sourceY++;
		}
	}
}
