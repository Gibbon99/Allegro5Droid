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

	sys_loadResource("001", "001.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("001", 0, 0, 0);
	sys_loadResource("123", "123.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("123", 255, 0, 255);
	sys_loadResource("139", "139.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("139", 255, 0, 255);
	sys_loadResource("247", "247.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("247", 255, 0, 255);
	sys_loadResource("249", "249.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("249", 255, 0, 255);
	sys_loadResource("296", "296.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("296", 255, 0, 255);
	sys_loadResource("302", "302.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("302", 255, 0, 255);
	sys_loadResource("329", "329.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("329", 255, 0, 255);
	sys_loadResource("420", "420.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("420", 255, 0, 255);
	sys_loadResource("476", "476.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("476", 255, 0, 255);
	sys_loadResource("493", "493.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("493", 255, 0, 255);
	sys_loadResource("516", "516.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("516", 255, 0, 255);
	sys_loadResource("571", "571.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("571", 255, 0, 255);
	sys_loadResource("598", "598.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("598", 255, 0, 255);
	sys_loadResource("614", "614.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("614", 255, 0, 255);
	sys_loadResource("615", "615.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("615", 255, 0, 255);
	sys_loadResource("629", "629.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("629", 255, 0, 255);
	sys_loadResource("711", "711.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("711", 255, 0, 255);
	sys_loadResource("742", "742.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("742", 255, 0, 255);
	sys_loadResource("751", "751.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("751", 255, 0, 255);
	sys_loadResource("821", "821.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("821", 255, 0, 255);
	sys_loadResource("834", "834.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("834", 255, 0, 255);
	sys_loadResource("883", "883.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("883", 255, 0, 255);
	sys_loadResource("999", "999.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor("999", 255, 0, 255);

	lvl_loadShipLevel("116-newDeck0.dat");
	lvl_loadShipLevel("116-newDeck1.dat");
	lvl_loadShipLevel("116-newDeck2.dat");
	lvl_loadShipLevel("116-newDeck3.dat");
	lvl_loadShipLevel("116-newDeck4.dat");
	lvl_loadShipLevel("116-newDeck5.dat");
	lvl_loadShipLevel("116-newDeck6.dat");
	lvl_loadShipLevel("116-newDeck8.dat");
	lvl_loadShipLevel("116-newDeck9.dat");
	lvl_loadShipLevel("116-newDeck10.dat");
	lvl_loadShipLevel("116-newDeck11.dat");
	lvl_loadShipLevel("116-newDeck12.dat");
	lvl_loadShipLevel("116-newDeck13.dat");
	lvl_loadShipLevel("116-newDeck14.dat");
	lvl_loadShipLevel("116-newDeck15.dat");
	lvl_loadShipLevel("116-newDeck16.dat");
	lvl_loadShipLevel("116-newDeck17.dat");
	lvl_loadShipLevel("116-newDeck18.dat");
	lvl_loadShipLevel("116-newDeck19.dat");
	lvl_loadShipLevel("116-newDeck20.dat");

	initResourcesAllDone = true;

	pixelsPerMeter          = 12.0f;        // 12 pixels is 1 meter
	wayPointDestinationSize = 1.0f;         // 1 meter
	shipDamping             = 0.0f;
	playerRadius            = 12.0f;
	playerMass              = 10.5f;
	playerFriction          = 0.5f;
	playerElastic           = 0.8f;
	playerAcceleration      = 150.101f;
	playerMaxSpeed          = 250.00f;
	gravity                 = 190.20f;
}