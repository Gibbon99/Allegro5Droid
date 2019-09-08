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
#include "hdr/system/sys_gameFrameUpdate.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun (double tickTime)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (currentMode)
	{
		case MODE_CONSOLE:
			break;

		case MODE_GAME:
			io_animateEnemySprites (lvl_getCurrentLevelName (), tickTime);
			gam_animateHealing (lvl_getCurrentLevelName (), tickTime);
			io_processKeyActions();
			gam_processHealingTiles();
			gam_AStarProcessPaths (lvl_getCurrentLevelName());

			sys_updateVisibleScreenArea();
			ai_processDroidAI(lvl_getCurrentLevelName());

			sys_processPhysics (tickTime);

			break;

		case MODE_LIFT_VIEW:
			io_processKeyActions();
			break;

		default:
			break;
	}
}
