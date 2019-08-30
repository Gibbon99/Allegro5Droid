#include <hdr/system/sys_physics.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_droidAI.h>
#include <hdr/game/gam_render.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_player.h>
#include "hdr/system/sys_gameFrameUpdate.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun()
//----------------------------------------------------------------------------------------------------------------------
{
	switch (currentMode)
	{
		case MODE_CONSOLE:
			break;

		case MODE_GAME:
			io_testAnimateSprite();
			gam_processPlayerMovement ();
			cpBodySetForce (playerDroid.body, playerDroid.velocity);
			sys_updateVisibleScreenArea();
			drd_processDroidAI ( lvl_getCurrentLevelName() );

			//	cpSpaceStep (space, SKIP_TICKS);
			cpSpaceStep (space, 1000.0f / TICKS_PER_SECOND);
			break;

		default:
			break;
	}
}
