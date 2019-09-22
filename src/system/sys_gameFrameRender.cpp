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
#include <hdr/gui/gui_sideView.h>
#include <hdr/game/gam_bullet.h>
#include <hdr/game/gam_healing.h>
#include <hdr/game/gam_pathFind.h>
#include <hdr/gui/gui_scrollBox.h>
#include <hdr/gui/gui_render.h>
#include <hdr/io/io_resourceImage.h>
#include <src/game/gam_terminal.h>
#include <hdr/gui/gui_database.h>
#include <hdr/gui/gui_deckView.h>
#include "hdr/system/sys_gameFrameRender.h"

ALLEGRO_BITMAP *backingBitmap;
ALLEGRO_BITMAP *previousScreen;

bool  showDebugPhysics;
float frameTimeArray[800];
float scaleW         = 0;
float scaleH         = 0;
float scaleX         = 0;
float scaleY         = 0;
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
	int   sx, sy;
	float renderDrawScale;

	backingBitmap = al_create_bitmap (screenWidth, screenHeight);
	if (nullptr == backingBitmap)
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create backing bitmap.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	sx              = windowWidth / screenWidth;
	sy              = windowHeight / screenHeight;
	renderDrawScale = std::min (sx, sy);

	// calculate how much the buffer should be scaled
	scaleW = (float) screenWidth * renderDrawScale;
	scaleH = (float) screenHeight * renderDrawScale;
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
		case MODE_SPLASH:
			sys_drawBitmap ("splash", 0.0f, 0.0f, RENDER_FULLSCREEN);
			break;

		case MODE_CONSOLE:
			con_renderConsole ();
			break;

		case MODE_GUI_INTRO:
			hud_renderHUD ();
			gui_renderScrollBox(&introScrollBox, interpolation);
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
			hud_renderHUD ();
			gui_drawGUI ();
			break;

		case MODE_GUI_TERMINAL_DECKVIEW:
			gam_showCurrentLevel(deckViewRatio);
			hud_renderHUD ();
			gui_drawGUI ();
			break;

		case MODE_GUI_TERMINAL_SHIPVIEW:
			gui_drawSideView ();
			hud_renderHUD();
			gui_drawGUI();
			break;

		case MODE_GUI_DATABASE:
			gui_renderDatabaseScreen();
			gui_renderScrollBox(&databaseScrollBox, interpolation);
			hud_renderHUD ();
			gui_drawGUI ();
			break;

		case MODE_GAME:

			gam_renderDoorFrames ();
			gam_renderHealingFrames (lvl_getCurrentLevelName ());
			gam_drawVisibleScreen (interpolation);

			gam_renderDroids (lvl_getCurrentLevelName (), interpolation);

			io_renderSpriteFrame ("001", playerDroid.currentFrame, screenWidth / 2, screenHeight / 2);
			bul_renderBullets ();
//			io_renderTintedSpriteFrame ("123", io_getFrame (), screenWidth / 2, screenHeight / 2, 0, 0, 0);
			hud_renderHUD ();

			gam_AStarDebugWayPoints (0);
//			lvl_showWayPoints (lvl_getCurrentLevelName());
			if (showDebugPhysics)
				sys_getPhysicsWorld()->DrawDebugData();

			break;

		case MODE_LIFT_VIEW:
			al_set_target_bitmap (backingBitmap);
			gui_drawSideView ();
			hud_renderHUD ();
			break;

		default:
			break;
	}

	sys_renderFrameTimeQueue ();

	//
	// Draw everything here
	fnt_setColor_f (0, 0, 0, 1);
	fnt_printSystemFont (5, 0, sys_getString ("Rate [ %i ] FPS [ %f ] ", displayRefreshRate, printFPS));
	fnt_printSystemFont (5, 20, sys_getString ("inter [ %f ]", interpolation));

	sys_screenFade ();
}