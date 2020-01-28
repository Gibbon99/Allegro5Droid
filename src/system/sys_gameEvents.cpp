#include <hdr/system/sys_init.h>
#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/system/sys_timerFunctions.h>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_console.h>
#include <hdr/game/gam_doors.h>
#include <hdr/game/gam_bullet.h>
#include <hdr/game/gam_healing.h>
#include <hdr/system/sys_audio.h>
#include <hdr/game/gam_hud.h>
#include <hdr/game/gam_pauseMode.h>
#include "hdr/system/sys_gameEvents.h"

ALLEGRO_EVENT_QUEUE    *eventQueue;
std::queue<PARA_EVENT> gameEventQueue;

//----------------------------------------------------------------------------------------------------------------------
//
// Init the Game Loop thread event queue
void evt_initGameLoopQueue ()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Start game loop thread
	evt_registerMutex (GAME_MUTEX_NAME);
	evt_registerThread ((functionPtr) evt_processGameEventQueue, GAME_THREAD_NAME);
	evt_setThreadState (true, GAME_THREAD_NAME);

	while (!evt_isThreadReady (GAME_THREAD_NAME))
	{
	}// Wait for thread to be ready to use
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the events put onto the GameLoop queue - run by detached thread
auto evt_processGameEventQueue () -> void *
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_EVENT        gameLoopEvent{};
	static PARA_MUTEX *gameLoopMutex = nullptr;

	while (runThreads)
	{
		evt_setThreadReady (GAME_THREAD_NAME);

		if (evt_shouldThreadRun (GAME_THREAD_NAME))
		{
			PARA_rest (THREAD_DELAY_MS / 10.0f);

			if (!gameEventQueue.empty ())   // stuff in the queue to process
			{
				if (nullptr == gameLoopMutex)
				{
					gameLoopMutex = evt_getMutex (GAME_MUTEX_NAME);    // Cache getting the mutex value
					if (nullptr == gameLoopMutex)
					{
						log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to locate gameloop mutex - [ %s ]", GAME_MUTEX_NAME));
					}
				}

				PARA_lockMutex (gameLoopMutex);   // Blocks if the mutex is locked by another thread

				if (gameEventQueue.front ().game.eventCounter > 0)
				{
					gameEventQueue.front ().game.eventCounter--;

					PARA_unlockMutex (gameLoopMutex);
				}
				else
				{
					gameLoopEvent = gameEventQueue.front ();
					gameEventQueue.pop ();
					PARA_unlockMutex (gameLoopMutex);

					switch (gameLoopEvent.game.eventAction)
					{
						/*
						case GAME_EVENT_DOOR:
							gam_handleDoorTrigger (gameLoopEvent.game.eventData1, gameLoopEvent.game.eventData2);
							break;

						case GAME_EVENT_DOOR_ANIMATE:
							gam_animateDoor (gameLoopEvent.game.eventData1, gameLoopEvent.game.eventData2);
							break;
*/
						case GAME_EVENT_HEALING_START:
							gam_startHealing (gameLoopEvent.game.eventData1, gameLoopEvent.game.eventData2);
							break;

						case GAME_EVENT_HEALING_STOP:
							gam_stopHealing (gameLoopEvent.game.eventData1);
							break;

						case GAME_EVENT_CHANGE_MODE:
							sys_changeMode (gameLoopEvent.game.eventData1, gameLoopEvent.game.eventData2);
							break;

						case GAME_EVENT_PLAY_AUDIO:
							snd_playSound (gameLoopEvent.audio.keyName, gameLoopEvent.audio.loop, gameLoopEvent.audio.gain, 0.0f);
							break;

						case GAME_EVENT_STOP_AUDIO:
							snd_stopSound (gameLoopEvent.audio.keyName);
							break;

						default:
							printf ("ERROR: Unknown Game Event action [ %i ]\n", gameLoopEvent.game.eventAction);
							break;
					}
				}
			}
		}
	}

	printf ("GAME LOOP thread stopped.\n");
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Push an event onto the game queue
void gam_pushNewEvent (union PARA_EVENT newEvent)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_MUTEX *tempMutex;

	tempMutex = evt_getMutex (GAME_MUTEX_NAME);

	if (nullptr == tempMutex)
	{
		return;
	}

	//
	// Put the new event onto the game queue
	PARA_lockMutex (evt_getMutex (GAME_MUTEX_NAME));   // Blocks if the mutex is locked by another thread
	gameEventQueue.push (newEvent);
	PARA_unlockMutex (evt_getMutex (GAME_MUTEX_NAME));
}

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
	ALLEGRO_EVENT event{};

	while (!al_is_event_queue_empty (eventQueue))    // Prevent input lag from not clearing queue
	{
		al_get_next_event (eventQueue, &event);

		switch (event.type)
		{
			//
			// Was the close button on the window pressed
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				quitProgram = true;
				return;

				//
				// Pause the game
			case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
				gam_handlePauseMode(true);
				return;

				//
				// Key pressed down - record state
			case ALLEGRO_EVENT_KEY_DOWN:
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					quitProgram = true;

				if (event.keyboard.keycode == ALLEGRO_KEY_D)
					gam_debugDoors();

				if (event.keyboard.keycode == keyBinding[gamePause].keyValue)
					keyBinding[gamePause].currentlyPressed = true;

				//
				// Only use Pause key when in isPaused state
				if (!isPaused)
				{
					if (event.keyboard.keycode == keyBinding[gameLeft].keyValue)
						keyBinding[gameLeft].currentlyPressed = true;

					if (event.keyboard.keycode == keyBinding[gameRight].keyValue)
						keyBinding[gameRight].currentlyPressed = true;

					if (event.keyboard.keycode == keyBinding[gameDown].keyValue)
						keyBinding[gameDown].currentlyPressed = true;

					if (event.keyboard.keycode == keyBinding[gameUp].keyValue)
						keyBinding[gameUp].currentlyPressed = true;

					if (event.keyboard.keycode == keyBinding[gameAction].keyValue)
						keyBinding[gameAction].currentlyPressed = true;
				}

				break;

			case ALLEGRO_EVENT_KEY_UP:
				if (!isPaused)
				{
					if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
						keyBinding[gameLeft].currentlyPressed = false;

					if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
						keyBinding[gameRight].currentlyPressed = false;

					if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
						keyBinding[gameDown].currentlyPressed = false;

					if (event.keyboard.keycode == ALLEGRO_KEY_UP)
						keyBinding[gameUp].currentlyPressed = false;

					if (event.keyboard.keycode == keyBinding[gameAction].keyValue)
						keyBinding[gameAction].currentlyPressed = false;
				}
				break;
				//
				// Process timer functions
			case ALLEGRO_EVENT_TIMER:
				if (timingTimer == event.timer.source)
					tim_tickTimers (&event.timer);

				if (fadeTimer == event.timer.source)
					tim_runFadeProcess (&event.timer);

				if (splashTimer == event.timer.source)
					tim_changeToGUI (&event.timer);

				if (deckviewLocationTimer == event.timer.source)
					tim_animateLocationIcon (&event.timer);

				break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Push an event onto the relevant queue for action
void evt_pushEvent (int delayCount, int eventType, int eventAction, int data1, int data2, std::string eventString)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_EVENT newEvent{};

	switch (eventType)
	{
		case PARA_EVENT_LOGFILE:
			newEvent.logFile.eventType    = PARA_EVENT_LOGFILE;
			newEvent.logFile.eventAction  = eventAction;
			newEvent.logFile.eventCounter = delayCount;

			newEvent.logFile.logLevel = data1;
			newEvent.logFile.logType  = data2;

			if (eventString.size () > LOGFILE_LINE_SIZE)
			{
				eventString.resize (LOGFILE_LINE_SIZE - 1);
			}
			strcpy (newEvent.logFile.logText, eventString.c_str ());

			log_pushNewEvent (newEvent);
			break;

		case PARA_EVENT_CONSOLE:
			newEvent.console.eventType    = PARA_EVENT_CONSOLE;
			newEvent.console.eventAction  = eventAction;
			newEvent.console.eventCounter = delayCount;

			newEvent.console.lineColor = data1;

			if (eventString.size () > CONSOLE_LINE_SIZE)
			{
				eventString.resize (CONSOLE_LINE_SIZE - 1);
			}

			strcpy (newEvent.console.lineText, eventString.c_str ());

			con_pushNewEvent (newEvent);
			break;

		case PARA_EVENT_GAME:
			newEvent.game.eventType    = PARA_EVENT_GAME;
			newEvent.game.eventAction  = eventAction;
			newEvent.game.eventCounter = delayCount;

			newEvent.game.eventData1 = data1;
			newEvent.game.eventData2 = data2;

			if (eventString.size () > CLIENT_GAME_FILENAME_SIZE)
			{
				eventString.resize (CLIENT_GAME_FILENAME_SIZE - 1);
			}

			strcpy (newEvent.game.lineText, eventString.c_str ());

			gam_pushNewEvent (newEvent);
			break;

		case PARA_EVENT_AUDIO:
//			if (!useSound)
//				return;

			newEvent.audio.eventType    = PARA_EVENT_GAME;
			newEvent.audio.eventAction  = eventAction;
			newEvent.audio.eventCounter = delayCount;

			newEvent.audio.gain = (float) data1;
			newEvent.audio.loop = (ALLEGRO_PLAYMODE) data2;

			if (eventString.size () > CLIENT_GAME_FILENAME_SIZE)
			{
				eventString.reserve (CLIENT_GAME_FILENAME_SIZE - 1);
			}

			strcpy (newEvent.audio.keyName, eventString.c_str ());

			gam_pushNewEvent (newEvent);
			break;

		default:
			printf ("Attempted to push unknown event type [ %i ].\n", eventType);
			break;
	}
}
