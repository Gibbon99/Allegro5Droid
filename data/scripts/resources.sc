#include "data/sharedDefines.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Load all the file resources
void script_loadAllResources()
//----------------------------------------------------------------------------------------------------------------------
{
	string levelName;

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
	sys_loadResource("alltiles", "alltiles.bmp", RESOURCE_BITMAP, 0, 0);

	lvl_loadShipLevel("newDeck1.dat");
	lvl_loadShipLevel("newDeck2.dat");
	lvl_loadShipLevel("newDeck3.dat");
	lvl_loadShipLevel("newDeck4.dat");
	lvl_loadShipLevel("newDeck5.dat");
	lvl_loadShipLevel("newDeck6.dat");
	lvl_loadShipLevel("newDeck8.dat");
	lvl_loadShipLevel("newDeck9.dat");
	lvl_loadShipLevel("newDeck10.dat");
	lvl_loadShipLevel("newDeck11.dat");
	lvl_loadShipLevel("newDeck12.dat");
	lvl_loadShipLevel("newDeck13.dat");
	lvl_loadShipLevel("newDeck14.dat");
	lvl_loadShipLevel("newDeck15.dat");
	lvl_loadShipLevel("newDeck16.dat");
	lvl_loadShipLevel("newDeck17.dat");
	lvl_loadShipLevel("newDeck18.dat");
	lvl_loadShipLevel("newDeck19.dat");
	lvl_loadShipLevel("newDeck20.dat");
}