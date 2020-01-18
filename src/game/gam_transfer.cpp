#include <hdr/system/sys_eventsEngine.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_transferRender.h>
#include <hdr/game/gam_hud.h>
#include <hdr/game/gam_transferDroidAI.h>
#include <hdr/game/gam_database.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_audio.h>
#include "hdr/game/gam_transfer.h"

int   currentChooseSideTimeLeft = 0;
int   chooseSideTimeOut;  // From script
float chooseSideDelay           = 1.0f;
float chooseSideDelayTime       = 0.0f; // From script

//---------------------------------------------------------------------------------------------------------------------
//
// Return true if a transfer mistake is made
bool trn_makeTransferMistake (int whichDroidLevel)
//---------------------------------------------------------------------------------------------------------------------
{
	int mistakeLevel = 0;

	mistakeLevel = sys_getCappedRandomNum (25);

	mistakeLevel -= whichDroidLevel;

	printf("mistakeLevel [ %i ] - Droid Level [ %i ]\n", mistakeLevel, whichDroidLevel);


	if (mistakeLevel > 0)
		return true;

	return false;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Return the target droid from a collision in transfer mode
int trn_getTransferTargetDroid ()
//---------------------------------------------------------------------------------------------------------------------
{
	return playerDroid.transferTargetDroidType;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Change to transfer game mode
void trn_changeToTransferGame (int targetDroid)
//---------------------------------------------------------------------------------------------------------------------
{
	playerDroid.transferTargetDroidType     = targetDroid;
	keyBinding[gameAction].currentlyPressed = false;
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_STOP_AUDIO, 0, 0, "greenAlert");
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_STOP_AUDIO, 0, 0, "transferMove");
	gui_changeToGUIScreen (gui_findIndex (GUI_OBJECT_SCREEN, "guiTransferOne"));
	sys_changeMode (MODE_GUI_TRANSFER_SCREEN_ONE_LOAD, true);
	gui_hostSetObjectFocus ("buttonTransferOneNext");
	hud_setText (false, "hudCaptured");
	currentChooseSideTimeLeft = chooseSideTimeOut;
	numDroidTokens            = dataBaseEntry[playerDroid.transferTargetDroidType].tokenCount;
	numPlayerTokens           = dataBaseEntry[playerDroid.droidType].tokenCount;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Have selected a side - now start the game
void trn_startTransferGame ()
//---------------------------------------------------------------------------------------------------------------------
{
	// Update HUD
	numDroidTokens  = dataBaseEntry[playerDroid.transferTargetDroidType].tokenCount;
	numPlayerTokens = dataBaseEntry[playerDroid.droidType].tokenCount;
	sys_changeMode (MODE_PLAY_TRANSFER_GAME, false);
	droidBlockPos  = -1;
	playerBlockPos = -1;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Get the current choose side delay counter to display on HUD
int trn_getChooseSideCounter ()
//---------------------------------------------------------------------------------------------------------------------
{
	return currentChooseSideTimeLeft;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Process choose side for countdown
void trn_processChooseSide (float tickTime)
//---------------------------------------------------------------------------------------------------------------------
{
	chooseSideDelay -= (chooseSideDelayTime * tickTime);
	if (chooseSideDelay < 0.0f)
		{
			chooseSideDelay = 1.0f;
			currentChooseSideTimeLeft--;
		}

	if (chooseSideDelay < 0)
		sys_changeMode (MODE_PLAY_TRANSFER_GAME, false);
}

//---------------------------------------------------------------------------------------------------------------------
//
// Handle the keyboard for choosing a side for transfer game
void trn_handleTransferChooseSide ()
//---------------------------------------------------------------------------------------------------------------------
{
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_ONCE, "keyPressGood");

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
			trn_startTransferGame ();
			keyBinding[gameAction].currentlyPressed = false;
			return;
		}
}

//--------------------------------------------------------
//
// Helper function - return true if circuit is a split type
bool trn_isCircuitSplit (int whichType)
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
void trn_setupTransferCellValues ()
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
					printf ("Randnum is larger than row type\n");
					exit (-1);
				}

			if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
				printf ("Got one into two middle\n");

			if ((i == 0) || (i == numberTransferRows - 1))  // Can't have split on first or last slot
				{
					if (trn_isCircuitSplit (randNum))
						transferRows[i].leftSideType = TRANSFER_ROW_FULL_LINE;
					else
						transferRows[i].leftSideType = randNum;
				}
			else if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
				{
					if (trn_isCircuitSplit (transferRows[i - 1].leftSideType))
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
					if (trn_isCircuitSplit (transferRows[i - 1].leftSideType))
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
					if (trn_isCircuitSplit (randNum))
						transferRows[i].rightSideType = TRANSFER_ROW_FULL_LINE;
					else
						transferRows[i].rightSideType = randNum;
				}
			else if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
				{
					if (trn_isCircuitSplit (transferRows[i - 1].rightSideType))
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
					if (trn_isCircuitSplit (transferRows[i].rightSideType))
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

//---------------------------------------------------------------------------------------------------------------------
//
// Process active circuits time
void trn_processCircuits (float tickTime)
//---------------------------------------------------------------------------------------------------------------------
{
	int rowCount = 0;

	for (auto &transferRowIndex : transferRows)
		{
			if (transferRowIndex.rightSideActive)
				{
					transferRowIndex.rightSideActiveCounter -= 0.2f * tickTime;
					if (transferRowIndex.rightSideActiveCounter < 0.0f)
						{
							transferRowIndex.rightSideActiveCounter = 1.0f;
							transferRowIndex.rightSideActive        = false;
						}

//					printf ("transferRowIndex.rightSideActiveAlphaCount [ %f ] Color [ %f ]\n", transferRowIndex.rightSideActiveAlphaCount, transferRowIndex.rightSideActiveAlphaColor);

//					transferRowIndex.rightSideActiveAlphaCount -= 2.5f * tickTime;
//					if (transferRowIndex.rightSideActiveAlphaCount < 0.0f)
					{
						transferRowIndex.rightSideActiveAlphaCount   = 1.0f;
						transferRowIndex.rightSideActiveAlphaColor -= 0.6f * tickTime;
						if (transferRowIndex.rightSideActiveAlphaColor < 0.0f)
							transferRowIndex.rightSideActiveAlphaColor = 1.0f;
					}
				}

			if (transferRowIndex.leftSideActive)
				{
					transferRowIndex.leftSideActiveCounter -= 20.0f * tickTime;
					if (transferRowIndex.leftSideActiveCounter < 0.0f)
						{
							transferRowIndex.leftSideActiveCounter = 1.0f;
							transferRowIndex.leftSideActive        = false;
						}

					transferRowIndex.leftSideActiveAlphaCount -= 2.5f * tickTime;
					if (transferRowIndex.leftSideActiveAlphaCount < 0.0f)
						{
							transferRowIndex.leftSideActiveAlphaCount   = 1.0f;
							transferRowIndex.leftSideActiveAlphaColor -= 0.1f;
							if (transferRowIndex.leftSideActiveAlphaColor < 0.0f)
								transferRowIndex.leftSideActiveAlphaColor = 1.0f;
						}
				}
			rowCount++;
		}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Setup the default values for the transfer rows
void trn_initTransferValues ()
//-------------------------------------------------------------------------------------------------------------------
{
	__transferRow tempTransferRow;

	if (transferRows.empty ())
		{
			for (int i = 0; i != numberTransferRows; i++)
				{
					tempTransferRow.startX                    = transferBitmapWidth / 2;
					tempTransferRow.startY                    = transferRowStartY + (i * transferRowHeight);
					tempTransferRow.leftSideActiveCounter     = 0.0f;
					tempTransferRow.rightSideActiveCounter    = 0.0f;
					tempTransferRow.leftSideType              = TRANSFER_ROW_REVERSE_QUARTER;
					tempTransferRow.rightSideType             = TRANSFER_ROW_REVERSE_QUARTER;
					tempTransferRow.leftSideActive            = false;
					tempTransferRow.rightSideActive           = false;
					tempTransferRow.rightSideActiveIsOn       = false;
					tempTransferRow.leftSideActiveIsOn        = false;
					tempTransferRow.rightSideActiveAlphaColor = 0.0f;
					tempTransferRow.rightSideActiveAlphaCount = 0.0f;
					tempTransferRow.leftSideActiveAlphaColor  = 0.0f;
					tempTransferRow.leftSideActiveAlphaCount  = 0.0f;

					if (i % 2 == 0)
						tempTransferRow.currentColor = TRANSFER_COLOR_LEFT;
					else
						tempTransferRow.currentColor = TRANSFER_COLOR_RIGHT;

					transferRows.push_back (tempTransferRow);
				}

			transferGameBitmap = al_create_bitmap (transferBitmapWidth, transferBitmapHeight);
			if (nullptr == transferGameBitmap)
				log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to create backing bitmap for transfer game."));

			al_set_target_bitmap (transferGameBitmap);
		}

	transferColorStatusCell = transferColorBorder;
	transferPlayerWhichSide = TRANSFER_COLOR_LEFT;

	activeTokenPosX = BLOCK_WIDTH * 3;

	transferColorLeftActive  = al_map_rgba_f (1.0f, 0.0f, 0.0f, 1.0f);
	transferColorRightActive = al_map_rgba_f (1.0f, 1.0f, 0.0f, 0.1f);

	trn_setupTransferCellValues ();
}
