#include <cmath>
#include <hdr/game/gam_render.h>
#include <hdr/game/gam_droidAI.h>
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
int  renderBackdrop     = 1;
int  windowWidth        = 0;
int  windowHeight       = 0;
int  screenWidth        = 0;
int  screenHeight       = 0;
int  screenType         = 0;
int  displayRefreshRate = 0;
int  currentMode        = 0;

double tickTime = 1.0f / TICKS_PER_SECOND;

std::vector<double> deltaArray;

//----------------------------------------------------------------------------------------------------------------------
//
// Return logical screen width
float sys_getLogicalWidth ()
//----------------------------------------------------------------------------------------------------------------------
{
	return screenWidth;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the logical screen height
float sys_getLogicalHeight ()
//----------------------------------------------------------------------------------------------------------------------
{
	return screenHeight;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return current tickTime
double sys_getTickTime ()
//----------------------------------------------------------------------------------------------------------------------
{
	return tickTime;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Smooth delta two
double sys_smoothDelta2 (double delta)
//----------------------------------------------------------------------------------------------------------------------
{
	static int numFramesToSmooth = 100;
	double     totalDelta;

	if (deltaArray.size () < numFramesToSmooth)
		{
			deltaArray.push_back (delta);
		}
	else
		{
			for (int i                         = 0; i != deltaArray.size () - 1; i++)
				{
					deltaArray[i] = deltaArray[i + 1];
				}
			deltaArray[deltaArray.size () - 1] = delta;
		}

	for (auto index : deltaArray)
		{
			totalDelta += index;
		}
	return totalDelta / deltaArray.size ();

}

//----------------------------------------------------------------------------------------------------------------------
//
// Run main loop
//
// Fixed update frames
// Render as fast as possible
int main (int argc, char *argv[])
//----------------------------------------------------------------------------------------------------------------------
{
	double currentTime    = 0;
	double accumulator    = 0;
	double newTime        = 0;
	double frameTime      = 0;
	double percentInFrame = 0;

	sys_initAll ();

	currentTime = PARA_getTime ();

	while (!quitProgram)
		{
			newTime   = PARA_getTime ();
			frameTime = newTime - currentTime;

			if (frameTime > 0.25)
				frameTime = 0.25;

			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= tickTime)
				{
					accumulator -= tickTime;
					sys_gameTickRun (tickTime);
					thinkFPS++;
				}
			percentInFrame = accumulator / tickTime;

			evt_handleEvents ();
//		percentInFrame = cpflerp(0.0, 1.0f, percentInFrame);

//		double smoothedDelta = sys_smoothDelta2(percentInFrame);

//		smoothedDelta = cpfclamp(smoothedDelta, 0.0f, 1.0f);

			double smoothedDelta = percentInFrame;

			sys_displayScreen (smoothedDelta);
			fps++;

			sys_pushFrameTimeIntoQueue (accumulator, 500.00f);

			frameTimePrint = (PARA_getTime () - newTime) * 1000.0f;

			if (accumulator < 0.0f)
				accumulator = 0.0f;
		}
	sys_shutdownToSystem ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to a new mode
void sys_changeMode (int newMode, bool fade)
//----------------------------------------------------------------------------------------------------------------------
{
	currentMode = newMode;

	if (fade)
		{
			previousScreen = al_clone_bitmap (backingBitmap);

			fadeAlphaValue = 255;
			fadeInProgress = FADE_ON;
			if (nullptr != fadeTimer)
				al_start_timer (fadeTimer);
		}
	else
		{
			fadeInProgress = FADE_NONE;
		}
}