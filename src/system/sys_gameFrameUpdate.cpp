#include <hdr/system/sys_physics.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_droidAI.h>
#include <hdr/game/gam_render.h>
#include "hdr/system/sys_gameFrameUpdate.h"

float prevCirclePosX, prevCirclePosY;
float circlePosX = 50.0f;
float circlePosY = 100.0f;
float velocity   = -10.0f;

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

			drd_processDroidAI ( currentLevelName );

			//	cpSpaceStep (space, SKIP_TICKS);
			cpSpaceStep (space, 1000.0f / TICKS_PER_SECOND);
			break;

		default:
			break;
	}
}
