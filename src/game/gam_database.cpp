#include <physfs.h>
#include <hdr/game/gam_game.h>
#include <hdr/gui/gui_text.h>
#include <hdr/game/gam_player.h>
#include <hdr/gui/gui_main.h>
#include <hdr/io/io_resources.h>
#include <hdr/game/gam_droids.h>
#include "hdr/game/gam_database.h"

std::vector<_dataBaseEntry> dataBaseEntry;
ALLEGRO_CONFIG              *dataBaseFile;

float       baseGameSpeed   = 1.0f;
std::string currentDatabaseIndex;
int         playerDroidType = 0;

//-----------------------------------------------------------------------------------------------------
//
// Set the local player droid type variable for moving through the database list
void gam_setLocalDroidType ()
//-----------------------------------------------------------------------------------------------------
{
	playerDroidType = playerDroid.droidType;
	currentDatabaseIndex = playerDroid.playerDroidTypeDBIndex;
}

//-----------------------------------------------------------------------------------------------------
//
// Enter the database mode
void gam_enterDatabaseMode ()
//-----------------------------------------------------------------------------------------------------
{
	databaseScrollBox.text                                    = gui_getString (currentDatabaseIndex);
	databaseScrollBox.__GUI_element.scrollBox.currentChar     = 0;
	databaseScrollBox.__GUI_element.scrollBox.numLinesToPrint = 0;
	databaseScrollBox.scrollBoxLineText.clear ();
	sys_loadResource ("db_sprite", currentDatabaseIndex, RESOURCE_SPRITE, 32, -1);
}

//-----------------------------------------------------------------------------------------------------
//
// Get the previous database droid record
void gam_previousDatabaseDroid ()
//-----------------------------------------------------------------------------------------------------
{
	if (playerDroidType > 0)
		{
			playerDroidType--;
			currentDatabaseIndex = "db_" + gl_getSpriteName (playerDroidType);
			gam_enterDatabaseMode ();
		}
	else
		{
			// Play bad beep sound
		}
}

//-----------------------------------------------------------------------------------------------------
//
// Get the next database droid information
void gam_nextDatabaseDroid ()
//-----------------------------------------------------------------------------------------------------
{
	if (playerDroidType < MAX_DROID_TYPES)
		{
			playerDroidType++;
			currentDatabaseIndex = "db_" + gl_getSpriteName (playerDroidType);
			gam_enterDatabaseMode ();
		}
	else
		{
			// Play bad beep sound
		}
}

