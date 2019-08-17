#include <hdr/system/sys_gameFrameUpdate.h>
#include <cmath>
#include "hdr/system/sys_gameFrameRender.h"

double renderCirclePosX, renderCirclePosY;

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_displayScreen(double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	al_clear_to_color(al_map_rgb_f(1, 1, 1));

	fnt_printSystemFont(5, 50, sys_getString("FPS [ %i ] thinkFPS [ %i ] frameTime [ %f ] inter [ %f ]", printFPS, printThinkFPS, frameTimePrint, interpolation));
	fnt_printSystemFont(5, 60, sys_getString("render X Y [ %f %f ]", renderCirclePosX, renderCirclePosY));

	renderCirclePosX = circlePosX + (velocity * interpolation);
	renderCirclePosY = circlePosY; // + (velocity * interpolation);

	al_draw_filled_circle(renderCirclePosX, renderCirclePosY, 32, al_map_rgb_f(0, 0, 0));

	al_flip_display();
}