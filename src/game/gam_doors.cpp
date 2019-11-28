#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_render.h>
#include "hdr/game/gam_doors.h"

std::vector<_doorTrigger> doorTriggers;
std::vector<_doorTrigger> doorBulletSensor;

float doorAnimSpeed = 1.0f;         // From script

//----------------------------------------------------------------------------------------------------------------------
//
// Process the animation of a door
void gam_animateDoor (int whichDoor, int state)
//----------------------------------------------------------------------------------------------------------------------
{
	int doorDelayTime = 0;

	doorTriggers[whichDoor].frameDelay -= doorAnimSpeed * (1.0f / TICKS_PER_SECOND);
	if (doorTriggers[whichDoor].frameDelay > 0.0f)
		{
			evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, state, "");
			return;
		}
	doorTriggers[whichDoor].frameDelay = 1.0f;

	if (state == GAME_DOOR_STATE_ENTER)
		{
			switch (doorTriggers[whichDoor].currentFrame)
				{
					case DOOR_ACROSS_CLOSED:
						doorTriggers[whichDoor].currentFrame   = DOOR_ACROSS_OPEN_1;
					doorBulletSensor[whichDoor].currentFrame = DOOR_ACROSS_OPEN_1;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
					gam_changeDoorFilters (DOOR_ACROSS_OPEN_1, whichDoor);
					break;

					case DOOR_ACROSS_OPEN_1:
						doorTriggers[whichDoor].currentFrame   = DOOR_ACROSS_OPEN_2;
					doorBulletSensor[whichDoor].currentFrame = DOOR_ACROSS_OPEN_2;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
					gam_changeDoorFilters (DOOR_ACROSS_OPEN_2, whichDoor);
					break;

					case DOOR_ACROSS_OPEN_2:
						doorTriggers[whichDoor].currentFrame   = DOOR_ACROSS_OPENED;
					doorBulletSensor[whichDoor].currentFrame = DOOR_ACROSS_OPENED;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
					gam_changeDoorFilters (DOOR_ACROSS_OPENED, whichDoor);
					break;

					case DOOR_UP_CLOSED:
						doorTriggers[whichDoor].currentFrame   = DOOR_UP_OPEN_1;
					doorBulletSensor[whichDoor].currentFrame = DOOR_UP_OPEN_1;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
					gam_changeDoorFilters (DOOR_UP_OPEN_1, whichDoor);
					break;

					case DOOR_UP_OPEN_1:
						doorTriggers[whichDoor].currentFrame   = DOOR_UP_OPEN_2;
					doorBulletSensor[whichDoor].currentFrame = DOOR_UP_OPEN_2;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
					gam_changeDoorFilters (DOOR_UP_OPEN_2, whichDoor);
					break;

					case DOOR_UP_OPEN_2:
						doorTriggers[whichDoor].currentFrame   = DOOR_UP_OPENED;
					doorBulletSensor[whichDoor].currentFrame = DOOR_UP_OPENED;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
					gam_changeDoorFilters (DOOR_UP_OPENED, whichDoor);
					break;
				}
		}

	if (state == GAME_DOOR_STATE_EXIT)
		{
			if (doorTriggers[whichDoor].numberUsing > 0)
				{
					return;
				}

			switch (doorTriggers[whichDoor].currentFrame)
				{
					case DOOR_ACROSS_OPENED:
						doorTriggers[whichDoor].currentFrame   = DOOR_ACROSS_CLOSING_1;
					doorBulletSensor[whichDoor].currentFrame = DOOR_ACROSS_CLOSING_1;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
					gam_changeDoorFilters (DOOR_ACROSS_CLOSING_1, whichDoor);
					break;

					case DOOR_ACROSS_CLOSING_1:
						doorTriggers[whichDoor].currentFrame   = DOOR_ACROSS_CLOSING_2;
					doorBulletSensor[whichDoor].currentFrame = DOOR_ACROSS_CLOSING_2;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
					gam_changeDoorFilters (DOOR_ACROSS_CLOSING_2, whichDoor);
					break;

					case DOOR_ACROSS_CLOSING_2:
						doorTriggers[whichDoor].currentFrame   = DOOR_ACROSS_CLOSED;
					doorBulletSensor[whichDoor].currentFrame = DOOR_ACROSS_CLOSED;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
					gam_changeDoorFilters (DOOR_ACROSS_CLOSED, whichDoor);
					break;

					case DOOR_UP_OPENED:
						doorTriggers[whichDoor].currentFrame   = DOOR_UP_CLOSING_1;
					doorBulletSensor[whichDoor].currentFrame = DOOR_UP_CLOSING_1;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
					gam_changeDoorFilters (DOOR_UP_CLOSING_1, whichDoor);
					break;

					case DOOR_UP_CLOSING_1:
						doorTriggers[whichDoor].currentFrame   = DOOR_UP_CLOSING_2;
					doorBulletSensor[whichDoor].currentFrame = DOOR_UP_CLOSING_2;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
					gam_changeDoorFilters (DOOR_UP_CLOSING_2, whichDoor);
					break;

					case DOOR_UP_CLOSING_2:
						doorTriggers[whichDoor].currentFrame   = DOOR_UP_CLOSED;
					doorBulletSensor[whichDoor].currentFrame = DOOR_UP_CLOSED;
					shipLevel.at (lvl_getCurrentLevelName ()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
					evt_pushEvent (doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
					gam_changeDoorFilters (DOOR_UP_CLOSED, whichDoor);
					break;
				}
		}
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
// Handle door sensor trigger
void gam_handleDoorTrigger (int whichDoor, int state)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (state)
		{
			case GAME_DOOR_STATE_ENTER:
				doorTriggers[whichDoor].numberUsing++;
			evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
			// play door sound
			break;

			case GAME_DOOR_STATE_EXIT:
				doorTriggers[whichDoor].numberUsing--;
			evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
			// play door sound
			break;
		}

}

//----------------------------------------------------------------------------------------------------------------------
//
// Change the collision filters for a door
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

	doorBulletSensor[whichDoor].fixtureDef.filter.categoryBits = PHYSIC_TYPE_DOOR_BULLET;
	doorBulletSensor[whichDoor].fixtureDef.filter.maskBits     = PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY;

	doorBulletSensor[whichDoor].body->CreateFixture (&doorBulletSensor[whichDoor].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a door sensor
void gam_createDoorSensor (unsigned long whichDoor, int index)
//----------------------------------------------------------------------------------------------------------------------
{
	doorTriggers[whichDoor].bodyDef.type = b2_staticBody;
	doorTriggers[whichDoor].bodyDef.position.Set (doorTriggers[whichDoor].worldPosition.x / pixelsPerMeter, doorTriggers[whichDoor].worldPosition.y / pixelsPerMeter);
	doorTriggers[whichDoor].bodyDef.active = true;
	doorTriggers[whichDoor].bodyDef.allowSleep = false;
	doorTriggers[whichDoor].body = sys_getPhysicsWorld ()->CreateBody (&doorTriggers[whichDoor].bodyDef);

	doorTriggers[whichDoor].body->SetAwake (true);

	doorTriggers[whichDoor].userData            = new _userData;
	doorTriggers[whichDoor].userData->userType  = PHYSIC_TYPE_DOOR_CLOSED;
	doorTriggers[whichDoor].userData->dataValue = (int) whichDoor;
	doorTriggers[whichDoor].body->SetUserData (doorTriggers[whichDoor].userData);

	doorTriggers[whichDoor].shape.SetAsBox ((doorTriggers[whichDoor].height * 0.5f) / pixelsPerMeter, (doorTriggers[whichDoor].width * 0.5f) / pixelsPerMeter);
	doorTriggers[whichDoor].fixtureDef.shape    = &doorTriggers[whichDoor].shape;
	doorTriggers[whichDoor].fixtureDef.isSensor = true;

	doorTriggers[whichDoor].body->CreateFixture (&doorTriggers[whichDoor].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory for door triggers
void gam_clearAllDoors ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto doorItr : doorTriggers)
		{
			if (doorItr.userData != nullptr)
				delete (doorItr.userData);
			if (doorItr.body != nullptr)
				sys_getPhysicsWorld ()->DestroyBody (doorItr.body);
		}
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
							tempDoorTrigger.height           = TILE_SIZE;
							tempDoorTrigger.width            = TILE_SIZE * 2;
							tempDoorTrigger.worldPosition.x  = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorTrigger.worldPosition.y  = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorTrigger.renderPosition.x = sourceX * TILE_SIZE;
							tempDoorTrigger.renderPosition.y = sourceY * TILE_SIZE;
							tempDoorTrigger.tileIndex        = i;
							tempDoorTrigger.numberUsing      = 0;
							tempDoorTrigger.frameDelay       = doorAnimSpeed;
							tempDoorTrigger.currentFrame     = DOOR_ACROSS_CLOSED;
							doorTriggers.push_back (tempDoorTrigger);

							tempDoorBulletSensor.height          = TILE_SIZE * 0.5f;
							tempDoorBulletSensor.width           = TILE_SIZE / 4;
							tempDoorBulletSensor.worldPosition.x = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorBulletSensor.worldPosition.y = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorBulletSensor.currentFrame    = DOOR_ACROSS_CLOSED;
							tempDoorBulletSensor.direction       = DIRECTION_ACROSS;
							doorBulletSensor.push_back (tempDoorBulletSensor);

							gam_createDoorSensor (doorTriggers.size () - 1, i);
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
							tempDoorTrigger.height           = TILE_SIZE * 2;
							tempDoorTrigger.width            = TILE_SIZE;
							tempDoorTrigger.worldPosition.x  = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorTrigger.worldPosition.y  = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorTrigger.renderPosition.x = sourceX * TILE_SIZE;
							tempDoorTrigger.renderPosition.y = sourceY * TILE_SIZE;
							tempDoorTrigger.tileIndex        = i;
							tempDoorTrigger.numberUsing      = 0;
							tempDoorTrigger.frameDelay       = doorAnimSpeed;
							tempDoorTrigger.currentFrame     = DOOR_UP_CLOSED;
							doorTriggers.push_back (tempDoorTrigger);

							tempDoorBulletSensor.height          = TILE_SIZE / 4;
							tempDoorBulletSensor.width           = TILE_SIZE * 0.5f;
							tempDoorBulletSensor.worldPosition.x = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorBulletSensor.worldPosition.y = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
							tempDoorBulletSensor.currentFrame    = DOOR_UP_CLOSED;
							tempDoorBulletSensor.direction       = DIRECTION_UP;
							doorBulletSensor.push_back (tempDoorBulletSensor);

							gam_createDoorSensor (doorTriggers.size () - 1, i);
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
