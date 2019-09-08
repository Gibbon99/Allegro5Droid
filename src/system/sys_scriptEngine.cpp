#include <hdr/script/scriptstdstring.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_fileSystem.h>
#include <hdr/system/sys_eventsEngine.h>
#include "hdr/system/sys_scriptEngine.h"

#define MODULE_NAME                 "clientModule"

int numScriptFiles       = 0;
int numScriptFilesLoaded = 0;

bool scriptReloaded                  = false;

std::vector<std::string> scriptFileCache;

//----------------------------------------------------------------------------------------------------------------------
//
// AngelScript engine
//
//----------------------------------------------------------------------------------------------------------------------
asIScriptEngine  *scriptEngine;
asIScriptContext *context            = nullptr;
CScriptBuilder   builder;
bool             scriptEngineStarted = false;

//----------------------------------------------------------------------------------------------------------------------
//
// This is used to hold the function ID and is used when creating functions
// from within a running script. A copy of scriptFunctionName is used as
// the source data.
//
//----------------------------------------------------------------------------------------------------------------------

std::vector<_scriptFunctionName> scriptFunctions;

//----------------------------------------------------------------------------------------------------------------------
//
// Struct to hold host variables mapping
//
//----------------------------------------------------------------------------------------------------------------------

std::vector<_hostScriptFunctions> hostVariables;

//----------------------------------------------------------------------------------------------------------------------
//
// Struct to hold host function mapping to script function names
//
//----------------------------------------------------------------------------------------------------------------------

std::vector<_scriptFunctionName>  scriptFunctionName;
std::vector<_hostScriptFunctions> hostScriptFunctions;

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new function that is defined in a script - create name to call it from host
void sys_scriptAddScriptFunction(std::string funcName, std::string hostCallName)
//----------------------------------------------------------------------------------------------------------------------
{
	_scriptFunctionName tempFunctionName;

	tempFunctionName.fromScript   = true;
	tempFunctionName.funcID       = nullptr;
	tempFunctionName.functionName = funcName;
	tempFunctionName.param1       = false;
	tempFunctionName.scriptName   = hostCallName;

	scriptFunctionName.push_back(tempFunctionName);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Error codes for AngelScript
std::string sys_getScriptError(int errNo)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (errNo)
	{
		case 0:
			return ("asSUCCESS");
			break;

		case -1:
			return ("asERROR");
			break;

		case -2:
			return ("asCONTEXT_ACTIVE");
			break;

		case -3:
			return ("asCONTEXT_NOT_FINISHED");
			break;

		case -4:
			return ("asCONTEXT_NOT_PREPARED");
			break;

		case -5:
			return ("asINVALID_ARG");
			break;

		case -6:
			return ("asNO_FUNCTION");
			break;

		case -7:
			return ("asNOT_SUPPORTED");
			break;

		case -8:
			return ("asINVALID_NAME");
			break;

		case -9:
			return ("asNAME_TAKEN");
			break;

		case -10:
			return ("asINVALID_DECLARATION");
			break;

		case -11:
			return ("asINVALID_OBJECT");
			break;

		case -12:
			return ("asINVALID_TYPE");
			break;

		case -13:
			return ("asALREADY_REGISTERED");
			break;

		case -14:
			return ("asMULTIPLE_FUNCTIONS");
			break;

		case -15:
			return ("asNO_MODULE");
			break;

		case -16:
			return ("asNO_GLOBAL_VAR");
			break;

		case -17:
			return ("asINVALID_CONFIGURATION");
			break;

		case -18:
			return ("asINVALID_INTERFACE");
			break;

		case -19:
			return ("asCANT_BIND_ALL_FUNCTIONS");
			break;

		case -20:
			return ("asLOWER_ARRAY_DIMENSION_NOT_REGISTERED");
			break;

		case -21:
			return ("asAPP_CANT_INTERFACE_DEFAULT_ARRAY");
			break;

		default:
			return ("Unknown error type.");
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Print messages from script compiler to console
void scr_Output(const asSMessageInfo *msg, void *param)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *type = "ERR ";

	if (msg->type == asMSGTYPE_WARNING)
	{
		type = "WARN";
	}

	else if (msg->type == asMSGTYPE_INFORMATION)
	{
		type = "INFO";
	}

	log_logMessage(LOG_LEVEL_INFO, sys_getString("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a variable to the array holding all the script accessible variables
void sys_scriptAddHostVariable(const std::string varName, void *varPtr)
//----------------------------------------------------------------------------------------------------------------------
{
	_hostScriptFunctions tempVar;

	tempVar.scriptFunctionName = varName;
	tempVar.hostFunctionPtr    = varPtr;

	if (scriptEngine->RegisterGlobalProperty(varName.c_str(), (void *) varPtr) < 0)
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: Error: Couldn't register variable - [ %s ]", varName.c_str()));
	}

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: Registered variable - [ %s ]", varName.c_str()));
	hostVariables.push_back(tempVar);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Register all the functions to make available to the script
void sys_scriptAddHostFunction(const std::string funcName, functionPtr funcPtr)
//----------------------------------------------------------------------------------------------------------------------
{
	int            returnCode;
	static asDWORD callType = -1;

	_hostScriptFunctions tempFunc;

	if (callType < 0)
	{
		if (!strstr(asGetLibraryOptions(), "AS_MAX_PORTABILTY"))
		{
			callType = asCALL_CDECL;
		}
		else
		{
			callType = asCALL_STDCALL;
		}
	}

	tempFunc.scriptFunctionName = funcName;
	tempFunc.hostFunctionPtr    = (void *) funcPtr;

	returnCode = scriptEngine->RegisterGlobalFunction(funcName.c_str(), asFUNCTION (funcPtr), asCALL_CDECL);
	if (returnCode < 0)
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to registerGlobalFunction [ %s ] - [ %s ]", funcName.c_str(), sys_getScriptError(returnCode).c_str()));
	}

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: Registered function - [ %s ]", funcName.c_str()));
	hostScriptFunctions.push_back(tempFunc);
}


//-----------------------------------------------------------------------------
//
// Compile the script files into a single module
bool sys_loadAndCompileScripts()
//-----------------------------------------------------------------------------
{
	int  retCode  = 0;
	char *memoryBuffer;
	int  fileSize = 0;

	retCode = builder.StartNewModule(scriptEngine, MODULE_NAME);

	if (retCode < 0)
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to start script module."));
	}

	for (const auto& scriptItr : scriptFileCache)
	{
		fileSize     = io_getFileSize(scriptItr.c_str());
		if (fileSize < 0)
			log_logMessage(LOG_LEVEL_INFO, sys_getString("Fatal error getting script file size [ %s ].", scriptItr.c_str()));

		memoryBuffer = (char *) malloc(sizeof(char) * fileSize);    // memleak
		if (nullptr == memoryBuffer)
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString("Fatal memory allocation error when loading script."));
		}

		io_getFileIntoMemory(scriptItr.c_str(), memoryBuffer);

		retCode = builder.AddSectionFromMemory(scriptItr.c_str(), memoryBuffer, fileSize, 0);
		switch (retCode)
		{
			case 0:
				log_logMessage(LOG_LEVEL_INFO, sys_getString("Section with same name has already been added [ %s ]. Ignoring.", scriptItr.c_str()));
				break;

			case 1:
				log_logMessage(LOG_LEVEL_INFO, sys_getString("Section has been added [ %s ]", scriptItr.c_str()));
				break;

			default:
				free(memoryBuffer);
				log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to add script section [ %s ].", scriptItr.c_str()));
				break;
		}
		free(memoryBuffer);

	}
	//
	// Build the script from the loaded sections using ScriptBuilder
	//
	retCode = builder.BuildModule();
	if (retCode < 0)
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to build the script module."));
		return false;
	}

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Compiled scripts."));

	// Cache the functionID from functions in the scripts
	sys_scriptCacheScriptFunctions();

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Restart the script engine
void sys_restartScriptEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_stopScriptEngine();

	scriptFunctions.clear();
	scriptFunctionName.clear();

	scriptReloaded = true;

	sys_initScriptEngine();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Shutdown the script engine and remove the instance, cleaning up memory
