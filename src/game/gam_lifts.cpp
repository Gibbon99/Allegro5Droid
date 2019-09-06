#include <hdr/gui/gui_sideView.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_logFile.h>
#include "hdr/game/gam_lifts.h"

std::vector<__liftSensor> lifts;

//----------------------------------------------------------------------------
//
// Activate the lift
void gam_performLiftAction()
//----------------------------------------------------------------------------
{
	currentDeckNumber = lvl_getDeckNumber (lvl_getCurrentLevelName ());
	if (-1 == currentDeckNumber)
	{
		log_logMessage (LOG_LEVEL_EXIT, sys_getString("Could not locate deck number to move lift."));
		return;
	}

	gam_getTunnelToUse ();
	sys_changeMode (MODE_LIFT_VIEW, true);
}

//----------------------------------------------------------------------------
//
// Get the current deck we are on and are moving around on
void gam_getCurrentDeck ( int checkLevel )
// ----------------------------------------------------------------------------
{
	int i;
	int topDeck = tunnel[currentTunnel].top;

	//
	// Look for the matching deck and current level
	//
	for ( i = 0; i != 10; i++ ) // TODO - why 10?
	{
		if ( checkLevel == tunnel[currentTunnel].decks[i] )
		{
			//
			// Found a match on this tunnel
			//
			tunnel[currentTunnel].current_deck = i;
			return;
		}

		if ( topDeck == tunnel[currentTunnel].decks[i] )
		{
			//
			// we have a problem - didn't find a matching level for this tunnel
			log_logMessage (LOG_LEVEL_EXIT, sys_getString("Error: Couldn't find a matching level for tunnel. Game in unstable state" ));
		}
	}
}

// ----------------------------------------------------------------------------
//
// Get the tunnel being used by the lift the player sprite is over
void gam_getTunnelToUse ()
// ----------------------------------------------------------------------------
{
	auto levelIndexItr = lvl_getLevelIndex ( lvl_getCurrentLevelName ());

	currentTunnel = levelIndexItr->second.lifts[playerDroid.liftIndex].tunnel;
	gam_getCurrentDeck ( levelIndexItr->second.deckNumber );

	return;

	switch ( levelIndexItr->second.numLifts )
	{
		case 1: // only one lift on these levels
			currentTunnel = levelIndexItr->second.lifts[0].tunnel;
			break;

		case 2: // two lifts on these levels
			if (((playerDroid.middlePosition.x) > levelIndexItr->second.lifts[0].posX - TILE_SIZE) &&
			    ((playerDroid.middlePosition.x) < levelIndexItr->second.lifts[0].posX + TILE_SIZE))
			{
				currentTunnel = levelIndexItr->second.lifts[0].tunnel;
			}
			else
			{
				currentTunnel = levelIndexItr->second.lifts[1].tunnel;
			}

			break;

		case 3: // only two levels with three lifts on them
			if ( levelIndexItr->second.deckNumber == 12 )
			{
				if ((playerDroid.middlePosition.x) > levelIndexItr->second.lifts[0].posX - TILE_SIZE )
				{
					currentTunnel = levelIndexItr->second.lifts[0].tunnel;
					gam_getCurrentDeck ( levelIndexItr->second.deckNumber );
					return;
				}

				if (((playerDroid.middlePosition.y) > levelIndexItr->second.lifts[1].posY - TILE_SIZE) &&
				    ((playerDroid.middlePosition.y) < levelIndexItr->second.lifts[1].posY + TILE_SIZE))
				{
					currentTunnel = levelIndexItr->second.lifts[1].tunnel;
					gam_getCurrentDeck ( levelIndexItr->second.deckNumber );
					return;
				}

				if ((playerDroid.middlePosition.y) > levelIndexItr->second.lifts[2].posY - TILE_SIZE )
				{
					currentTunnel = levelIndexItr->second.lifts[2].tunnel;
					gam_getCurrentDeck ( levelIndexItr->second.deckNumber );
					return;
				}
			} // end of if level is 12 test
			else
			{
				if ( levelIndexItr->second.deckNumber == 14 )
				{
					if (((playerDroid.middlePosition.x) > lifts[2].worldPosition.x - TILE_SIZE) &&
					    ((playerDroid.middlePosition.x) < lifts[2].worldPosition.x + TILE_SIZE))
					{
						currentTunnel = levelIndexItr->second.lifts[2].tunnel;
						gam_getCurrentDeck ( levelIndexItr->second.deckNumber );
						return;
					}

					if (((playerDroid.middlePosition.x) > levelIndexItr->second.lifts[1].posX - TILE_SIZE) &&
					    ((playerDroid.middlePosition.x) < levelIndexItr->second.lifts[1].posX + TILE_SIZE))
					{
						currentTunnel = levelIndexItr->second.lifts[1].tunnel;
						gam_getCurrentDeck ( levelIndexItr->second.deckNumber );
						return;
					}

					if (((playerDroid.middlePosition.x) > levelIndexItr->second.lifts[0].posX - TILE_SIZE) &&
					    ((playerDroid.middlePosition.x) < levelIndexItr->second.lifts[0].posX + TILE_SIZE))
					{
						currentTunnel = levelIndexItr->second.lifts[0].tunnel;
						gam_getCurrentDeck ( levelIndexItr->second.deckNumber );
						return;
					}
				} // end of is level 14 test
			} // end of else statement
	} // end of switch statement

	printf("Tunnel in use [ %i ]\n", currentTunnel );

	//
	// Get the current deck pointer
	gam_getCurrentDeck ( levelIndexItr->second.deckNumber );
} // end of function

