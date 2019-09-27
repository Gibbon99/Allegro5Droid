#include <hdr/system/sys_gameEvents.h>
#include <hdr/system/sys_configFile.h>
#include <hdr/system/sys_font.h>
#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_fileSystem.h>
#include <hdr/io/io_console.h>
#include <hdr/system/sys_scriptEngine.h>
#include <hdr/system/sys_timerFunctions.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_render.h>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_droidAI.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_lifts.h>
#include <hdr/game/gam_healing.h>
#include <hdr/system/sys_init.h>
#include <hdr/game/gam_game.h>
#include <hdr/game/gam_bullet.h>
//#include "system/sys_init.h"

ALLEGRO_TIMER   *timingTimer;
ALLEGRO_TIMER   *fadeTimer;
ALLEGRO_TIMER   *splashTimer;
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP  *windowIcon;

bool initResourcesAllDone = false;

int fullScreen = 1; // Set for checkbox in GUI script

#define VSYNC_WAIT 1
#define VSYNC_FORCE_OFF 2

//----------------------------------------------------------------------------------------------------------------------
//
// Get information about connected adapters and monitors
void sys_getMonitorInfo ()
//----------------------------------------------------------------------------------------------------------------------
{
	ALLEGRO_MONITOR_INFO info;
	int                  numAdapters = 0;
	int                  dpi         = 0;

	numAdapters = al_get_num_video_adapters ();

	log_logMessage (LOG_LEVEL_INFO, sys_getString ("%d adapters found...", numAdapters));

	for (int i = 0; i < numAdapters; i++)
	{
		al_get_monitor_info (i, &info);
		log_logMessage (LOG_LEVEL_INFO, sys_getString ("Adapter %d: ", i));

		dpi = al_get_monitor_dpi (i);
		log_logMessage (LOG_LEVEL_INFO, sys_getString ("(%d, %d) - (%d, %d) - dpi: %d", info.x1, info.y1, info.x2, info.y2, dpi));

		al_set_new_display_adapter (i);
		log_logMessage (LOG_LEVEL_INFO, sys_getString ("   Available fullscreen display modes:"));

		for (int j = 0; j < al_get_num_display_modes (); j++)
		{
			ALLEGRO_DISPLAY_MODE mode;

			al_get_display_mode (j, &mode);

			log_logMessage (LOG_LEVEL_INFO, sys_getString ("   Mode %3d: %4d x %4d, %d Hz", j, mode.width, mode.height, mode.refresh_rate));
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the display
bool sys_initDisplay ()
//----------------------------------------------------------------------------------------------------------------------
{
	ALLEGRO_DISPLAY_MODE mode;
	int                  screenTypeFlag = 0;

	switch (screenType)
	{
		case 0:
			screenTypeFlag = ALLEGRO_WINDOWED;
			break;

		case 1:
			screenTypeFlag = ALLEGRO_FULLSCREEN_WINDOW;
			break;

		case 2:
			screenTypeFlag = ALLEGRO_FULLSCREEN;
			break;

		default:
			screenTypeFlag = ALLEGRO_WINDOWED;
			break;
	}

	al_set_new_display_option (0, VSYNC_WAIT, ALLEGRO_SUGGEST);

	al_set_new_display_flags (screenTypeFlag);

	display = al_create_display (windowWidth, windowHeight);
	if (nullptr == display)
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create window.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

//	printf("%i\n", display->refresh_rate);

//	sys_getMonitorInfo();

	al_get_display_mode (0, &mode);

	displayRefreshRate = mode.refresh_rate;

	sys_initRenderVariables ();

	windowIcon = al_load_bitmap ("icon.bmp");
	if (nullptr != windowIcon)
	{
		al_set_display_icon (display, windowIcon);
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run init for system startup
void sys_initAll ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!al_install_system (ALLEGRO_VERSION_INT, nullptr))
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start the Allegro library", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	runThreads = true;
	io_initLogfile ();
	con_initConsole ();

	while (!isDoneConsole)
	{
	}
	//
	// Start fileSystem first - need to be able to read config file and set paths
	if (!io_startFileSystem ())
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start the filesystem.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	al_set_physfs_file_interface (); // Set to current main thread

	//
	// Read startup values from config file - need window sizes and display options
	if (!cfg_getStartupValues ())
	{
		return;
	}
	//
	// Start display
	if (!sys_initDisplay ())
	{
		return;
	}
	al_install_keyboard ();  // Needs to be before Events can be registered
	al_install_mouse ();
	//
	// Start an event queue
	evt_initEvents ();
	al_register_event_source (eventQueue, al_get_display_event_source (display));
	al_register_event_source (eventQueue, al_get_keyboard_event_source ());
//	al_register_event_source(eventQueue, al_get_mouse_event_source());
	//
	// Display output on screen
	sys_changeMode (MODE_CONSOLE, false);
	//
	// Start, load and compile scripts
	if (!sys_initScriptEngine ())
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start Script Engine.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	//
	// Load images from addon
	if (!al_init_image_addon ())
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start Image addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	//
	// Use Audio library
	if (!al_install_audio ())
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start Audio.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	//
	// Start Audio Codec library
	if (!al_init_acodec_addon ())
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start Audio Codec addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	//
	// Create all the timers
	if (!tim_initAllTimers ())
		return;
	//
	// Create built in library font
	if (!fnt_initSystemFont ())
	{
		return;
	}
	//
	// Start TTF addon
	if (!al_init_ttf_addon())
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start True Type Font addon.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}

	gl_setupDroidToSpriteLookup ();

	//
	// Load resources from file system
	sys_runScriptFunction ("script_loadAllResources", "");

	gam_calcTileTexCoords ();

	sys_runScriptFunction ("script_initGUI", std::string ());

	io_setDefaultKeybindings ();
	io_setKeyDescription ();

	sys_setupPhysicsEngine ();
	sys_setupPlayerPhysics ();
	gam_setupLifts ();

	evt_initGameLoopQueue ();

	ai_setupAITree ();

	gam_getDBInformation ();

	for (auto levelItr : shipLevel)
	{
		gam_initDroidValues (levelItr.first);
		gam_findHealingTiles (levelItr.first);
	}

	lvl_changeToLevel ("Staterooms", 0);

	gam_initPlayerValues ();

	bul_initBulletArraySize ();

//	al_start_timer(splashTimer);
	sys_changeMode (MODE_GAME, true);

	gam_startNewGame();

}
