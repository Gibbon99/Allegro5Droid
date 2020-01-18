#include <hdr/io/io_keyboard.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_transferDroidAI.h>
#include <hdr/game/gam_transferRender.h>
#include <hdr/system/sys_audio.h>
#include "hdr/game/gam_transferPlayer.h"

// ----------------------------------------------------------------------------
//
// Move the player token based on passed on direction
void trn_movePlayerToken (int direction)
// ----------------------------------------------------------------------------
{
	printf ("Move player token\n");

	if (MOVE_UP == direction)
		{
			playerBlockPos--;
			if (playerBlockPos < 0)
				playerBlockPos = transferRows.size () - 1;  // wrap around to bottom
		}

	if (MOVE_DOWN == direction)
		{
			playerBlockPos++;
			if (playerBlockPos > transferRows.size () - 1)
				playerBlockPos = 0;          // wrap back to top
		}
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_ONCE, "keyPressGood");
}

//---------------------------------------------------------------------------------------------------------------------
//
// Do transfer movements for player
void trn_processPlayerActions()
//---------------------------------------------------------------------------------------------------------------------
{
	if (keyBinding[gameUp].currentlyPressed)
		{
			trn_movePlayerToken (MOVE_UP);
			keyBinding[gameUp].currentlyPressed = false;
			return;
		}

	if (keyBinding[gameDown].currentlyPressed)
		{
			trn_movePlayerToken (MOVE_DOWN);
			keyBinding[gameDown].currentlyPressed = false;
			return;
		}

	if (keyBinding[gameAction].currentlyPressed)
		{
			keyBinding[gameAction].currentlyPressed = false;
			return;
		}
}