//-----------------------------------------------------------------------------------------------------
//
// Load a droid database file.
// Load into memory and then parse using LoadData with ini file
bool io_getDBDroidInfo (const std::string &fileName)
//-----------------------------------------------------------------------------------------------------
{
	PHYSFS_sint64  fileLength;
	_dataBaseEntry tempDataBaseEntry;

	dataBaseFile = al_load_config_file (fileName.c_str ());

	tempDataBaseEntry.maxHealth = (int) strtol (al_get_config_value (dataBaseFile, "droidInfo", "max_health"), nullptr, 10);

	if (dataBaseEntry.empty ())    // 001 base droid stats
		{
			tempDataBaseEntry.maxSpeed = strtof (al_get_config_value (dataBaseFile, "droidInfo", "max_speed"), nullptr);
			tempDataBaseEntry.maxSpeed /= baseGameSpeed;

			tempDataBaseEntry.accelerate = strtof (al_get_config_value (dataBaseFile, "droidInfo", "accelerate"), nullptr);
			tempDataBaseEntry.accelerate /= baseGameSpeed;
		}
	else
		{
			//
			// All other droid speeds and acceleration are based on player droid values
			// Easy to change overall speed of the game
			//
			tempDataBaseEntry.maxSpeed   = dataBaseEntry[0].maxSpeed + strtof (al_get_config_value (dataBaseFile, "droidInfo", "max_speed"), nullptr);
			tempDataBaseEntry.accelerate = dataBaseEntry[0].accelerate + strtof (al_get_config_value (dataBaseFile, "droidInfo", "accelerate"), nullptr);

			tempDataBaseEntry.maxSpeed /= baseGameSpeed;
			tempDataBaseEntry.accelerate /= baseGameSpeed;

		}

	tempDataBaseEntry.score        = strtol (al_get_config_value (dataBaseFile, "droidInfo", "max_speed"), nullptr, 10);
	tempDataBaseEntry.bounceDamage = strtol (al_get_config_value (dataBaseFile, "droidInfo", "bounce_damage"), nullptr, 10);
	tempDataBaseEntry.canShoot     = strtol (al_get_config_value (dataBaseFile, "droidInfo", "can_shoot"), nullptr, 10);
	tempDataBaseEntry.rechargeTime = strtol (al_get_config_value (dataBaseFile, "droidInfo", "rechargeTime"), nullptr, 10);
	tempDataBaseEntry.bulletType   = strtol (al_get_config_value (dataBaseFile, "droidInfo", "bullet_type"), nullptr, 10);

	//
	// Map the type of bullet to the sprite image for it
	//
	// Set to -1 if can_shoot == 0
	switch (tempDataBaseEntry.bulletType)
		{
			case 0: // small double laser
				tempDataBaseEntry.bulletType = BULLET_TYPE_NORMAL;
			break;

			case 1: // Large single laser
				tempDataBaseEntry.bulletType = BULLET_TYPE_SINGLE;
			break;

			case 2: // Large double laser
				tempDataBaseEntry.bulletType = BULLET_TYPE_DOUBLE;
			break;

			case 3: // Disrupter
				tempDataBaseEntry.bulletType = BULLET_TYPE_DISRUPTER;
			break;

			default:
				break;
		}

	tempDataBaseEntry.chanceToShoot   = strtol (al_get_config_value (dataBaseFile, "droidInfo", "chance_to_shoot"), nullptr, 10);
	tempDataBaseEntry.bulletDamage    = strtol (al_get_config_value (dataBaseFile, "droidInfo", "bullet_damage"), nullptr, 10);
	tempDataBaseEntry.disrupterImmune = strtol (al_get_config_value (dataBaseFile, "droidInfo", "disrupter_immune"), nullptr, 10);
	tempDataBaseEntry.tokenCount      = strtol (al_get_config_value (dataBaseFile, "droidInfo", "token_count"), nullptr, 10);
	tempDataBaseEntry.height          = strtol (al_get_config_value (dataBaseFile, "droidInfo", "height"), nullptr, 10);
	tempDataBaseEntry.weight          = strtol (al_get_config_value (dataBaseFile, "droidInfo", "weight"), nullptr, 10);
	tempDataBaseEntry.dbImageFileName = al_get_config_value (dataBaseFile, "droidInfo", "dbImageFileName");

	//
	// Read in key for text language
	//
	tempDataBaseEntry.description = al_get_config_value (dataBaseFile, "droidInfo", "description");
	tempDataBaseEntry.className   = al_get_config_value (dataBaseFile, "droidInfo", "className");
	tempDataBaseEntry.drive       = al_get_config_value (dataBaseFile, "droidInfo", "drive");
	tempDataBaseEntry.brain       = al_get_config_value (dataBaseFile, "droidInfo", "brain");
	tempDataBaseEntry.weapon      = al_get_config_value (dataBaseFile, "droidInfo", "weapon");
	tempDataBaseEntry.sensor1     = al_get_config_value (dataBaseFile, "droidInfo", "sensor1");
	tempDataBaseEntry.sensor2     = al_get_config_value (dataBaseFile, "droidInfo", "sensor2");
	tempDataBaseEntry.sensor3     = al_get_config_value (dataBaseFile, "droidInfo", "sensor3");
	tempDataBaseEntry.notes       = al_get_config_value (dataBaseFile, "droidInfo", "notes");

	//
	// Now get the text string using that text key for the current language
	//
	tempDataBaseEntry.description = gui_getString (tempDataBaseEntry.description);
	tempDataBaseEntry.className   = gui_getString (tempDataBaseEntry.className);
	tempDataBaseEntry.drive       = gui_getString (tempDataBaseEntry.drive);
	tempDataBaseEntry.brain       = gui_getString (tempDataBaseEntry.brain);
	tempDataBaseEntry.weapon      = gui_getString (tempDataBaseEntry.weapon);
	tempDataBaseEntry.sensor1     = gui_getString (tempDataBaseEntry.sensor1);
	tempDataBaseEntry.sensor2     = gui_getString (tempDataBaseEntry.sensor2);
	tempDataBaseEntry.sensor3     = gui_getString (tempDataBaseEntry.sensor3);
	tempDataBaseEntry.notes       = gui_getString (tempDataBaseEntry.notes);
	//
	// Add in index pointers
//	tempDataBaseEntry.imageIndex = imageIndexCounter; // Start at first image and increment
//	imageIndexCounter++;

	//
	// Close the file and free resources
	al_destroy_config (dataBaseFile);

	dataBaseEntry.push_back (tempDataBaseEntry);

	return true;
}

//------------------------------------------------------------
//
// Get and build up information for droid database files
bool gam_getDBInformation ()
//------------------------------------------------------------
{
	if (!io_getDBDroidInfo ("001.txt"))
		return false;

	if (!io_getDBDroidInfo ("123.txt"))
		return false;

	if (!io_getDBDroidInfo ("139.txt"))
		return false;

	if (!io_getDBDroidInfo ("247.txt"))
		return false;

	if (!io_getDBDroidInfo ("249.txt"))
		return false;

	if (!io_getDBDroidInfo ("296.txt"))
		return false;

	if (!io_getDBDroidInfo ("302.txt"))
		return false;

	if (!io_getDBDroidInfo ("329.txt"))
		return false;

	if (!io_getDBDroidInfo ("420.txt"))
		return false;

	if (!io_getDBDroidInfo ("476.txt"))
		return false;

	if (!io_getDBDroidInfo ("493.txt"))
		return false;

	if (!io_getDBDroidInfo ("516.txt"))
		return false;

	if (!io_getDBDroidInfo ("571.txt"))
		return false;

	if (!io_getDBDroidInfo ("598.txt"))
		return false;

	if (!io_getDBDroidInfo ("614.txt"))
		return false;

	if (!io_getDBDroidInfo ("615.txt"))
		return false;

	if (!io_getDBDroidInfo ("629.txt"))
		return false;

	if (!io_getDBDroidInfo ("711.txt"))
		return false;

	if (!io_getDBDroidInfo ("742.txt"))
		return false;

	if (!io_getDBDroidInfo ("751.txt"))
		return false;

	if (!io_getDBDroidInfo ("821.txt"))
		return false;

	if (!io_getDBDroidInfo ("834.txt"))
		return false;

	if (!io_getDBDroidInfo ("883.txt"))
		return false;

	if (!io_getDBDroidInfo ("999.txt"))
		return false;

	return true;
}
