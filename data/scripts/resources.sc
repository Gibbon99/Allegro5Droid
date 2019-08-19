#include "data/sharedDefines.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Load all the file resources
void script_loadAllResources()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_loadResource("collision", "collosion1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("endTransmission1", "endTransmission1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("greenAlert", "greenAlert.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("lift1", "lift1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("introSound", "scrollBeeps.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("transferDeadlock", "transferdeadlock.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("yellowAlert", "yellowAlert.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("console1", "console1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("endTransmission2", "endTransmission2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("keypressBad", "keypressBad.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("lift2", "lift2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("start1", "start1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("transferMove", "transferMove.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("console2", "console2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("energyHeal", "energyHeal.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("keyPressGood", "keyPressGood.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("lift3", "lift3.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("start2", "start2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("transferStage1", "transferStage1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("damage", "damage.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("explode1", "explode1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("laser", "laser.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("lowEnergy", "lowEnergy.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("startAll", "startAll.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("transferStage2", "transferStage2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("disrupter", "disruptor.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("explode2", "explode2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("levelPowerDown", "levelShutdown.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("redAlert", "redAlert.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("transfer1", "transfer1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("transferStart", "transferStart.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource("door", "door.wav", RESOURCE_AUDIO, 0, 0);

	sys_loadResource("splash", "splash.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource("hud", "hud.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource("tut_healing", "tut_healing.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource("tut_lift", "tut_lift.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource("tut_terminal", "tut_terminal.bmp", RESOURCE_BITMAP, 0, 0);
}