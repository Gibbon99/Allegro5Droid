#include <hdr/gui/gui_text.h>
#include <hdr/game/gam_lifts.h>
#include <hdr/game/gam_player.h>
#include <hdr/gui/gui_sideView.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_transfer.h>
#include <hdr/game/gam_hud.h>
#include "hdr/io/io_keyboard.h"

__KeyBindings keyBinding[NUMBER_ACTIONS];

//----------------------------------------------------------------------------------------------------------------------
//
// Assign the text description for each key
void io_setKeyDescription ()
//----------------------------------------------------------------------------------------------------------------------
{
	keyBinding[gameLeft].text       = gui_getString ("gameLeft");
	keyBinding[gameRight].text      = gui_getString ("gameRight");
	keyBinding[gameDown].text       = gui_getString ("gameDown");
	keyBinding[gameUp].text         = gui_getString ("gameUp");
	keyBinding[gamePause].text      = gui_getString ("gamePause");
	keyBinding[gameAction].text     = gui_getString ("gameAction");
	keyBinding[gameEscape].text     = gui_getString ("gameEscape");
	keyBinding[consoleAction].text  = gui_getString ("consoleAction");
	keyBinding[gameScreenShot].text = gui_getString ("gameScreenShot");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the default values for keybindings
void io_setDefaultKeybindings ()
//----------------------------------------------------------------------------------------------------------------------
{
	keyBinding[gameLeft].keyValue       = ALLEGRO_KEY_LEFT;
	keyBinding[gameRight].keyValue      = ALLEGRO_KEY_RIGHT;
	keyBinding[gameDown].keyValue       = ALLEGRO_KEY_DOWN;
	keyBinding[gameUp].keyValue         = ALLEGRO_KEY_UP;
	keyBinding[gamePause].keyValue      = ALLEGRO_KEY_P;
	keyBinding[gameAction].keyValue     = ALLEGRO_KEY_LCTRL;
	keyBinding[gameEscape].keyValue     = ALLEGRO_KEY_ESCAPE;
	keyBinding[consoleAction].keyValue  = ALLEGRO_KEY_BACKQUOTE;
	keyBinding[gameScreenShot].keyValue = ALLEGRO_KEY_S;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process keyboard events
void io_processKeyActions ()
//----------------------------------------------------------------------------------------------------------------------
{
	switch (currentMode)
		{
			case MODE_GUI:
			case MODE_GUI_OPTIONS:
			case MODE_GUI_OPTIONS_VIDEO:
			case MODE_GUI_TUT_MOVE:
			case MODE_GUI_TUT_TRANSFER_GAME:
			case MODE_GUI_TUT_TRANSFER_START:
			case MODE_GUI_TUT_LIFTS:
			case MODE_GUI_TUT_TERMINALS:
			case MODE_GUI_TUT_HEALING:
			case MODE_GUI_TUT_TIPS:
			case MODE_GUI_TERMINAL:
			case MODE_GUI_TERMINAL_SHIPVIEW:
			case MODE_GUI_TERMINAL_DECKVIEW:
			case MODE_GUI_DATABASE:
			case MODE_GUI_TRANSFER_SCREEN_ONE:
			case MODE_GUI_TRANSFER_SCREEN_TWO:
				if (keyBinding[gameUp].currentlyPressed)
					{
						keyBinding[gameUp].currentlyPressed = false;
						gui_handleFocusMove (GUI_MOVE_UP, false, false);
					}

			if (keyBinding[gameDown].currentlyPressed)
				{
					keyBinding[gameDown].currentlyPressed = false;
					gui_handleFocusMove (GUI_MOVE_DOWN, false, false);
				}

			if (keyBinding[gameLeft].currentlyPressed)
				{
					keyBinding[gameLeft].currentlyPressed = false;
					gui_handleFocusMove (GUI_MOVE_LEFT, false, false);
				}

			if (keyBinding[gameRight].currentlyPressed)
				{
					keyBinding[gameRight].currentlyPressed = false;
					gui_handleFocusMove (GUI_MOVE_RIGHT, false, false);
				}

			if (keyBinding[gameAction].currentlyPressed)
				{
					keyBinding[gameAction].currentlyPressed = false;
					gui_handleFocusMove (GUI_ACTION, true, false);
				}
			break;

			case MODE_GUI_TRANSFER_CHOOSE_SIDE:
				trn_handleTransferChooseSide();
				break;

			case MODE_GAME:
				gam_processPlayerMovement ();
			if (keyBinding[gameAction].currentlyPressed)
				{
					gam_processActionKey ();
				}
			else
				{
					if (playerDroid.inTransferMode)
						{
							playerDroid.inTransferMode = false;
							evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_STOP_AUDIO, 20, ALLEGRO_PLAYMODE_LOOP, "transferMove");
							hud_setText ("hudMoving");
						}
				}
			break;

			case MODE_LIFT_VIEW:
				if (keyBinding[gameUp].currentlyPressed)
					{
						gam_moveLift (1);
						keyBinding[gameUp].currentlyPressed = false;
					}

			if (keyBinding[gameDown].currentlyPressed)
				{
					gam_moveLift (2);
					keyBinding[gameDown].currentlyPressed = false;
				}

			if (keyBinding[gameAction].currentlyPressed)
				{
					keyBinding[gameAction].currentlyPressed = false;
					lvl_changeToLevel (lvl_returnLevelNameFromDeck (currentDeckNumber), gam_putPlayerOnLiftFromTunnel (currentDeckNumber));

					sys_changeMode (MODE_GAME, true);
					hud_setText ("hudMoving");
				}

			break;
		}
}