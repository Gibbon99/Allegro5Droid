#include <hdr/io/io_logFile.h>
#include "hdr/system/sys_scriptFunctions.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Print a string and int param to the console from a script
void sys_scriptPrintInt(std::string textIn, int intIn)
//----------------------------------------------------------------------------------------------------------------------
{
	log_logMessage(LOG_LEVEL_CONSOLE, sys_getString("%s - %i", textIn.c_str(), intIn));
}