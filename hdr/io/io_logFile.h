#pragma once

#include <queue>
#include "system/sys_main.h"
//#include <hdr/io/io_console.h>

enum LOG_LEVEL
{
	LOG_LEVEL_EXIT = 0,
	LOG_LEVEL_INFO,
	LOG_LEVEL_NONE,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_CONSOLE,
};

// Push an event onto the logging queue
void log_pushNewEvent(union PARA_EVENT newEvent);

// Push a logging event onto the logging queue
void log_logMessage(int logLevel, std::string logText);

// Process the events put onto the logging queue - run by detached thread
void *io_processLoggingEventQueue();

// Start the logging thread and mutex, start the logfile
void io_initLogfile();