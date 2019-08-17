#include <hdr/system/sys_gameEvents.h>
#include <hdr/system/sys_configFile.h>
#include <hdr/system/sys_font.h>
#include "system/sys_init.h"

ALLEGRO_TIMER   *timingTimer;
ALLEGRO_DISPLAY *display;

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
	int dpi = 0;

	numAdapters = al_get_num_video_adapters();

	printf("%d adapters found...\n", numAdapters);

	for (int i = 0; i < numAdapters; i++)
	{
		al_get_monitor_info(i, &info);
		printf("Adapter %d: ", i);

		dpi = al_get_monitor_dpi(i);
		printf("(%d, %d) - (%d, %d) - dpi: %d\n", info.x1, info.y1, info.x2, info.y2, dpi);

		al_set_new_display_adapter(i);
		printf("   Available fullscreen display modes:\n");

		for (int j = 0; j < al_get_num_display_modes(); j++)
		{
			ALLEGRO_DISPLAY_MODE mode;

			al_get_display_mode(j, &mode);

			printf("   Mode %3d: %4d x %4d, %d Hz\n", j, mode.width, mode.height, mode.refresh_rate);
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

	al_set_new_display_option(0, VSYNC_WAIT, ALLEGRO_SUGGEST);

//	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

	display = al_create_display(windowWidth, windowHeight);
	if (nullptr == display)
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create window.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

//	printf("%i\n", display->refresh_rate);

	sys_getMonitorInfo();

	al_get_display_mode(0, &mode);

	displayRefreshRate = mode.refresh_rate;
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
}