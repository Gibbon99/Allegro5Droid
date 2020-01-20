#include <hdr/gui/gui_text.h>
#include <hdr/system/sys_audio.h>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_render.h>
#include "hdr/system/sys_configFile.h"

#define CONFIG_FILENAME "config.ini"

ALLEGRO_CONFIG  *configFile;

//----------------------------------------------------------------------------------------------------------------------
//
// Get startup values from config file
bool cfg_getStartupValues()
//----------------------------------------------------------------------------------------------------------------------
{
	configFile = al_load_config_file(CONFIG_FILENAME);
	if (nullptr == configFile)
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not find config file.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	windowWidth = (int)strtol(al_get_config_value(configFile, "main", "windowWidth"), nullptr, 10);
	windowHeight = (int)strtol(al_get_config_value(configFile, "main", "windowHeight"), nullptr, 10);

	screenWidth = (int)strtol(al_get_config_value(configFile, "main", "screenWidth"), nullptr, 10);
	screenHeight = (int)strtol(al_get_config_value(configFile, "main", "screenHeight"), nullptr, 10);

	screenType = (int)strtol(al_get_config_value(configFile, "main", "screenType"), nullptr, 10);
	
	currentLanguage = (int)strtol(al_get_config_value(configFile, "main", "currentLanguage"), nullptr, 10);

	numReserveSamples = (int)strtol(al_get_config_value(configFile, "main", "numReserveSamples"), nullptr, 10);
	if (numReserveSamples < 0) numReserveSamples = 1;
	if (numReserveSamples > MAX_NUM_RESERVE_SAMPLES) numReserveSamples = MAX_NUM_RESERVE_SAMPLES;

	renderBackdrop = (int)strtol(al_get_config_value(configFile, "main", "renderBackdrop"), nullptr, 10);

	enableSound = (bool)strtol(al_get_config_value (configFile, "main", "enableSound"), nullptr, 10);

	volumeLevel = (int)strtol(al_get_config_value (configFile, "main", "volumeLevel"), nullptr, 10);
	if (volumeLevel > MAX_VOLUME_LEVEL) volumeLevel = MAX_VOLUME_LEVEL;
	if (volumeLevel < 0) volumeLevel = 0;

	volumeLevelStr = std::to_string(volumeLevel);

	tileType = al_get_config_value(configFile, "main", "tileType");
	tileColor = al_get_config_value(configFile, "main", "tileColor");

//	al_destroy_config(configFile);

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Update value in currently open config file
void cfg_setConfigValue(std::string whichKey, std::string newValue)
//----------------------------------------------------------------------------------------------------------------------
{
	if (configFile == nullptr)
		{
			log_logMessage(LOG_LEVEL_ERROR, "Config file not open.");
			return;
		}

	al_set_config_value(configFile, "main", whichKey.c_str(), newValue.c_str());

	printf("Saving key [ %s ] - Value [ %s ]", whichKey.c_str(), newValue.c_str());

	al_save_config_file(CONFIG_FILENAME, configFile);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Close the open config file
void cfg_closeConfigFile()
//----------------------------------------------------------------------------------------------------------------------
{
	if (configFile != nullptr)
		{
			al_destroy_config (configFile);
		}
}