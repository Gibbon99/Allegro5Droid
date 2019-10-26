#include <hdr/system/sys_physics.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_droidAI.h>
#include <hdr/game/gam_render.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/game/gam_physicActions.h>
#include <hdr/game/gam_healing.h>
#include <hdr/game/gam_pathFind.h>
#include <hdr/gui/gui_scrollBox.h>
#include <hdr/game/gam_lineOfSight.h>
#include <hdr/gui/gui_database.h>
#include <hdr/game/gam_particles.h>
#include <hdr/game/gam_game.h>
#include <hdr/system/sys_audio.h>
#include "hdr/system/sys_gameFrameUpdate.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun (double tickTime)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string cachedCurrentLevelName;

	cachedCurrentLevelName = lvl_getCurrentLevelName ();

	switch (currentMode)
	{
		case MODE_CONSOLE:
			break;

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
		case MODE_GUI_TRANSFER_SCREEN_ONE:
		case MODE_GUI_TRANSFER_SCREEN_TWO:
		case MODE_GUI_TRANSFER_CHOOSE_SIDE:
			io_processKeyActions();
			break;

		case MODE_GUI_DATABASE:
			gui_animateDroidSprite(tickTime);
			gui_scrollScrollBox (&databaseScrollBox, tickTime);
			io_processKeyActions ();
			break;

		case MODE_GUI_INTRO:
			gui_scrollScrollBox (&introScrollBox, tickTime);
			break;

		case MODE_GAME:
			io_animateEnemySprites (cachedCurrentLevelName, tickTime);
			gam_animateHealing (cachedCurrentLevelName, tickTime);
			io_processKeyActions();
			gam_processHealingTiles();
			gam_AStarProcessPaths (cachedCurrentLevelName);
			gam_weaponRechargePlayer(tickTime);

			gam_processScore (tickTime);
			sys_updateVisibleScreenArea();
			ai_processDroidAI (cachedCurrentLevelName, tickTime);
			gam_doLineOfSight ();
			par_processEmitterQueue ();
			par_animateParticles (tickTime);
			sys_processPhysics (tickTime);

//			if (false == snd_isPlaying ("greenAlert"))
//				printf("Sound [ %s ] is NOT playing.\n", "greenAlert");

			break;

		case MODE_LIFT_VIEW:
			io_processKeyActions();
			break;

		default:
			break;
	}
}
