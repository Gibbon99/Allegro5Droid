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
			playerDroid.body->ApplyForce(playerDroid.velocity, playerDroid.body->GetWorldCenter(), true);
			sys_updateVisibleScreenArea();
			ai_processDroidAI(lvl_getCurrentLevelName());

			sys_stepPhysicsWorld (1.0f / TICKS_PER_SECOND);

			playerDroid.body->SetLinearVelocity({0,0});
			break;

		default:
			break;
	}
}
