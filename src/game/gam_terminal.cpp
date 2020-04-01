#include <hdr/game/gam_lifts.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_resources.h>
#include <hdr/system/sys_init.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_hud.h>
#include "hdr/game/gam_terminal.h"

std::vector<__liftSensor> terminals;
float deckViewRatio;

//----------------------------------------------------------------------------------------------------------------------
//
// Create a terminal sensor
void gam_createTerminalSensor (unsigned long whichTerminal, int index)
//----------------------------------------------------------------------------------------------------------------------
{
	terminals[whichTerminal].bodyDef.type = b2_staticBody;
	terminals[whichTerminal].bodyDef.position.Set (terminals[whichTerminal].worldPosition.x / pixelsPerMeter, terminals[whichTerminal].worldPosition.y / pixelsPerMeter);
	terminals[whichTerminal].body = sys_getPhysicsWorld ()->CreateBody (&terminals[whichTerminal].bodyDef);

	terminals[whichTerminal].userData            = new _userData;
	terminals[whichTerminal].userData->userType  = PHYSIC_TYPE_TERMINAL;
	terminals[whichTerminal].userData->dataValue = (int) index;
	terminals[whichTerminal].body->SetUserData (terminals[whichTerminal].userData);

	terminals[whichTerminal].shape.SetAsBox ((terminals[whichTerminal].height) / pixelsPerMeter, (terminals[whichTerminal].width) / pixelsPerMeter);
	terminals[whichTerminal].fixtureDef.shape    = &terminals[whichTerminal].shape;
	terminals[whichTerminal].fixtureDef.isSensor = true;
	terminals[whichTerminal].body->CreateFixture (&terminals[whichTerminal].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory and free bodies
void gam_clearTerminals ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &terminalItr : terminals)
		{
			if (terminalItr.userData != nullptr)
				delete (terminalItr.userData);
			if (terminalItr.body != nullptr)
				sys_getPhysicsWorld ()->DestroyBody (terminalItr.body);
		}
	terminals.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the positions of terminals on the current level
void gam_findTerminalPositions (const std::string &levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	__liftSensor tempTerminal;

	int countX        = 0;
	int countY        = 0;
	int currentTile   = 0;
	int countTerminal = 0;

	countTerminal = 0;
	countX        = 0;
	countY        = 0;

	if (!lifts.empty ())
		{
			gam_clearTerminals ();
		}

	for (int index = 0; index < shipLevel.at (levelName).levelDimensions.x * shipLevel.at (levelName).levelDimensions.y; index++)
		{
			currentTile = shipLevel.at (levelName).tiles[((countY * (shipLevel.at (levelName).levelDimensions.x)) + countX)];

			if ((TERMINAL_BOTTOM == currentTile) ||
			    (TERMINAL_LEFT == currentTile) ||
			    (TERMINAL_RIGHT == currentTile) ||
			    (TERMINAL_TOP == currentTile))
				{
					tempTerminal.worldPosition.x = (countX * TILE_SIZE) + (TILE_SIZE * 0.5f);
					tempTerminal.worldPosition.y = (countY * TILE_SIZE) + (TILE_SIZE * 0.5f);

					tempTerminal.width  = TILE_SIZE / 8;
					tempTerminal.height = TILE_SIZE / 8;

					terminals.push_back (tempTerminal);

					gam_createTerminalSensor (terminals.size () - 1, countTerminal);

					countTerminal++;
				}

			countX++;

			if (countX == shipLevel.at (levelName).levelDimensions.x)
				{
					countX = 0;
					countY++;
				}
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pressed the action key while over a terminal tile
void gam_performTerminalAction ()
//----------------------------------------------------------------------------------------------------------------------
{
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_STOP_AUDIO, 0, 0, "greenAlert"); // TODO: Change to current alert
	gui_changeToGUIScreen (gui_findIndex (GUI_OBJECT_SCREEN, "mainTerminalScreen"));
	sys_changeMode (MODE_GUI_TERMINAL, true);
	gui_hostSetObjectFocus ("terminalLogoffButton");
	hud_setText (false, "hudTerminal");
}
