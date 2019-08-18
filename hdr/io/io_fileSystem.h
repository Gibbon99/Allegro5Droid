#pragma once

#include "system/sys_main.h"
#include <physfs.h>

// Find the names of all the files in the scripts directory and store ready for loading
void io_getScriptFileNames();

// Check if a file exists
bool io_doesFileExist(std::string fileName);

// Load a file into a pointer
int io_getFileIntoMemory ( const char *fileName, void *results );

// Get the length of a file
PHYSFS_sint64 io_getFileSize ( const char *fileName );

// Start the packfile system
bool io_startFileSystem();

// Return the state of the file system
bool io_isFileSystemReady();

