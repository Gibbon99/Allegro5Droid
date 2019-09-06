#pragma once

#include "system/sys_main.h"

typedef int (*functionPtr)( ... );

#define LOGFILE_LINE_SIZE               196
#define CONSOLE_LINE_SIZE               96
#define CLIENT_GAME_FILENAME_SIZE       32
#define MAIN_THREAD_SIZE                32

#define THREAD_DELAY_MS                 0.1

#define LOGGING_THREAD_NAME             "loggingThread"
#define LOGGING_MUTEX_NAME              "loggingMutex"

#define CONSOLE_THREAD_NAME             "consoleThread"
#define CONSOLE_MUTEX_NAME              "consoleMutex"

#define GAME_THREAD_NAME                "gameLoopThread"
#define GAME_MUTEX_NAME                 "gameMutex"

enum PARA_EVENT_TYPES
{
	PARA_EVENT_LOGFILE = 0,
	PARA_EVENT_CONSOLE,
	PARA_EVENT_GAME,
	PARA_EVENT_MAIN,
	PARA_EVENT_AUDIO
};

enum eventActions
{
	LOGFILE_EVENT_START = 0,
	LOGFILE_EVENT_STOP,
	LOGFILE_EVENT_LOG,

	CONSOLE_EVENT_START,
	CONSOLE_EVENT_STOP,
	CONSOLE_EVENT_LOG,

	MAIN_LOOP_EVENT,
	MAIN_LOOP_EVENT_ADD_BULLET,
	MAIN_LOOP_EVENT_REMOVE_DROID_PHYSICS,

	USER_EVENT_AUDIO,
	AUDIO_PLAY_SAMPLE,

	GAME_EVENT_DOOR,
	GAME_EVENT_DOOR_ANIMATE,
	GAME_DOOR_STATE_ENTER,
	GAME_DOOR_STATE_EXIT
};

//
// LOGFILE event structure
//
typedef struct
{
	int eventType;
	int eventAction;
	int eventCounter;

	int  logLevel;
	int  logType;
	char logText[LOGFILE_LINE_SIZE];
} __LOG_FILE;

//
// CONSOLE event structure
//
typedef struct
{
	int eventType;
	int eventAction;
	int eventCounter;

	int  lineColor;
	char lineText[CONSOLE_LINE_SIZE];
} __CONSOLE;

typedef struct
{
	int eventType;
	int eventAction;
	int eventCounter;

	int         interval;
	functionPtr function;
	char        eventText[MAIN_THREAD_SIZE];
} __MAIN_THREAD;
//
// GAME event structure
//
typedef struct
{
	int eventType;
	int eventAction;
	int eventCounter;

	int  eventData1;
	int  eventData2;
	char lineText[CLIENT_GAME_FILENAME_SIZE];
} __GAME;

union PARA_EVENT
{
	__LOG_FILE    logFile;
	__CONSOLE     console;
	__GAME        game;
	__MAIN_THREAD main;
	__GAME        audio;
};

extern bool runThreads;     // Master flag to control state of detached threads

// Should this thread be running
bool evt_shouldThreadRun(const std::string threadName);

// Control thread running and processing based on thread name
void evt_setThreadState(int newState, const std::string threadName);

// Stop threads
void evt_stopThreads();

// Keep a list of threads that have been created.
void evt_registerThread(functionPtr threadFunction, const std::string threadName);

// Keep a list of mutexes that have been created, and remove at shutdown
void evt_registerMutex(const std::string mutexName);

// Destroy all the mutexes
void evt_destroyMutexes();

// Find a mutex based on its name
PARA_MUTEX *evt_getMutex(const std::string mutexName);

// Push an event onto the relevant queue for action
void evt_pushEvent(int delayCount, int eventType, int eventAction, int data1, int data2, std::string eventString);

// See if the thread is ready or not
bool evt_isThreadReady(std::string threadName);

// Set thread ready
void evt_setThreadReady(std::string threadName);