//----------------------------------------------------------------------------
//
// Put the player onto lift on new deck
int gam_putPlayerOnLiftFromTunnel(int currentDeck)
// ----------------------------------------------------------------------------
{
	switch (currentTunnel)
	{
		case 0:
			return 0;
			break;

		case 1:
			return 0;
			break;

		case 2:
		{
			switch (currentDeck)
			{
				case 17:
					return 0;
					break;

				case 15:
					return 1;
					break;

				default:
					break;
			}
			break;
		}

		case 3:
		{
			switch (currentDeck)
			{
				case 12:
				case 13:
				case 14:
				case 17:
					return 1;
					break;

				case 20:
					return 0;
					break;

				default:
					break;
			}
		}

		case 4:
		{
			switch (currentDeck)
			{
				case 12:
					return 2;
					break;

				case 6:
				case 5:
					return 1;
					break;

				case 1:
					return 0;
					break;

				default:
					break;
			}
		}

		case 5:
			return 0;
			break;

		case 6:
		{
			switch (currentDeck)
			{
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					return 0;
					break;

				case 9:
					return 1;
					break;

				default:
					break;
			}
		}

		case 7:
		{
			switch (currentDeck)
			{
				case 14:
					return 2;
					break;

				case 18:
					return 0;
					break;

				default:
					break;
			}
		}

		default:
			break;
	}

	printf("ERROR: Should not be here\n");

	return -1;
} // end of function


// ----------------------------------------------------------------------------
//
// Move the lift position
void gam_moveLift(int direction)
// ----------------------------------------------------------------------------
{
	printf("Move lift [ %i ]\n", direction);

	switch (direction)
	{
		case 1: //UP
			if (currentDeckNumber != tunnel[currentTunnel].top)
			{
				tunnel[currentTunnel].current_deck++;
				tunnel[currentTunnel].current = tunnel[currentTunnel].decks[tunnel[currentTunnel].current_deck];
				currentDeckNumber = tunnel[currentTunnel].current;
//				evt_sendEvent (USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_KEYPRESS_GOOD, 0, 0, glm::vec2(), glm::vec2(), "");
			}
			else
			{
//				evt_sendEvent (USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_KEYPRESS_BAD, 0, 0, glm::vec2(), glm::vec2(), "");
			}

			break;

		case 2: //DOWN
			if (currentDeckNumber != tunnel[currentTunnel].bottom)
			{
				tunnel[currentTunnel].current_deck--;
				tunnel[currentTunnel].current = tunnel[currentTunnel].decks[tunnel[currentTunnel].current_deck];
				currentDeckNumber = tunnel[currentTunnel].current;
//				evt_sendEvent (USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_KEYPRESS_GOOD, 0, 0, glm::vec2(), glm::vec2(), "");
			}
			else
			{
//				evt_sendEvent (USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_KEYPRESS_BAD, 0, 0, glm::vec2(), glm::vec2(), "");
			}

			break;

		case 3: // ACTION

			lvl_changeToLevel (lvl_returnLevelNameFromDeck (currentDeckNumber), gam_putPlayerOnLiftFromTunnel (currentDeckNumber));

//			evt_sendEvent (USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_KEYPRESS_GOOD, 0, 0, glm::vec2(), glm::vec2(), "");

			sys_changeMode ( MODE_GAME, true );

			break;

		default:
			break;
	}
}


