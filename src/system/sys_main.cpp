#include <cmath>
#include "system/sys_main.h"
#include "system/sys_init.h"
#include "system/sys_gameFrameUpdate.h"
#include "system/sys_gameFrameRender.h"
#include "system/sys_shutdown.h"
#include "system/sys_gameEvents.h"

unsigned int fps           = 0;
unsigned int printFPS      = 0;
unsigned int printThinkFPS = 0;
unsigned int thinkFPS      = 0;
unsigned int loops         = 0;

double frameTime          = 0;
double frameTimePrint     = 0;
double percentWithinFrame = 0;
double time0              = 0;
double time1              = 0;

bool quitProgram  = false;
int  windowWidth  = 0;
int  windowHeight = 0;

//----------------------------------------------------------------------------------------------------------------------
//
// Run main loop
//
// Fixed update frames
// Render as fast as possible
int main(int argc, char *argv[])
//----------------------------------------------------------------------------------------------------------------------
{
	double startTime = 0;
	double tickTime  = 0;

	tickTime = (1000.0f / TICKS_PER_SECOND) * 0.001f;

	sys_initAll();

	startTime = al_get_time();
	time0     = startTime;

	while (!quitProgram)
	{
		time1     = al_get_time();
		frameTime = time1;

		loops = 0;
		while ((time1 - time0) > tickTime && (loops < MAX_FRAMESKIP))
		{
			time0 += tickTime;
			loops++;
			sys_gameTickRun();
			time1 = al_get_time();
			thinkFPS++;
		}

		evt_handleEvents();

		percentWithinFrame = fmin(1.0f, float(time1 - time0) / tickTime);
		if (percentWithinFrame >= 1.0f)
		{
			percentWithinFrame = 1.0f;
			sys_displayScreen(percentWithinFrame);
		}
		else
			sys_displayScreen(percentWithinFrame);

		fps++;
		frameTimePrint = al_get_time() - frameTime;
	}

	sys_shutdownToSystem();

	return 0;
}
