#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/system/sys_init.h>
#include <hdr/system/sys_gameEvents.h>
#include <hdr/game/gam_pathFind.h>
#include "hdr/system/sys_timerFunctions.h"

int splashTimeout;
bool locationIconVisible = true;

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

	aStarPathTimer = al_create_timer (aStarPathInterval);
	if (nullptr == aStarPathTimer)
		{
			quitProgram = true;
			al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Timers. Exiting", "Could not init fade timer.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

	splashTimer = al_create_timer (5);
	if (nullptr == splashTimer)
		{
			quitProgram = true;
			al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Timers. Exiting", "Could not start timer for splash countdown.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}
	al_register_event_source (eventQueue, al_get_timer_event_source (splashTimer));

	deckviewLocationTimer = al_create_timer(0.5);
	if (nullptr == deckviewLocationTimer)
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Timers. Exiting", "Could not start timer for splash countdown.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}
	al_register_event_source (eventQueue, al_get_timer_event_source (deckviewLocationTimer));
	al_start_timer (deckviewLocationTimer);

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

//----------------------------------------------------------------------------------------------------------------------
//
// Function that is called when the splash screen timeout is reached
void tim_changeToGUI(ALLEGRO_TIMER_EVENT *timer_event)
//----------------------------------------------------------------------------------------------------------------------
{
	al_stop_timer(splashTimer);
	al_destroy_timer(splashTimer);
	sys_changeMode (MODE_GUI, true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Function to animate the deckview location icon
void tim_animateLocationIcon(ALLEGRO_TIMER_EVENT *time_event)
//----------------------------------------------------------------------------------------------------------------------
{
	locationIconVisible = !locationIconVisible;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the value of the Icon visible flag
bool tim_getIconState()
//----------------------------------------------------------------------------------------------------------------------
{
	return locationIconVisible;
}