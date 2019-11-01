#include <hdr/system/sys_eventsEngine.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_transferRender.h>
#include <hdr/game/gam_hud.h>
#include "hdr/game/gam_transfer.h"

//---------------------------------------------------------------------------------------------------------------------
//
// Return the target droid from a collision in transfer mode
int gam_getTransferTargetDroid ()
//---------------------------------------------------------------------------------------------------------------------
{
	return playerDroid.transferTargetDroidType;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Change to transfer game mode
void gam_changeToTransferGame (int targetDroid)
//---------------------------------------------------------------------------------------------------------------------
{
	playerDroid.transferTargetDroidType     = targetDroid;
	keyBinding[gameAction].currentlyPressed = false;
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_STOP_AUDIO, 0, 0, "greenAlert");
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_STOP_AUDIO, 0, 0, "transferMove");
	gui_changeToGUIScreen (gui_findIndex (GUI_OBJECT_SCREEN, "guiTransferOne"));
	sys_changeMode (MODE_GUI_TRANSFER_SCREEN_ONE_LOAD, true);
	gui_hostSetObjectFocus ("buttonTransferOneNext");
	hud_setText ("hudCaptured");
}

//---------------------------------------------------------------------------------------------------------------------
//
// Have selected a side - now start the game
void trn_startTransferGame()
//---------------------------------------------------------------------------------------------------------------------
{
	// Update HUD
	// Start transfer timer countdown
	sys_changeMode (MODE_PLAY_TRANSFER_GAME, false);
}

//---------------------------------------------------------------------------------------------------------------------
//
// Handle the keyboard for choosing a side for transfer game
void trn_handleTransferChooseSide ()
//---------------------------------------------------------------------------------------------------------------------
{
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 0, 0, "keyPressGood");

	if (keyBinding[gameLeft].currentlyPressed)
		{
			transferPlayerWhichSide = TRANSFER_COLOR_LEFT;
			keyBinding[gameLeft].currentlyPressed = false;
			return;
		}

	if (keyBinding[gameRight].currentlyPressed)
		{
			transferPlayerWhichSide = TRANSFER_COLOR_RIGHT;
			keyBinding[gameRight].currentlyPressed = false;
			return;
		}

	if (keyBinding[gameAction].currentlyPressed)
		{
			trn_startTransferGame();
			keyBinding[gameAction].currentlyPressed = false;
			return;
		}
}


//--------------------------------------------------------
//
// Helper function - return true if circuit is a split type
bool isCircuitSplit (int whichType)
//--------------------------------------------------------
{
	return (whichType == TRANSFER_ROW_TWO_INTO_ONE_MIDDLE) ||
	       (whichType == TRANSFER_ROW_ONE_INTO_TWO_MIDDLE) ||
	       (whichType == TRANSFER_ROW_TWO_INTO_ONE_BOTTOM) ||
	       (whichType == TRANSFER_ROW_TWO_INTO_ONE_TOP) ||
	       (whichType == TRANSFER_ROW_ONE_INTO_TWO_TOP) ||
	       (whichType == TRANSFER_ROW_ONE_INTO_TWO_BOTTOM);
}

//------------------------------------------------------------
//
// Called to setup the transfer cell values
void setupTransferCellValues()
//------------------------------------------------------------
{
	int  i          = 0;
	int  randNum    = 0;
	int  randEffect = 0;     // Will the circuit be reversed or repeater
	bool validCircuit;

	for (i = 0; i != numberTransferRows; i++)
		{
//
// Left side
//
			randNum = sys_getCappedRandomNum (TRANSFER_ROW_FULL_LINE_3);
			randNum = randNum + 1;

			if (randNum > TRANSFER_ROW_FULL_LINE_3)
				{
					printf("Randnum is larger than row type\n");
					exit(-1);
				}

			if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
				printf("Got one into two middle\n");

			if ((i == 0) || (i == numberTransferRows - 1))  // Can't have split on first or last slot
				{
					if (isCircuitSplit (randNum))
						transferRows[i].leftSideType = TRANSFER_ROW_FULL_LINE;
					else
						transferRows[i].leftSideType = randNum;
				}
			else if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
				{
					if (isCircuitSplit (transferRows[i - 1].leftSideType))
						transferRows[i].leftSideType = TRANSFER_ROW_FULL_LINE;
					else
						{
							transferRows[i].leftSideType     = randNum;
							transferRows[i - 1].leftSideType = TRANSFER_ROW_ONE_INTO_TWO_TOP;
							i++;
							transferRows[i].leftSideType = TRANSFER_ROW_ONE_INTO_TWO_BOTTOM;
						}
				}
			else if (TRANSFER_ROW_TWO_INTO_ONE_MIDDLE == randNum)
				{
					if (isCircuitSplit (transferRows[i - 1].leftSideType))
						transferRows[i].leftSideType = TRANSFER_ROW_FULL_LINE;
					else
						{
							transferRows[i].leftSideType     = randNum;
							transferRows[i - 1].leftSideType = TRANSFER_ROW_TWO_INTO_ONE_TOP;
							i++;
							transferRows[i].leftSideType = TRANSFER_ROW_TWO_INTO_ONE_BOTTOM;
						}
				}
			else // Ok to use non split one here
				{
					transferRows[i].leftSideType = randNum;
				}
		}
//
// Right side
//

	for (i = 0; i != numberTransferRows; i++)
		{
//			as_transSetDefaultValues (i);

			randNum = sys_getCappedRandomNum (TRANSFER_ROW_FULL_LINE_3);
			randNum = randNum + 1;

			if ((i == 0) || (i == numberTransferRows - 1))  // Can't have split on first or last slot
				{
					if (isCircuitSplit (randNum))
						transferRows[i].rightSideType = TRANSFER_ROW_FULL_LINE;
					else
						transferRows[i].rightSideType = randNum;
				}
			else if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
				{
					if (isCircuitSplit (transferRows[i - 1].rightSideType))
						transferRows[i].rightSideType = TRANSFER_ROW_FULL_LINE;
					else
						{
							transferRows[i].rightSideType     = randNum;
							transferRows[i - 1].rightSideType = TRANSFER_ROW_ONE_INTO_TWO_TOP;
							i++;
							transferRows[i].rightSideType = TRANSFER_ROW_ONE_INTO_TWO_BOTTOM;
						}
				}
			else if (TRANSFER_ROW_TWO_INTO_ONE_MIDDLE == randNum)
				{
					if (isCircuitSplit (transferRows[i].rightSideType))
						transferRows[i].rightSideType = TRANSFER_ROW_FULL_LINE;
					else
						{
							transferRows[i].rightSideType     = randNum;
							transferRows[i - 1].rightSideType = TRANSFER_ROW_TWO_INTO_ONE_TOP;
							i++;
							transferRows[i].rightSideType = TRANSFER_ROW_TWO_INTO_ONE_BOTTOM;
						}
				}
			else // Ok to use non split one here
				{
					transferRows[i].rightSideType = randNum;
				}
		}
}
//
//---------------------------------------------------------------------------------------------------------------------
//
// Play the tranfer game
void trn_processTransferGame(int tickTime)
//---------------------------------------------------------------------------------------------------------------------
{
	// Move droid tokens
	// Check cell status and update colors
	// Check cell timers for energy stop

}