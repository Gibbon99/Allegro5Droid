#pragma once

#include <system/sys_main.h>

typedef int (*functionPtr)( ... );


#define PARA_MUTEX      ALLEGRO_MUTEX
#define PARA_THREAD     ALLEGRO_THREAD
#define PARA_BITMAP     ALLEGRO_BITMAP
#define PARA_DISPLAY    ALLEGRO_DISPLAY
#define PARA_MEM_FILE   ALLEGRO_FILE
#define PARA_COLOR      ALLEGRO_COLOR

// Lock a mutex
void PARA_lockMutex(PARA_MUTEX *thisMutex);

// Unlock a mutex
void PARA_unlockMutex(PARA_MUTEX *thisMutex);

// Destroy a mutex
void PARA_destroyMutex(PARA_MUTEX *thisMutex);

// Rest for the passed in milliseconds
void PARA_rest(double seconds);

// Wrap around creating a thread
PARA_THREAD *PARA_createThread(functionPtr threadFunction, const std::string threadName);

// Wrap around creating a mutex
PARA_MUTEX *PARA_createMutex(const std::string mutexName);

// Prepare the frame for rendering
void PARA_prepareFrame(ALLEGRO_BITMAP *thisBitmap);

// Present a finished frame using the graphical library
void PARA_presentFrame(PARA_DISPLAY *thisDisplay, PARA_BITMAP *thisBitmap);

// Get current time
double PARA_getTime();

// Return a file pointer to a block of memory to be treated like a file
PARA_MEM_FILE *para_openMemFile(char *memoryPointer, int memorySize);

// Wrap the library file read routine
void para_readFile(PARA_MEM_FILE *filePointer, void *memoryPtr, size_t readSize);

// Close a memory file
void para_closeFile(PARA_MEM_FILE *filePointer);
