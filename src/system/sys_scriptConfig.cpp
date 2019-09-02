#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/system/sys_init.h>
#include <hdr/gui/gui_text.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_droidAIPatrol.h>
#include <hdr/game/gam_player.h>
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
	sys_scriptAddHostVariable ("float wayPointDestinationSize", &wayPointDestinationSize);
	sys_scriptAddHostVariable ("float shipDamping", &shipDamping);
	sys_scriptAddHostVariable("float playerRadius", &playerRadius);
	sys_scriptAddHostVariable("float playerMass", &playerMass);
	sys_scriptAddHostVariable("float playerFriction", &playerFriction);
	sys_scriptAddHostVariable("float playerElastic", &playerElastic);
	sys_scriptAddHostVariable("float pixelsPerMeter", &pixelsPerMeter);
	sys_scriptAddHostVariable("float playerAcceleration", &playerAcceleration);
	sys_scriptAddHostVariable("float playerMaxSpeed", &playerMaxSpeed);
	sys_scriptAddHostVariable("float gravity", &gravity);
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
	sys_scriptAddHostFunction("void io_spriteSetAlphaColor(string &in, int red, int green, int blue)", (functionPtr) &io_spriteSetAlphaColor);
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