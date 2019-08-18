#pragma once

#include "system/sys_main.h"
#include "script/angelscript.h"
#include "script/scriptbuilder.h"
#include "system/sys_scriptConfig.h"

//-----------------------------------------------------------------------------
//
// Structure to hold all the functions in the script
//-----------------------------------------------------------------------------

typedef struct
{
	asIScriptFunction *funcID;
	bool              fromScript; // Is this created from a console script
	std::string       functionName;
	std::string       scriptName;
	bool              param1; // Does this function need something passed to it
} _scriptFunctionName;

typedef struct
{
	std::string scriptFunctionName;
	const void  *hostFunctionPtr;
} _hostScriptFunctions;

extern std::vector<std::string>     scriptFileCache;

// Shutdown the script engine and remove the instance, cleaning up memory
void sys_stopScriptEngine();

// Start the script engine
bool sys_initScriptEngine();

// Execute a script function from the host
void sys_runScriptFunction(const std::string functionName, const std::string param);

// Add a variable to the array holding all the script accessible variables
void sys_scriptAddHostVariable(const std::string varName, void *varPtr);

// Register all the functions to make available to the script
void sys_scriptAddHostFunction(const std::string funcName, functionPtr funcPtr);

// Cache the functionID from functions in the scripts
void sys_scriptCacheScriptFunctions();
