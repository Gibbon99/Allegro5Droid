#include <hdr/system/sys_audio.h>
#include <hdr/game/gam_hud.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/game/gam_game.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_pauseMode.h>

bool isPaused = false;

//---------------------------------------------------------------------------------------------------------------------------
//
// Handle entering or leaving pause mode
void gam_handlePauseMode (bool enterPauseMode)
//---------------------------------------------------------------------------------------------------------------------------
{
	if (enterPauseMode)
	{
		keyBinding[gamePause].currentlyPressed = false;
		isPaused = true;
		sys_changeMode (MODE_GAME_PAUSED, false);
		hud_setText (false, "hudPaused");
		snd_stopAllPlaying ();
		return;
	}
	//
	// Exiting Pause Mode
	keyBinding[gamePause].currentlyPressed = false;
	isPaused = false;
	sys_changeMode (MODE_GAME, false);
	hud_setText (false, "hudMoving");
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_LOOP, currentAlertLevelSoundName);
}