#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/system/sys_init.h>
#include <hdr/system/sys_gameEvents.h>
#include "hdr/system/sys_timerFunctions.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the timers and add to event source
bool tim_initAllTimers ()
//----------------------------------------------------------------------------------------------------------------------
{
//
// Create timer to track FPS, thinkFPS and frameTime
	timingTimer = al_create_timer (1.0f);
	if (nullptr == timingTimer)
		{
			quitProgram = true;
			al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Timers. Exiting", "Could not start the initial timer.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}
	al_register_event_source (eventQueue, al_get_timer_event_source (timingTimer));
	al_start_timer (timingTimer);
//
// Create a timer to process fade routine
	fadeTimer = al_create_timer (0.02f);
	if (nullptr == fadeTimer)
		{
			quitProgram = true;
			al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Timers. Exiting", "Could not init fade timer.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}
	al_register_event_source (eventQueue, al_get_timer_event_source (fadeTimer));

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called once a second to reset timing counters
void tim_tickTimers (ALLEGRO_TIMER_EVENT *timer_event)
//----------------------------------------------------------------------------------------------------------------------
{
	printFPS      = fps;
	printThinkFPS = thinkFPS;
	fps           = 0;
	thinkFPS      = 0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// The fade is run from a timer thread
void tim_runFadeProcess (ALLEGRO_TIMER_EVENT *timer_event)
//----------------------------------------------------------------------------------------------------------------------
{
//	printf("Fade value [ %i ]\n", fadeAlphaValue);

	switch (fadeInProgress)
		{
			case FADE_OFF:
				if (fadeAlphaValue > 200)
					fadeAlphaValue += 4;
				else
					fadeAlphaValue += 2;
			if (fadeAlphaValue > 254)
				{
					fadeAlphaValue = 254;
					fadeInProgress = FADE_OFF;
				}
			break;

			case FADE_ON:
				fadeAlphaValue -= 2;
			if (fadeAlphaValue < 0)
				{
					fadeAlphaValue = 0;
					fadeInProgress = FADE_NONE;
					al_stop_timer (fadeTimer);
				}

			default:
				break;
		}
}
