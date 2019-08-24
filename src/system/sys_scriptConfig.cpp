#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/system/sys_init.h>
#include <hdr/gui/gui_text.h>
#include "hdr/system/sys_scriptConfig.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the global variables to be shared between host and scripts
void sys_scriptInitVariables()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_scriptAddHostVariable("int quitProgram", &quitProgram);
	sys_scriptAddHostVariable("bool initResourcesAllDone", &initResourcesAllDone);
	sys_scriptAddHostVariable("int currentLanguage", &currentLanguage);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the globals functions that the scripts can call for action in the host
void sys_scriptInitFunctions()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_scriptAddHostFunction("void sys_printConInt(string &in, uint param)", (functionPtr) &sys_scriptPrintInt);
	sys_scriptAddHostFunction("void sys_loadResource(string &in, string &in, int type, int numFrames, int size)", (functionPtr) &sys_loadResource);
	sys_scriptAddHostFunction("bool lvl_loadShipLevel (string &in)", (functionPtr) &lvl_loadShipLevel);
	sys_scriptAddHostFunction("void gui_addKeyAndText(string &in, string &in)", (functionPtr) &gui_addKeyAndText);
	sys_scriptAddHostFunction("string gui_getString(string &in)", (functionPtr) &gui_getString);
}

//----------------------------------------------------------------------------------------------------------------------
//
// This is how we call a script from the Host program : Name in Script : Name to call from host
void sys_scriptInitScriptFunctions()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_scriptAddScriptFunction("void script_loadAllResources()", "script_loadAllResources");
	sys_scriptAddScriptFunction("void script_initGUI()", "script_initGUI");
}