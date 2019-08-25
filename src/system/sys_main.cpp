#include <cmath>
#include "system/sys_main.h"
#include "system/sys_init.h"
#include "system/sys_gameFrameUpdate.h"
#include "system/sys_gameFrameRender.h"
#include "system/sys_shutdown.h"
#include "system/sys_gameEvents.h"

double fps            = 0;
double printFPS       = 0;
double thinkFPS       = 0;
double printThinkFPS  = 0;
double frameTimePrint = 0;

bool quitProgram        = false;
int  windowWidth        = 0;
int  windowHeight       = 0;
int  screenWidth        = 0;
int  screenHeight       = 0;
int  screenType         = 0;
int  displayRefreshRate = 0;
int  currentMode        = 0;

std::vector<double>    deltaArray;
//----------------------------------------------------------------------------------------------------------------------
//
// Smooth delta two
double sys_smoothDelta2(double delta)
//----------------------------------------------------------------------------------------------------------------------
{
	static int numFramesToSmooth = 100;
	double totalDelta;

	if (deltaArray.size() < numFramesToSmooth)
		{
			deltaArray.push_back (delta);
		}
	else
		{
			for (int i = 0; i != deltaArray.size () - 1; i++)
				{
					deltaArray[i] = deltaArray[i + 1];
				}
			deltaArray[deltaArray.size() - 1] = delta;
		}

	for (auto index : deltaArray)
		{
			totalDelta += index;
		}
	return totalDelta / deltaArray.size();

}

//----------------------------------------------------------------------------------------------------------------------
//
// Smooth out the delta ( frame time ) for rendering
void sys_smoothDelta(double &deltaPtr, double refreshRate)
//----------------------------------------------------------------------------------------------------------------------
{
	static double smoothDeltaBuffer = 0;
	static int    frameCount        = 0;
	static double oldDelta          = 0;

	// add in whatever time we currently have saved in the buffer
	deltaPtr += smoothDeltaBuffer;

	// calculate how many frames will have passed on the next vsync
	frameCount = (int) (deltaPtr * refreshRate + 1);

	// save off the delta, we will need it later to update the buffer
	oldDelta = deltaPtr;

	// recalculate delta to be an even frame rate multiple
	deltaPtr = frameCount / refreshRate;

	// update delta buffer so we keep the same time on average
	smoothDeltaBuffer = oldDelta - deltaPtr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run main loop
//
// Fixed update frames
// Render as fast as possible
int main(int argc, char *argv[])
//----------------------------------------------------------------------------------------------------------------------
{
	double tickTime       = 1.0f / TICKS_PER_SECOND;
	double currentTime    = 0;
	double accumulator    = 0;
	double newTime        = 0;
	double frameTime      = 0;
	double percentInFrame = 0;

	sys_initAll();

	currentTime = PARA_getTime();

	while (!quitProgram)
	{
		newTime   = PARA_getTime();
		frameTime = newTime - currentTime;

		if (frameTime > 0.25)
		{
			frameTime = 0.25;
		}

		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= tickTime)
		{
			accumulator -= tickTime;
			sys_gameTickRun();
			thinkFPS++;
		}

		evt_handleEvents();

		percentInFrame = accumulator / tickTime;

//		sys_smoothDelta(percentInFrame, displayRefreshRate);

		double smoothedDelta = sys_smoothDelta2(percentInFrame);

		sys_displayScreen(smoothedDelta);
		fps++;

		sys_pushFrameTimeIntoQueue(smoothedDelta, 10.00f);

		frameTimePrint = (PARA_getTime() - newTime) * 1000.0f;
//		sys_pushFrameTimeIntoQueue(percentInFrame, 100.0f);

		if (accumulator < 0.0f)
			accumulator = 0.0f;
	}

	sys_shutdownToSystem();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to a new mode
void sys_changeMode(int newMode, bool fade)
//----------------------------------------------------------------------------------------------------------------------
{
	currentMode = newMode;
	printf("Changing mode\n");

	if (fade)
		{
			previousScreen = al_clone_bitmap(backingBitmap);

			fadeAlphaValue = 255;
			fadeInProgress = FADE_ON;
			if (nullptr != fadeTimer)
				al_start_timer(fadeTimer);
		}
	else
		{
			fadeInProgress = FADE_NONE;
		}
}