//----------------------------------------------------------------------------------------------------------------------
//
// Create a door sensor
void gam_createLiftSensor(unsigned long whichLift, int index)
//----------------------------------------------------------------------------------------------------------------------
{
	lifts[whichLift].bodyDef.type = b2_staticBody;
	lifts[whichLift].bodyDef.position.Set(lifts[whichLift].worldPosition.x / pixelsPerMeter,lifts[whichLift].worldPosition.y / pixelsPerMeter);
	lifts[whichLift].body = sys_getPhysicsWorld()->CreateBody(&lifts[whichLift].bodyDef);

	lifts[whichLift].userData            = new _userData;
	lifts[whichLift].userData->userType  = PHYSIC_TYPE_LIFT;
	lifts[whichLift].userData->dataValue = (int) index;
	lifts[whichLift].body->SetUserData(lifts[whichLift].userData);

	lifts[whichLift].shape.SetAsBox((lifts[whichLift].height) / pixelsPerMeter,	(lifts[whichLift].width) / pixelsPerMeter);
	lifts[whichLift].fixtureDef.shape    = &lifts[whichLift].shape;
	lifts[whichLift].fixtureDef.isSensor = true;
	lifts[whichLift].body->CreateFixture(&lifts[whichLift].fixtureDef);
}

//---------------------------------------------------------
//
// Get the positions of lifts
void gam_findLiftPositions ( const std::string &levelName )
//---------------------------------------------------------
{
	__liftSensor tempLift;

	int countX = 0;
	int countY = 0;
	int currentTile = 0;
	int countLift = 0;

	countLift = 0;
	countX = 0;
	countY = 0;

	if (!lifts.empty())
	{
		for (auto liftItr : lifts)
		{
			sys_getPhysicsWorld ()->DestroyBody (liftItr.body);
		}
		lifts.clear();
	}

	for (int index = 0; index < shipLevel.at(levelName).levelDimensions.x * shipLevel.at(levelName).levelDimensions.y; index++)
	{
		currentTile = shipLevel.at(levelName).tiles[ ( ( countY * ( shipLevel.at(levelName).levelDimensions.x ) ) + countX )];

		if ( LIFT_TILE == currentTile )
		{
			tempLift.worldPosition.x = (countX * TILE_SIZE) + (TILE_SIZE * 0.5f);
			tempLift.worldPosition.y = (countY * TILE_SIZE) + (TILE_SIZE * 0.5f);

			tempLift.width = TILE_SIZE / 8;
			tempLift.height = TILE_SIZE / 8;

			lifts.push_back(tempLift);

			gam_createLiftSensor(lifts.size() - 1, countLift);

			countLift++;
		}

		countX++;

		if ( countX == shipLevel.at(levelName).levelDimensions.x )
		{
			countX = 0;
			countY++;
		}
	}
}


//---------------------------------------------------------
//
// Store the lift linking information into vector array
//
// Call a second time to add to vector on same level
void gam_setupLiftsStore ( const std::string &whichLevel, int whichTunnel )
//---------------------------------------------------------
{
	_liftBasic tempLift;

	tempLift.tunnel = whichTunnel;

//	printf("level [ %s ] Tunnel [ %i ] numLifts [ %i ]\n", whichLevel.c_str(), whichTunnel, shipLevel.at(whichLevel).numLifts);

	shipLevel.at(whichLevel).lifts.push_back(tempLift);
}

//---------------------------------------------------------
//
// Setup lifts
void gam_setupLifts ()
//---------------------------------------------------------
{
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 0 ), 1 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 1 ), 4 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 2 ), 5 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 3 ), 0 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 4 ), 0 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 5 ), 1 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 5 ), 4 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 6 ), 1 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 6 ), 4 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 8 ), 5 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 9 ), 5 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 9 ), 6 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 10 ), 6 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 11 ), 6 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 12 ), 6 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 12 ), 3 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 12 ), 4 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 13 ), 6 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 13 ), 3 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 14 ), 6 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 14 ), 3 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 14 ), 7 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 15 ), 0 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 15 ), 2 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 16 ), 0 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 17 ), 2 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 17 ), 3 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 18 ), 7 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 19 ), 0 );
	gam_setupLiftsStore ( lvl_returnLevelNameFromDeck ( 20 ), 3 );

	/*
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 0 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 1 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 2 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 3 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 4 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 5 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 6 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 8 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 9 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 10 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 11 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 12 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 13 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 14 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 15 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 16 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 17 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 18 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 19 ));
	gam_findLiftPositions ( lvl_returnLevelNameFromDeck ( 20 ));
	*/
}