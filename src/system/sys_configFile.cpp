#include <hdr/gui/gui_text.h>
#include <hdr/system/sys_audio.h>
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
	al_destroy_config(configFile);

	return true;
}