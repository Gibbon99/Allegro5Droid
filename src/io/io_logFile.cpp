#include <hdr/system/sys_shutdown.h>
#include <hdr/io/io_console.h>
#include <hdr/system/sys_eventsEngine.h>

#include "hdr/io/io_logFile.h"

std::queue<PARA_EVENT> loggingEventQueue;
static bool            fileLoggingOn = false;
FILE                   *logFileHandle;

//----------------------------------------------------------------------------------------------------------------------
//
// Open the log file for writing - pass in fileName
// Returns TRUE/FALSE for operation
bool openLogFile(const char *logFileName)
//----------------------------------------------------------------------------------------------------------------------
{
#if defined (WIN32)
	logFileHandle = fopen(logFileName, "w");
	setbuf(logFileHandle, nullptr);	// Set as unbuffered stream
#endif

#if defined __gnu_linux__
	logFileHandle = fopen(logFileName, "w");
	setbuf(logFileHandle, nullptr);    // Set as unbuffered stream
#endif

	return logFileHandle != nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write the passed string to the open logfile
void io_writeToFile(const std::string &textToWrite)
//----------------------------------------------------------------------------------------------------------------------
{
	int bytesWritten;

	if (!fileLoggingOn)
	{
		return;
	}

#if defined _WIN32
	bytesWritten = fprintf(logFileHandle, "%s", textToWrite.c_str());
#endif

#if defined __gnu_linux__
	bytesWritten = fprintf(logFileHandle, "%s\n", textToWrite.c_str());
#endif

	if (bytesWritten < 0)
	{
		printf("Write to logfile failed.\n");
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write the current time and date to the log file
void logTimeToFile()
//----------------------------------------------------------------------------------------------------------------------
{
	char tmptime[64];
	char tmpdate[64];

	if (!fileLoggingOn)
	{
		return;
	}

#if defined (WIN32)
	_strtime ( tmptime );
	_strdate ( tmpdate );
#else
	time_t    currentTime;
	struct tm *localTime;

	// Get current time
	currentTime = time(nullptr);

	// Convert it to local time
	localTime = localtime(&currentTime);

	sprintf(tmptime, "%02i:%02i:%02i", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
	sprintf(tmpdate, "%02i-%02i-%02i", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
#endif

	io_writeToFile(sys_getString("%s\t%s", tmpdate, tmptime));
}

//----------------------------------------------------------------------------------------------------------------------
//
// if the log file is open - close it
void io_closeLogFile()
//----------------------------------------------------------------------------------------------------------------------
{
	if (fileLoggingOn)
	{
		io_writeToFile("Log file closed:");
		logTimeToFile();
		io_writeToFile("-----------------------------------------------------------------------------\n\n");
#if defined (WIN32)
		_close ( logFile );
#else
		fclose(logFileHandle);
#endif
		fileLoggingOn = false;
	}
}

//----------------------------------------------------------------------------------------------------------------------
// start the log file - if possible
// set fileLoggingOn to true
bool io_startLogFile(const char *logFileName)
//----------------------------------------------------------------------------------------------------------------------
{
	fileLoggingOn = false;

	if (!openLogFile(logFileName))
	{
		printf("%s\n", sys_getString("Couldn't create the logfile - check file permissions or disk space.").c_str());
		fileLoggingOn = false;
		return false;
	}
	else
	{
		fileLoggingOn = true;
		io_writeToFile("-----------------------------------------------------------------------------");
		io_writeToFile("Log file opened:");
		logTimeToFile();
		io_writeToFile("Logfile started...");
		return true;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Push an event onto the logging queue
void log_pushNewEvent(union PARA_EVENT newEvent)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_MUTEX *tempMutex;

	tempMutex = evt_getMutex(LOGGING_MUTEX_NAME);

	if (nullptr == tempMutex)
	{
		return;
	}

	//
	// Put the new event onto the logfile queue
	PARA_lockMutex(evt_getMutex(LOGGING_MUTEX_NAME));   // Blocks if the mutex is locked by another thread
	loggingEventQueue.push(newEvent);
	PARA_unlockMutex(evt_getMutex(LOGGING_MUTEX_NAME));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Push a logging event onto the logging queue
void log_logMessage(int logLevel, std::string logText)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_EVENT  newEvent{};
	std::string tempText;
	int lineColor = BLACK;

	sprintf((char *) tempText.c_str(), "[ %f ] - ", PARA_getTime());
	tempText.append(logText);

	if (LOG_LEVEL_EXIT == logLevel)
	{
		printf("[ %f ] - ERROR : %s\n", PARA_getTime(), logText.c_str());
		PARA_rest(1000);
		sys_shutdownToSystem();
	}

	switch (logLevel)
	{
		case LOG_LEVEL_NONE:
			break;

		case LOG_LEVEL_INFO:
			logText.insert(0, "INFO | ");
			break;

		case LOG_LEVEL_ERROR:
			logText.insert(0, "ERROR | ");
			break;

		case LOG_LEVEL_DEBUG:
			logText.insert(0, "DEBUG | ");
			break;

		case LOG_LEVEL_CONSOLE:
			logText.insert(0, "CONSOLE | ");
			break;

		default:
			break;
	}
	//
	// Prevent string buffer overflow in the event structure
	if (logText.size() > LOGFILE_LINE_SIZE - 1)
	{
		logText.resize(LOGFILE_LINE_SIZE - 1);
	}

	newEvent.logFile.eventType   = PARA_EVENT_LOGFILE;
	newEvent.logFile.eventAction = LOGFILE_EVENT_LOG;

	newEvent.logFile.logLevel = logLevel;
	strcpy(newEvent.logFile.logText, logText.c_str());

	if (fileLoggingOn)
	{
		log_pushNewEvent(newEvent);
	}

	if (isDoneConsole)
	{
		newEvent.console.eventType   = PARA_EVENT_CONSOLE;
		newEvent.console.eventAction = CONSOLE_EVENT_LOG;

		newEvent.console.lineColor = lineColor;
		strcpy(newEvent.console.lineText, logText.c_str());

		con_pushNewEvent(newEvent);
	}
	else
	{
		printf("Console not ready - [ %f ] - %s\n", PARA_getTime(), logText.c_str());
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the events put onto the logging queue - run by detached thread
void *io_processLoggingEventQueue()
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_EVENT        logFileEvent{};
	static PARA_MUTEX *logfileMutex = nullptr;

	while (runThreads)
	{
		evt_setThreadReady(LOGGING_THREAD_NAME);

		if (evt_shouldThreadRun(LOGGING_THREAD_NAME))
		{
			PARA_rest(THREAD_DELAY_MS);

			if (!loggingEventQueue.empty())   // stuff in the queue to process
			{
				if (nullptr == logfileMutex)
				{
					logfileMutex = evt_getMutex(LOGGING_MUTEX_NAME);    // Cache getting the mutex value
					if (nullptr == logfileMutex)
					{
						log_logMessage(LOG_LEVEL_EXIT, sys_getString("Unable to locate logfile mutex - [ %s ]", LOGGING_MUTEX_NAME));
					}
				}
				PARA_lockMutex(logfileMutex);   // Blocks if the mutex is locked by another thread

				logFileEvent = loggingEventQueue.front();
				loggingEventQueue.pop();

				PARA_unlockMutex(logfileMutex);

				if (logFileEvent.logFile.eventCounter > 0)
				{
					logFileEvent.logFile.eventCounter--;
					evt_pushEvent(logFileEvent.logFile.eventCounter, logFileEvent.logFile.eventType, logFileEvent.logFile.eventAction, logFileEvent.logFile.logLevel, logFileEvent.logFile.logType, logFileEvent.logFile.logText);
				}
				else
				{
					switch (logFileEvent.logFile.eventAction)
					{
						case LOGFILE_EVENT_START:
							io_startLogFile(logFileEvent.logFile.logText);
							break;

						case LOGFILE_EVENT_STOP:
							io_closeLogFile();
							break;

						case LOGFILE_EVENT_LOG:
							io_writeToFile(logFileEvent.logFile.logText);
							break;

						default:
							printf("ERROR: Unknown logfile action [ %i ]\n", logFileEvent.logFile.eventAction);
							break;
					}
				}
			}
		}
	}

	printf("LOGGING thread stopped.\n");
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the logging thread and mutex, start the logfile
void io_initLogfile()
//----------------------------------------------------------------------------------------------------------------------
{
//
// Start logging thread and logfile
	evt_registerMutex(LOGGING_MUTEX_NAME);
	evt_registerThread((functionPtr) io_processLoggingEventQueue, LOGGING_THREAD_NAME);
	evt_setThreadState(true, LOGGING_THREAD_NAME);

	while (!evt_isThreadReady(LOGGING_THREAD_NAME))
	{
	}// Wait for thread to be ready to use

	evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_START, 0, 0, "logfile.log");
}