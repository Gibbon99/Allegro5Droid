#include "data/sharedDefines.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Load all the file resources
void script_loadAllResources ()
//----------------------------------------------------------------------------------------------------------------------
{
	string levelName;

	sys_loadResource ("collision", "collosion1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("endTransmission1", "endTransmission1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("greenAlert", "greenAlert.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("lift1", "lift1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("introSound", "scrollBeeps.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("transferDeadlock", "transferdeadlock.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("yellowAlert", "yellowAlert.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("console1", "console1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("endTransmission2", "endTransmission2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("keypressBad", "keypressBad.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("lift2", "lift2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("start1", "start1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("transferMove", "transferMove.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("console2", "console2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("energyHeal", "energyHeal.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("keyPressGood", "keyPressGood.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("lift3", "lift3.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("start2", "start2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("transferStage1", "transferStage1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("damage", "damage.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("explode1", "explode1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("laser", "laser.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("lowEnergy", "lowEnergy.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("startAll", "startAll.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("transferStage2", "transferStage2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("disrupter", "disruptor.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("explode2", "explode2.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("levelPowerDown", "levelShutdown.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("redAlert", "redAlert.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("transfer1", "transfer1.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("transferStart", "transferStart.wav", RESOURCE_AUDIO, 0, 0);
	sys_loadResource ("door", "door.wav", RESOURCE_AUDIO, 0, 0);

	sys_loadResource ("splash", "splash.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource ("hud", "hud.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource ("tut_healing", "tut_healing.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource ("tut_lift", "tut_lift.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource ("tut_terminal", "tut_terminal.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource ("alltiles", "alltiles.bmp", RESOURCE_BITMAP, 0, 0);
	sys_loadResource ("alltiles_128", "alltiles_128.bmp", RESOURCE_BITMAP, 0, 0);
	//
	// Explosion
	sys_loadResource ("explosion", "explosion.bmp", RESOURCE_SPRITE, 25, 0);
	io_spriteSetAlphaColor ("explosion", 0, 0, 0);
	//
	// Bullets
	sys_loadResource ("bullet_001", "bullet_001.bmp", RESOURCE_SPRITE, 8, 0);
	io_spriteSetAlphaColor ("bullet_001", 255, 0, 255);
	sys_loadResource ("bullet_476", "bullet_476.bmp", RESOURCE_SPRITE, 8, 0);
	io_spriteSetAlphaColor ("bullet_476", 255, 0, 255);
	sys_loadResource ("bullet_821", "bullet_821.bmp", RESOURCE_SPRITE, 8, 0);
	io_spriteSetAlphaColor ("bullet_821", 255, 0, 255);
	//
	// Sprite imaages
	sys_loadResource ("001", "001.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("001", 0, 0, 0);
	sys_loadResource ("123", "123.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("123", 255, 0, 255);
	sys_loadResource ("139", "139.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("139", 255, 0, 255);
	sys_loadResource ("247", "247.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("247", 255, 0, 255);
	sys_loadResource ("249", "249.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("249", 255, 0, 255);
	sys_loadResource ("296", "296.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("296", 255, 0, 255);
	sys_loadResource ("302", "302.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("302", 255, 0, 255);
	sys_loadResource ("329", "329.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("329", 255, 0, 255);
	sys_loadResource ("420", "420.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("420", 255, 0, 255);
	sys_loadResource ("476", "476.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("476", 255, 0, 255);
	sys_loadResource ("493", "493.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("493", 255, 0, 255);
	sys_loadResource ("516", "516.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("516", 255, 0, 255);
	sys_loadResource ("571", "571.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("571", 255, 0, 255);
	sys_loadResource ("598", "598.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("598", 255, 0, 255);
	sys_loadResource ("614", "614.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("614", 255, 0, 255);
	sys_loadResource ("615", "615.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("615", 255, 0, 255);
	sys_loadResource ("629", "629.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("629", 255, 0, 255);
	sys_loadResource ("711", "711.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("711", 255, 0, 255);
	sys_loadResource ("742", "742.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("742", 255, 0, 255);
	sys_loadResource ("751", "751.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("751", 255, 0, 255);
	sys_loadResource ("821", "821.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("821", 255, 0, 255);
	sys_loadResource ("834", "834.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("834", 255, 0, 255);
	sys_loadResource ("883", "883.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("883", 255, 0, 255);
	sys_loadResource ("999", "999.bmp", RESOURCE_SPRITE, 9, 0);
	io_spriteSetAlphaColor ("999", 255, 0, 255);

	sys_loadResource ("db_sprite", "db_001.bmp", RESOURCE_SPRITE, 32, 0);
	io_spriteSetAlphaColor ("db_sprite", 0, 0, 0);

	sys_loadResource ("sideView", "sideview.dat", RESOURCE_SHIP_VIEW, 0, 0);

	lvl_loadShipLevel ("116-newDeck0.dat");
	lvl_loadShipLevel ("116-newDeck1.dat");
	lvl_loadShipLevel ("116-newDeck2.dat");
	lvl_loadShipLevel ("116-newDeck3.dat");
	lvl_loadShipLevel ("116-newDeck4.dat");
	lvl_loadShipLevel ("116-newDeck5.dat");
	lvl_loadShipLevel ("116-newDeck6.dat");
	lvl_loadShipLevel ("116-newDeck8.dat");
	lvl_loadShipLevel ("116-newDeck9.dat");
	lvl_loadShipLevel ("116-newDeck10.dat");
	lvl_loadShipLevel ("116-newDeck11.dat");
	lvl_loadShipLevel ("116-newDeck12.dat");
	lvl_loadShipLevel ("116-newDeck13.dat");
	lvl_loadShipLevel ("116-newDeck14.dat");
	lvl_loadShipLevel ("116-newDeck15.dat");
	lvl_loadShipLevel ("116-newDeck16.dat");
	lvl_loadShipLevel ("116-newDeck17.dat");
	lvl_loadShipLevel ("116-newDeck18.dat");
	lvl_loadShipLevel ("116-newDeck19.dat");
	lvl_loadShipLevel ("116-newDeck20.dat");

	sys_loadResource ("console", "console.ttf", RESOURCE_FONT, 0, 8);
	sys_loadResource ("intro", "intro.ttf", RESOURCE_FONT, 0, 14);
	sys_loadResource ("gui", "gui.ttf", RESOURCE_FONT, 0, 14);

	initResourcesAllDone     = true;
	showDebugPhysics         = false;
	doorAnimSpeed            = 4.5f;
	pixelsPerMeter           = 12.0f;        // 12 pixels is 1 meter
	wayPointDestinationSize  = 1.0f;         // 1 meter
	shipDamping              = 0.0f;
	playerRadius             = 12.0f;
	playerFriction           = 0.5f;
	playerElastic            = 0.8f;
	playerAcceleration       = 150.0f;
	playerMaxSpeed           = 250.0f;
	gravity                  = 190.0f;
	sideviewDrawScale        = 0.5f;
	droidAnimateSpeed        = 60.0f;
	explodeAnimateSpeed      = 120.0f;
	badHealthFactor          = 0.10f;        // Percent of health before it's considered bad CHANGE ME
	collisionDamageInflicted = 3;
	collisionExplosionDamage = 10;
	bulletAnimSpeed          = 10.0f;
	bulletMoveSpeed          = 0.5f;
	bulletDensity            = 0.2f;
	healingAnimSpeed         = 10.0f;
	droidBeenShotValue       = 90.0f;
	splashTimeout            = 5;
	databaseAnimTime         = 10.0f;
	//
	// Counter for how long droid remembers being shot
	droidBeenShotValue       = 3.0f;
	witnessShootValue        = 1.5f;
	witnessTransferValue     = 1.5f;
	//
	// Amounts to influence chance to shoot
	ai_beenShot              = 0.7f;
	ai_healthAmount          = 0.3f;
	ai_playerVisible         = 0.2f;
	ai_witnessShoot          = 0.0f;    // 0.4f
	ai_witnessTransfer       = 0.0f;    // 0.6f
	ai_greenFactor           = 0.1f;
	ai_yellowFactor          = 0.2f;
	ai_redFactor             = 0.4f;
	deckViewRatio            = 0.8f;
	//
	// Array starting size
	numStartingBullets       = 10;
}