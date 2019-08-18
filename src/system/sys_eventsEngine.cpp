#include <hdr/io/io_logFile.h>
#include <hdr/io/io_console.h>
#include "hdr/system/sys_eventsEngine.h"

typedef struct
{
	PARA_THREAD *thread = nullptr;
	bool        run = false;
	bool        ready = false;
	std::string name = "";
} _registeredThreads;

typedef struct
{
	PARA_MUTEX  *mutex = nullptr;
	std::string name = "";
} _registeredMutexes;

std::vector<_registeredThreads> registeredThreads;
std::vector<_registeredMutexes> registeredMutexes;

bool runThreads = true;     // Master flag to control state of detached threads

//----------------------------------------------------------------------------------------------------------------------
//
// Set thread ready
void evt_setThreadReady(std::string threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredThreads.empty())
		return;

	for (auto& threadItr : registeredThreads)
	{
		if (threadName == threadItr.name)
		{
			threadItr.ready = true;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// See if the thread is ready or not
bool evt_isThreadReady(std::string threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredThreads.empty())
		return false;

	for (const auto& threadItr : registeredThreads)
	{
		if (threadName == threadItr.name)
		{
			return threadItr.ready;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Should this thread be running
bool evt_shouldThreadRun(const std::string threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredThreads.empty())
	{
		printf("Attempting to start thread [ %s ] but thread is not created.\n", threadName.c_str());
		PARA_rest(100);     // Give thread time to startup
		return false;
	}

	for (const auto& threadItr : registeredThreads)
	{
		if (threadName == threadItr.name)
			return threadItr.run;
	}

	printf("Attempting to start thread [ %s ] but thread is not created.\n", threadName.c_str());
	PARA_rest(100);     // Give thread time to startup
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Control thread running and processing based on thread name
void evt_setThreadState(int newState, const std::string threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredThreads.empty())
	{
		printf("Attempting to change the state of thread [ %s ] but thread is not created.\n", threadName.c_str());
		PARA_rest(100);     // Give thread time to startup
		return;
	}

	for (auto &threadItr : registeredThreads)
	{
		if (threadName == threadItr.name)
		{
			threadItr.run = newState;
			return;
		}
	}
	printf("Attempting to change the state of thread [ %s ] but thread is not created.\n", threadName.c_str());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Stop threads
void evt_stopThreads()
//----------------------------------------------------------------------------------------------------------------------
{
	runThreads = false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Keep a list of threads that have been created.
void evt_registerThread(functionPtr threadFunction, const std::string threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	_registeredThreads newThread;

	newThread.thread = PARA_createThread(*threadFunction, threadName);
	newThread.name   = threadName;
	newThread.run    = false;
	newThread.ready  = false;

	registeredThreads.push_back(newThread);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Keep a list of mutexes that have been created, and remove at shutdown
void evt_registerMutex(const std::string mutexName)
//----------------------------------------------------------------------------------------------------------------------
{
	_registeredMutexes newMutex;

	newMutex.mutex = PARA_createMutex(mutexName);
	newMutex.name  = mutexName;

	registeredMutexes.push_back(newMutex);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy all the mutexes
void evt_destroyMutexes()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto mutexItr : registeredMutexes)
	{
		PARA_destroyMutex(mutexItr.mutex);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find a mutex based on its name
PARA_MUTEX *evt_getMutex(const std::string mutexName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredMutexes.empty())
	{
		printf("Attempting to access mutex [ %s ] but mutex pool is empty.\n", mutexName.c_str());
		return nullptr;
	}

	for (const auto &mutexItr : registeredMutexes)
	{
		if (mutexItr.name == mutexName)
		{
			return mutexItr.mutex;
		}
	}

	log_logMessage(LOG_LEVEL_EXIT, sys_getString("Unable to locate mutex [ %s ]", mutexName.c_str()));
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Push an event onto the relevant queue for action
void evt_pushEvent(int delayCount, int eventType, int eventAction, int data1, int data2, std::string eventString)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_EVENT newEvent{};

	switch (eventType)
	{
		case PARA_EVENT_LOGFILE:
			newEvent.logFile.eventType   = PARA_EVENT_LOGFILE;
			newEvent.logFile.eventAction = eventAction;
			newEvent.logFile.eventCounter = delayCount;

			newEvent.logFile.logLevel = data1;
			newEvent.logFile.logType  = data2;

			if (eventString.size() > LOGFILE_LINE_SIZE)
				eventString.resize(LOGFILE_LINE_SIZE - 1);
			strcpy(newEvent.logFile.logText, eventString.c_str());

			log_pushNewEvent(newEvent);
			break;

		case PARA_EVENT_CONSOLE:
			newEvent.console.eventType = PARA_EVENT_CONSOLE;
			newEvent.console.eventAction = eventAction;
			newEvent.console.eventCounter = delayCount;

			newEvent.console.lineColor = data1;

			if (eventString.size() > CONSOLE_LINE_SIZE)
				eventString.resize(CONSOLE_LINE_SIZE - 1);

			strcpy(newEvent.console.lineText, eventString.c_str());

			con_pushNewEvent(newEvent);
			break;
/*
		case PARA_EVENT_GAME:
			newEvent.game.eventType = PARA_EVENT_GAME;
			newEvent.game.eventAction = eventAction;
			newEvent.game.eventCounter = delayCount;

			newEvent.game.eventData1 = data1;
			newEvent.game.eventData2 = data2;

			if (eventString.size() > CLIENT_GAME_FILENAME_SIZE)
				eventString.resize(CLIENT_GAME_FILENAME_SIZE - 1);

			strcpy(newEvent.game.lineText, eventString.c_str());

			gam_pushNewEvent(newEvent);

//			printf("[ %lu ] - Got GAME event [ %s ]\n", PARA_getTime(), eventString.c_str());

			break;

		case PARA_EVENT_MAIN:
			newEvent.main.eventType = PARA_EVENT_MAIN;
			newEvent.main.eventAction = eventAction;
			newEvent.main.eventCounter = delayCount;

			if (eventString.size() > CLIENT_GAME_FILENAME_SIZE)
				eventString.resize(CLIENT_GAME_FILENAME_SIZE - 1);

			strcpy(newEvent.main.eventText, eventString.c_str());

			newEvent.main.function = (functionPtr)data1;
			sys_pushNewMainEvent(newEvent);
			break;

		case PARA_EVENT_AUDIO:
			if (!useSound)
				return;

			newEvent.audio.eventType = PARA_EVENT_AUDIO;
			newEvent.audio.eventAction = eventAction;
			newEvent.audio.eventCounter = delayCount;

			newEvent.audio.eventData1 = data1;
			newEvent.audio.eventData2 = data2;

			if (eventString.size() > CLIENT_GAME_FILENAME_SIZE)
				eventString.reserve(CLIENT_GAME_FILENAME_SIZE - 1);

			strcpy(newEvent.audio.lineText, eventString.c_str());

			aud_pushNewEvent(newEvent);
			break;
*/
		default:
			printf("Attempted to push unknown event type [ %i ].\n", eventType);
			break;
	}
}
