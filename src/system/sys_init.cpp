#include <hdr/system/sys_gameEvents.h>
#include <hdr/system/sys_configFile.h>
#include <hdr/system/sys_font.h>
#include "system/sys_init.h"

ALLEGRO_TIMER   *timingTimer;
ALLEGRO_DISPLAY *display;

//----------------------------------------------------------------------------------------------------------------------
//
// Start the display
bool sys_initDisplay()
//----------------------------------------------------------------------------------------------------------------------
{
	display = al_create_display(windowWidth, windowHeight);
	if (nullptr == display)
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create window.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
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