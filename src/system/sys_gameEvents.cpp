#include <hdr/system/sys_init.h>
#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/system/sys_timerFunctions.h>
#include "hdr/system/sys_gameEvents.h"

ALLEGRO_EVENT_QUEUE *eventQueue;

//----------------------------------------------------------------------------------------------------------------------
//
// Init the event queues
void evt_initEvents ()
//----------------------------------------------------------------------------------------------------------------------
{
	eventQueue = al_create_event_queue ();
	if (nullptr == eventQueue)
		{
			quitProgram = true;
			al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start the event queue.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process input and game events from queue
void evt_handleEvents ()
//----------------------------------------------------------------------------------------------------------------------
{
	ALLEGRO_EVENT event;

	al_get_next_event (eventQueue, &event);

	switch (event.type)
		{
			/* Was the X button on the window pressed? */
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				quitProgram = true;
			return;

			/* Was a key pressed? */
			case ALLEGRO_EVENT_KEY_DOWN:
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					{
						quitProgram = true;
					}
			break;

			//
			// Process timer functions
			case ALLEGRO_EVENT_TIMER:
				if (timingTimer == event.timer.source)
					{
						tim_tickTimers (&event.timer);
					}

				if (fadeTimer == event.timer.source)
					{
						tim_runFadeProcess (&event.timer);
					}
			break;
		}
}
