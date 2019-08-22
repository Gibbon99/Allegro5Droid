#include <queue>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include "hdr/io/io_console.h"

std::vector<_conLine> conLines;
std::queue<PARA_EVENT> consoleEventQueue;
bool isDoneConsole = false;
int numVisibleConLines = 0;

//----------------------------------------------------------------------------------------------------------------------
//
// Push an event onto the console queue
void con_pushNewEvent(union PARA_EVENT newEvent)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_MUTEX *tempMutex;

	tempMutex = evt_getMutex(CONSOLE_MUTEX_NAME);

	if (nullptr == tempMutex)
	{
		return;
	}

	//
	// Put the new event onto the console queue
	PARA_lockMutex(evt_getMutex(CONSOLE_MUTEX_NAME));   // Blocks if the mutex is locked by another thread
	consoleEventQueue.push(newEvent);
	PARA_unlockMutex(evt_getMutex(CONSOLE_MUTEX_NAME));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Startup the console
void con_startConsole()
//----------------------------------------------------------------------------------------------------------------------
{
	conLines.reserve(100);
	gam_setVisibleConLines(0);
	isDoneConsole = true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init the console system
void con_initConsole()
//----------------------------------------------------------------------------------------------------------------------
{
	evt_registerMutex(CONSOLE_MUTEX_NAME);
	evt_registerThread((functionPtr) con_processConsoleEventQueue, CONSOLE_THREAD_NAME);
	evt_setThreadState(true, CONSOLE_THREAD_NAME);

	while (!evt_isThreadReady(CONSOLE_THREAD_NAME))
	{
	};    // Wait for thread to be ready to use

	evt_pushEvent(0, PARA_EVENT_CONSOLE, CONSOLE_EVENT_START, 0, 0, "");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Stop the console
void con_stopConsole()
//----------------------------------------------------------------------------------------------------------------------
{
	isDoneConsole = false;
	evt_setThreadState(false, CONSOLE_THREAD_NAME);
	conLines.clear();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Put an entry into the console - just printf if the server
void con_write(int lineColor, const std::string lineText)
//----------------------------------------------------------------------------------------------------------------------
{
	if (!isDoneConsole)
	{
		return;
	}

	__paraColor newLineColor;
	_conLine    newConLine;

	switch (lineColor)
	{
		case WHITE:
			newLineColor.red   = 255;
			newLineColor.green = 255;
			newLineColor.blue  = 255;
			newLineColor.alpha = 255;
			break;

		case RED:
			newLineColor.red   = 255;
			newLineColor.green = 0;
			newLineColor.blue  = 0;
			newLineColor.alpha = 255;
			break;

		case GREEN:
			newLineColor.red   = 0;
			newLineColor.green = 255;
			newLineColor.blue  = 0;
			newLineColor.alpha = 255;
			break;

		case BLUE:
			newLineColor.red   = 0;
			newLineColor.green = 0;
			newLineColor.blue  = 255;
			newLineColor.alpha = 255;
			break;

		case BLACK:
			newLineColor.red   = 0;
			newLineColor.green = 0;
			newLineColor.blue  = 0;
			newLineColor.alpha = 255;
			break;

		default:
			newLineColor.red   = 255;
			newLineColor.green = 255;
			newLineColor.blue  = 255;
			newLineColor.alpha = 255;
			break;
	}

	newConLine.conLineColor = newLineColor;
	newConLine.conLine      = lineText;
	conLines.push_back(newConLine);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add an event to the console queue
void con_print(int lineColor, bool logToFile, const std::string lineText)
//----------------------------------------------------------------------------------------------------------------------
{
	evt_pushEvent(0, PARA_EVENT_CONSOLE, CONSOLE_EVENT_LOG, lineColor, 0, lineText);
	if (logToFile)
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, lineText);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the events put onto the console queue - run by detached thread
void *con_processConsoleEventQueue()
//----------------------------------------------------------------------------------------------------------------------
{
	static PARA_MUTEX *consoleMutex = nullptr;

	PARA_EVENT consoleEvent{};

	while (runThreads)
	{
		evt_setThreadReady(CONSOLE_THREAD_NAME);

		if (evt_shouldThreadRun(CONSOLE_THREAD_NAME))
		{
			PARA_rest(THREAD_DELAY_MS / 10);

			if (!consoleEventQueue.empty())   // stuff in the queue to process
			{
				if (consoleMutex == nullptr)  // if we don't have the mutex value
				{
					consoleMutex = evt_getMutex(CONSOLE_MUTEX_NAME); // then cache it, rather than getting each time
					if (consoleMutex == nullptr)
					{
						log_logMessage(LOG_LEVEL_EXIT, sys_getString("Unable to locate valid mutex - [ % ]", "console"));
					}
				}
				PARA_lockMutex(consoleMutex);   // Blocks if the mutex is locked by another thread

				consoleEvent = consoleEventQueue.front();
				consoleEventQueue.pop();

				PARA_unlockMutex(consoleMutex);

				if (consoleEvent.console.eventCounter > 0)
				{
					consoleEvent.console.eventCounter--;
					evt_pushEvent(consoleEvent.console.eventCounter, consoleEvent.console.eventType, consoleEvent.console.eventAction, consoleEvent.console.lineColor, 0, consoleEvent.console.lineText);
				}
				else
				{
					switch (consoleEvent.console.eventAction)
					{
						case CONSOLE_EVENT_START:
							con_startConsole();
							break;

						case CONSOLE_EVENT_STOP:
							con_stopConsole();
							break;

						case CONSOLE_EVENT_LOG:
							con_write(consoleEvent.console.lineColor, consoleEvent.console.lineText);
							break;

						default:
							printf("ERROR: Unknown console event action [ %i ]\n", consoleEvent.console.eventAction);
							break;
					}
				}
			}
		}
	}

	printf("CONSOLE thread stopped.\n");
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the number of visible console lines to render on one screen
void gam_setVisibleConLines (int fontHeight)
//----------------------------------------------------------------------------------------------------------------------
{
	if (fontHeight == 0)
		fontHeight = 8;

	numVisibleConLines = screenHeight / fontHeight;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw the contents of the console
void con_renderConsole()
//----------------------------------------------------------------------------------------------------------------------
{
	int positionY = 0;
	int positionX = 1;
	int lineCount = 0;

	if (conLines.empty())
		return;

	positionY = screenHeight - 8;

	// Set the current font to use when drawing text
//	ttf_setCurrentFont("console.ttf");

	for (auto consoleItr = conLines.rbegin(); consoleItr != conLines.rend(); ++consoleItr)
	{
		fnt_printSystemFont(positionX, positionY, consoleItr->conLine);

		lineCount++;
		positionY -= 8;
		if ( lineCount == conLines.size())
			break;
	}
}