void sys_stopScriptEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	if (context != nullptr)
	{
		context->Suspend();
		context->Unprepare();
		context->Release();
		context = nullptr;
	}
	if (scriptEngine != nullptr)
	{
		scriptEngine->ShutDownAndRelease();
		scriptEngine = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the script engine
bool sys_initScriptEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	io_getScriptFileNames();

	scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

	if (nullptr == scriptEngine)
	{
		scriptEngineStarted = false;
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: Error: Failed to create script engine- [ %s ]", sys_getScriptError(0).c_str()));
	}

	// The script compiler will write any compiler messages to the callback.
	scriptEngine->SetMessageCallback(asFUNCTION (scr_Output), nullptr, asCALL_CDECL);

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: Scriptengine started."));

	RegisterStdString(scriptEngine);

//	RegisterScriptArray(scriptEngine, true);

	// What version are we running
	log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: ScriptEngine version - [ %s ]", asGetLibraryVersion()));

	// What options are compiled
	log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: Options - [ %s ]", asGetLibraryOptions()));

	// Add variables available to the script
	sys_scriptInitVariables();

	// Add all the functions that the scripts can access
	sys_scriptInitFunctions();

	sys_loadAndCompileScripts();

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cache the functionID from functions in the scripts
void sys_scriptCacheScriptFunctions()
//----------------------------------------------------------------------------------------------------------------------
{
	_scriptFunctionName tempFunctionName;
	asIScriptModule     *mod;

	sys_scriptInitScriptFunctions();

	mod = scriptEngine->GetModule(MODULE_NAME);

	//
	// Get function ID's for each function we will call in the script
	//
	for (auto funcItr : scriptFunctionName)
	{
		tempFunctionName.funcID = mod->GetFunctionByDecl(funcItr.functionName.c_str());

		if (tempFunctionName.funcID == 0)
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to get function ID for [ %s ]", funcItr.functionName.c_str()));
		}
		else
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString("Func ID for [ %s ] - [ %i ]", funcItr.functionName.c_str(), tempFunctionName.funcID));

			//
			// Store the funcID and name to execute
			//
			tempFunctionName.param1       = funcItr.param1;
			tempFunctionName.functionName = funcItr.functionName;
			tempFunctionName.scriptName   = funcItr.scriptName;
			tempFunctionName.fromScript   = funcItr.fromScript;

			scriptFunctions.push_back(tempFunctionName);
		}
	}
	//
	// Do some preparation before execution
	context = scriptEngine->CreateContext();

	scriptEngineStarted = true;
}

