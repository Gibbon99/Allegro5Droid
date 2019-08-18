#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_gameFrameRender.h>
#include "hdr/system/sys_wrapper.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Lock a mutex
void PARA_lockMutex(PARA_MUTEX *thisMutex)
//----------------------------------------------------------------------------------------------------------------------
{
	al_lock_mutex(thisMutex);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Unlock a mutex
void PARA_unlockMutex(PARA_MUTEX *thisMutex)
//----------------------------------------------------------------------------------------------------------------------
{
	al_unlock_mutex(thisMutex);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy a mutex
void PARA_destroyMutex(PARA_MUTEX *thisMutex)
//----------------------------------------------------------------------------------------------------------------------
{
	al_destroy_mutex(thisMutex);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Rest for the passed in milliseconds
void PARA_rest(double ms)
//----------------------------------------------------------------------------------------------------------------------
{
	al_rest(ms);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Wrap around creating a thread
PARA_THREAD *PARA_createThread(functionPtr threadFunction, const std::string threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_THREAD     *tempThread;

	tempThread = al_create_thread(reinterpret_cast<void *(*)(ALLEGRO_THREAD *, void *)>(threadFunction), nullptr);
	if (tempThread == nullptr)
		log_logMessage(LOG_LEVEL_EXIT, sys_getString("Unable to create thread [ %s ]", threadName.c_str()));

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Created thread [ %s ]", threadName.c_str()));
	al_start_thread(tempThread);

	return tempThread;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Wrap around creating a mutex
PARA_MUTEX *PARA_createMutex(const std::string mutexName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_MUTEX      *tempMutex;

	tempMutex = al_create_mutex();
	if (tempMutex == nullptr)
		log_logMessage(LOG_LEVEL_EXIT, sys_getString("Unable to create mutex [ %s ]", mutexName.c_str()));

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Created mutex [ %s ]", mutexName.c_str()));

	return tempMutex;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Present a finished frame using the graphical library
void PARA_presentFrame(PARA_DISPLAY *thisDisplay, PARA_BITMAP *thisBitmap)
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Now display backing bitmap onto display
	al_set_target_backbuffer(thisDisplay);
	al_clear_to_color(al_map_rgb_f(1, 1, 0));
	al_draw_scaled_bitmap(thisBitmap, 0, 0, screenWidth, screenHeight, scaleX, scaleY, scaleW, scaleH, 0);

	al_flip_display();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Prepare the frame for rendering
void PARA_prepareFrame(ALLEGRO_BITMAP *thisBitmap)
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Set target bitmap to receive all drawing
	al_set_target_bitmap(thisBitmap);
	al_clear_to_color(al_map_rgb_f(1, 1, 1));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get current time
double PARA_getTime()
//----------------------------------------------------------------------------------------------------------------------
{
	return al_get_time();
}