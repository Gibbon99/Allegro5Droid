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

std::vector<_registeredThreads> registeredThreads{};
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
			if (!threadItr.ready)
			{
				threadItr.ready = true;

				printf("Thread [ %s ] set to ready = true\n", threadName.c_str());

				return;
			}
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

	newThread.name   = threadName;
	newThread.run    = false;
	newThread.ready  = false;
	registeredThreads.push_back(newThread);
	//
	// Create vector element first
	// Thread was being created too fast and running before the vector element was created
	// resulting in no elements being available when size() was tested
	registeredThreads[registeredThreads.size() - 1].thread = PARA_createThread(*threadFunction, threadName);
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
