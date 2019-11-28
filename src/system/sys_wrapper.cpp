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
// Rest for the passed in seconds
void PARA_rest(double seconds)
//----------------------------------------------------------------------------------------------------------------------
{
	al_rest(seconds);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Wrap around creating a thread
PARA_THREAD *PARA_createThread(functionPtr threadFunction, const std::string threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_THREAD     *tempThread;

	al_run_detached_thread(reinterpret_cast<void *(*)(void *)>(threadFunction), nullptr);
//	if (tempThread == nullptr)
//		log_logMessage(LOG_LEVEL_EXIT, sys_getString("Unable to create thread [ %s ]", threadName.c_str()));

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Created thread [ %s ]", threadName.c_str()));
//	al_start_thread(tempThread);

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
	al_draw_scaled_bitmap(thisBitmap, 0, 0, al_get_bitmap_width(thisBitmap), al_get_bitmap_height(thisBitmap), scaleX, scaleY, scaleW, scaleH, 0);

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
	al_clear_to_color(al_map_rgb_f(0, 0, 0));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get current time
double PARA_getTime()
//----------------------------------------------------------------------------------------------------------------------
{
	return al_get_time();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return a file pointer to a block of memory to be treated like a file
PARA_MEM_FILE *PARA_openMemFile(char *memoryPointer, int memorySize)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_MEM_FILE   *filePointer;

	filePointer = al_open_memfile(memoryPointer, memorySize, "r");

	return filePointer;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Wrap the library file read routine
void PARA_readFile(PARA_MEM_FILE *filePointer, void *memoryPtr, size_t readSize)
//----------------------------------------------------------------------------------------------------------------------
{
	size_t      returnValue;

	returnValue = al_fread(filePointer, memoryPtr, readSize);
	if (returnValue == 0)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Failed in reading file."));
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Close a memory file
void PARA_closeFile(PARA_MEM_FILE *filePointer)
//----------------------------------------------------------------------------------------------------------------------
{
	al_fclose(filePointer);
}