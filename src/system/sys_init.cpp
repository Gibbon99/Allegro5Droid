#include <hdr/system/sys_gameEvents.h>
#include <hdr/system/sys_configFile.h>
#include <hdr/system/sys_font.h>
#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_fileSystem.h>
#include <hdr/io/io_console.h>
#include <hdr/system/sys_scriptEngine.h>
#include "system/sys_init.h"

ALLEGRO_TIMER   *timingTimer;
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP  *windowIcon;

#define VSYNC_WAIT 1
#define VSYNC_FORCE_OFF 2

//----------------------------------------------------------------------------------------------------------------------
//
// Get information about connected adapters and monitors
void sys_getMonitorInfo()
//----------------------------------------------------------------------------------------------------------------------
{
	ALLEGRO_MONITOR_INFO info;
	int                  numAdapters = 0;
	int                  dpi         = 0;

	numAdapters = al_get_num_video_adapters();

	log_logMessage(LOG_LEVEL_INFO, sys_getString("%d adapters found...", numAdapters));

	for (int i = 0; i < numAdapters; i++)
	{
		al_get_monitor_info(i, &info);
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Adapter %d: ", i));

		dpi = al_get_monitor_dpi(i);
		log_logMessage(LOG_LEVEL_INFO, sys_getString("(%d, %d) - (%d, %d) - dpi: %d", info.x1, info.y1, info.x2, info.y2, dpi));

		al_set_new_display_adapter(i);
		log_logMessage(LOG_LEVEL_INFO, sys_getString("   Available fullscreen display modes:"));

		for (int j = 0; j < al_get_num_display_modes(); j++)
		{
			ALLEGRO_DISPLAY_MODE mode;

			al_get_display_mode(j, &mode);

			log_logMessage(LOG_LEVEL_INFO, sys_getString("   Mode %3d: %4d x %4d, %d Hz", j, mode.width, mode.height, mode.refresh_rate));
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the display
bool sys_initDisplay()
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

	al_set_new_display_option(0, VSYNC_WAIT, ALLEGRO_SUGGEST);

	al_set_new_display_flags(screenTypeFlag);

	display = al_create_display(windowWidth, windowHeight);
	if (nullptr == display)
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create window.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

//	printf("%i\n", display->refresh_rate);

//	sys_getMonitorInfo();

	al_get_display_mode(0, &mode);

	displayRefreshRate = mode.refresh_rate;

	sys_initRenderVariables();

	windowIcon = al_load_bitmap("icon.bmp");
	if (nullptr != windowIcon)
	{
		al_set_display_icon(display, windowIcon);
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run init for system startup
void sys_initAll()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!al_install_system(ALLEGRO_VERSION_INT, nullptr))
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start the Allegro library", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
	}
	runThreads = true;
	io_initLogfile();
	con_initConsole();

	while (!isDoneConsole);

	sys_changeMode(MODE_CONSOLE);

	if (!io_startFileSystem())
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not start the filesystem.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return;
	}
	al_set_physfs_file_interface(); // Set to current main thread

	sys_initScriptEngine();
	//
	// Load images
	al_init_image_addon();

	al_install_keyboard();
	al_install_mouse();
	//
	// Read startup values from config file
	if (!cfg_getStartupValues())
	{
		return;
	}
	//
	// Start an event queue
	evt_initEvents();
	//
	// Start display
	if (!sys_initDisplay())
	{
		return;
	}
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	//
	// Create timing to track FPS, thinkFPS and frameTime
	timingTimer = al_create_timer(1.0f);
	if (nullptr == timingTimer)
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Timers. Exiting", "Could not start the initial timer.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
	}
	al_register_event_source(eventQueue, al_get_timer_event_source(timingTimer));
	al_start_timer(timingTimer);
	//
	// Create built in library font
	if (!fnt_initSystemFont())
	{
		return;
	}
//	sys_changeMode(MODE_GAME);
}