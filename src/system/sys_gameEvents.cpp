#include "hdr/system/sys_gameEvents.h"

ALLEGRO_EVENT_QUEUE *eventQueue;

//----------------------------------------------------------------------------------------------------------------------
//
// Called once a second to reset timing counters
static void tickTimers(ALLEGRO_TIMER_EVENT *timer_event)
//----------------------------------------------------------------------------------------------------------------------
{
	printFPS = fps;
	printThinkFPS = thinkFPS;
	fps      = 0;
	thinkFPS = 0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init the event queues
void evt_initEvents()
//----------------------------------------------------------------------------------------------------------------------
{
	eventQueue = al_create_event_queue();
	if (nullptr == eventQueue)
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start the event queue.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process input and game events from queue
void evt_handleEvents()
//----------------------------------------------------------------------------------------------------------------------
{
	ALLEGRO_EVENT event;

	al_get_next_event(eventQueue, &event);

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

			/* Is it time for the next timer tick? */
		case ALLEGRO_EVENT_TIMER:
			tickTimers(&event.timer);
			break;
	}
}
