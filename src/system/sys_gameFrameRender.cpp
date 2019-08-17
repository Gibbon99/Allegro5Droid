#include <hdr/system/sys_gameFrameUpdate.h>
#include <cmath>
#include <queue>
#include <list>
#include "hdr/system/sys_gameFrameRender.h"

float frameTimeArray[800];

double renderCirclePosX, renderCirclePosY;

//----------------------------------------------------------------------------------------------------------------------
//
// Push a value onto the frametime Queue
void sys_pushFrameTimeIntoQueue(double thisFrameTime, float factor)
//----------------------------------------------------------------------------------------------------------------------
{
	float renderFrameTime = 0;

	renderFrameTime = thisFrameTime * factor;

	for (int i = 0; i != 800; i++)
	{
		frameTimeArray[i] = frameTimeArray[i + 1];
	}
	frameTimeArray[799] = renderFrameTime;

//	printf("%f\n", thisFrameTime);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the frametime queue
void sys_renderFrameTimeQueue()
//----------------------------------------------------------------------------------------------------------------------
{
	float startX = windowWidth;
	float startY = windowHeight;

	for (int i = 0; i != 800; i++)
	{
		al_draw_line(startX, startY, startX, startY - frameTimeArray[i], al_map_rgb_f(0, 0, 0), 1);
		startX--;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_displayScreen(double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	al_clear_to_color(al_map_rgb_f(1, 1, 1));

	fnt_printSystemFont(5, 50, sys_getString("Rate [ %i ] FPS [ %f ] thinkFPS [ %f ] frameTime [ %f ] inter [ %f ]", displayRefreshRate, printFPS, printThinkFPS, frameTimePrint, interpolation));
	fnt_printSystemFont(5, 60, sys_getString("render X Y [ %f %f ]", renderCirclePosX, renderCirclePosY));

	renderCirclePosX = prevCirclePosX + ((circlePosX - prevCirclePosX) * interpolation);
	renderCirclePosY = circlePosY; // + (velocity * interpolation);

	al_draw_filled_rectangle(renderCirclePosX, renderCirclePosY, renderCirclePosX + 128, renderCirclePosY + 128, al_map_rgb_f(0, 0, 0));

	sys_renderFrameTimeQueue();

	al_flip_display();
}