#include <hdr/system/sys_gameFrameUpdate.h>
#include <cmath>
#include <queue>
#include <list>
#include <hdr/system/sys_init.h>
#include <hdr/io/io_console.h>
#include <hdr/io/io_resources.h>
#include <hdr/game/gam_hud.h>
#include <hdr/game/gam_render.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_doors.h>
#include "hdr/system/sys_gameFrameRender.h"

ALLEGRO_BITMAP *backingBitmap;
ALLEGRO_BITMAP *previousScreen;

float frameTimeArray[800];
float scaleW         = 0;
float scaleH         = 0;
float scaleX         = 0;
float scaleY         = 0;
int   scale          = 0;
float fadeAlphaValue = 0;
int   fadeInProgress = FADE_NONE;

double renderCirclePosX, renderCirclePosY;

//----------------------------------------------------------------------------------------------------------------------
//
// Process if we need to do a screen fade or not
void sys_screenFade ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (fadeInProgress == FADE_NONE)
		{
			PARA_presentFrame (display, backingBitmap);
			return;
		}

	if (fadeInProgress == FADE_ON)
		{
			al_draw_filled_rectangle (0, 0, screenWidth, screenHeight, al_map_rgba (0, 0, 0, fadeAlphaValue));

			PARA_presentFrame (display, backingBitmap);
		}
	else if (fadeInProgress == FADE_OFF)
		{
			al_draw_filled_rectangle (0, 0, screenWidth, screenHeight, al_map_rgba (0, 0, 0, fadeAlphaValue));

			PARA_presentFrame (display, previousScreen);
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init render variables
bool sys_initRenderVariables ()
//----------------------------------------------------------------------------------------------------------------------
{
	int sx, sy;

	backingBitmap = al_create_bitmap (screenWidth, screenHeight);
	if (nullptr == backingBitmap)
		{
			quitProgram = true;
			al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create backing bitmap.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

	sx    = windowWidth / screenWidth;
	sy    = windowHeight / screenHeight;
	scale = std::min (sx, sy);

	// calculate how much the buffer should be scaled
	scaleW = (float) screenWidth * scale;
	scaleH = (float) screenHeight * scale;
	scaleX = ((float) windowWidth - scaleW) / 2;
	scaleY = ((float) windowHeight - scaleH) / 2;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Push a value onto the frametime Queue
void sys_pushFrameTimeIntoQueue (double thisFrameTime, float factor)
//----------------------------------------------------------------------------------------------------------------------
{
	float renderFrameTime = 0;

	renderFrameTime = thisFrameTime * factor;

	for (int i                      = 0; i != screenWidth - 1; i++)
		{
			frameTimeArray[i] = frameTimeArray[i + 1];
		}
	frameTimeArray[screenWidth - 1] = renderFrameTime;

//	printf("%f\n", thisFrameTime);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the frametime queue
void sys_renderFrameTimeQueue ()
//----------------------------------------------------------------------------------------------------------------------
{
	float startX = screenWidth;
	float startY = screenHeight;

	for (int i = 0; i != screenWidth - 1; i++)
		{
			al_draw_line (startX, startY, startX, startY - frameTimeArray[i], al_map_rgba_f (0, 0, 0, 0.4), 1);
			startX--;
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_displayScreen (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_prepareFrame (backingBitmap);

	switch (currentMode)
		{
			case MODE_CONSOLE:
				con_renderConsole ();
			break;

			case MODE_GAME:

//				gam_drawAllTiles();
				gam_renderDoorFrames();
			gam_drawVisibleScreen (interpolation);
//			lvl_showWayPoints (lvl_getCurrentLevelName());
//			sys_debugPhysicsWalls(lvl_getCurrentLevelName());

			gam_renderDroids (lvl_getCurrentLevelName(), interpolation);

//			io_renderSpriteFrame("123", io_getFrame(), screenWidth / 2, screenHeight / 2);
			io_renderTintedSpriteFrame ("123", io_getFrame (), screenWidth / 2, screenHeight / 2, 0, 0, 0);
			hud_renderHUD ();

			sys_getPhysicsWorld()->DrawDebugData();

			break;

			default:
				break;
		}

	sys_renderFrameTimeQueue ();

	//
	// Draw everything here
	fnt_printSystemFont (5, 0, sys_getString ("Rate [ %i ] FPS [ %f ] ", displayRefreshRate, printFPS));
	fnt_printSystemFont (5, 10, sys_getString ("worldPos [ %f %f ] ", playerDroid.worldPos.x, playerDroid.worldPos.y));
	fnt_printSystemFont (5, 20, sys_getString ("inter [ %f ]", interpolation));
	fnt_printSystemFont (5, 30, sys_getString ("Pos [ %f %f ]", playerDroid.worldPos.x, playerDroid.worldPos.y));

	sys_screenFade ();
}