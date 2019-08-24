#include <hdr/system/sys_init.h>
#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/system/sys_timerFunctions.h>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_keyboard.h>
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

			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				keyBinding[gameLeft].currentlyPressed = true;

			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				keyBinding[gameRight].currentlyPressed = true;

			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				keyBinding[gameDown].currentlyPressed = true;

			if (event.keyboard.keycode == ALLEGRO_KEY_UP)
				keyBinding[gameUp].currentlyPressed = true;

			break;

			case ALLEGRO_EVENT_KEY_UP:
				if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
					keyBinding[gameLeft].currentlyPressed = false;

			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				keyBinding[gameRight].currentlyPressed = false;

			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				keyBinding[gameDown].currentlyPressed = false;

			if (event.keyboard.keycode == ALLEGRO_KEY_UP)
				keyBinding[gameUp].currentlyPressed = false;

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

	gam_processPlayerMovement();
	cpBodySetForce(playerPhysicsObjectClient.body, playerVelocity);
}