//----------------------------------------------------------------------
//
// Print out the variables that Angelscript knows about internally - DEBUGGING
void sys_debugPrintVariables()
//----------------------------------------------------------------------
{
	asUINT stackLevel = 0;

	asIScriptContext *ctx    = asGetActiveContext();
	asIScriptEngine  *engine = ctx->GetEngine();

	int  typeId1      = ctx->GetThisTypeId();
	void *varPointer1 = ctx->GetThisPointer();
	if (typeId1)
	{
		log_logMessage(LOG_LEVEL_DEBUG, sys_getString(" this = 0x%x\n", varPointer1));
	}

	int         numVars = ctx->GetVarCount();
	for (asUINT n       = 0; n < numVars; n++)
	{
		int  typeId      = ctx->GetVarTypeId(n);
		void *varPointer = ctx->GetAddressOfVar(n);
		if (typeId == engine->GetTypeIdByDecl("int"))
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString(" %s = %d\n", ctx->GetVarDeclaration(n, stackLevel), *(int *) varPointer));
		}
		else if (typeId == engine->GetTypeIdByDecl("string"))
		{
			auto *str = (std::string *) varPointer;
			if (str)
			{
				log_logMessage(LOG_LEVEL_INFO, sys_getString(" %s = '%s'\n", ctx->GetVarDeclaration(n, stackLevel), str->c_str()));
			}
			else
			{
				log_logMessage(LOG_LEVEL_INFO, sys_getString(" %s = <null>\n", ctx->GetVarDeclaration(n, stackLevel)));
			}
		}
		else
		{
			auto *getValue = (b2Vec2 *) varPointer;
			log_logMessage(LOG_LEVEL_INFO, sys_getString(" %s = {...}\n", ctx->GetVarDeclaration(n, stackLevel)));
			log_logMessage(LOG_LEVEL_INFO, sys_getString("Debug [ %3.3f %3.3f ]\n", getValue->x, getValue->y));
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Execute a script function from the host
void sys_runScriptFunction(const std::string functionName, const std::string param)
//----------------------------------------------------------------------------------------------------------------------
{
	int returnCode = 0;
	int testInt    = 0;

	if (nullptr == context)
	{
		return;
	}

	for (const auto &funcItr : scriptFunctions)
	{
//		log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Looking for function [ %s ] to [ %s ] to execute", functionName.c_str(), funcItr.scriptName.c_str()));

		if (scriptEngine != context->GetEngine())
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString("Context DOES NOT belong to this engine."));
		}

		if (funcItr.scriptName == functionName)
		{

			if (scriptEngine != funcItr.funcID->GetEngine())
			{
				log_logMessage(LOG_LEVEL_INFO, sys_getString("Function DOES NOT belong to this engine."));
			}

			context->Prepare(funcItr.funcID);
			//
			// Is there a parameter being passed for this function
			if (!param.empty())
			{
				//
				// See if it's a number or not
				if (isdigit(param.c_str()[0]))
				{
					//
					// Parameter is a number - convert before passing it in
					testInt = strtol(param.c_str(), nullptr, 10);
					context->SetArgAddress(0, &testInt);
				}
				else
				{
					//
					// Send in a string
					context->SetArgAddress(0, (void *) param.c_str());
				}
			}

			returnCode = context->Execute();
			if (returnCode != asEXECUTION_FINISHED)
			{
				//
				// The execution didn't finish as we had planned. Determine why.
				//
				if (returnCode == asEXECUTION_ABORTED)
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: The script was aborted before it could finish. Probably it timed out."));
				}

				else if (returnCode == asEXECUTION_EXCEPTION)
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Script: The script ended with an exception."));
					//
					// Write some information about the script exception
					//
					asIScriptFunction *func = context->GetExceptionFunction();
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Func: [ %s ]", func->GetDeclaration()));
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Module: [ %s ]", func->GetModuleName()));
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Section: [ %s ]", func->GetScriptSectionName()));
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Line: [ %i ]", context->GetExceptionLineNumber()));
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Desc: [ %s ]", context->GetExceptionString()));
				}
				else
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("The script ended for an unknown reason [ %i ].", returnCode));
				}
			}
			return;
		}
	}
	log_logMessage(LOG_LEVEL_INFO, sys_getString("Function [ %s ] not found in script", functionName.c_str()));
}