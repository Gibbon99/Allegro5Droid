#include <physfs.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/system/sys_scriptEngine.h>
#include "hdr/io/io_fileSystem.h"

bool        fileSystemReady = false;

//----------------------------------------------------------------------------------------------------------------------
//
// Return the state of the file system
bool io_isFileSystemReady()
//----------------------------------------------------------------------------------------------------------------------
{
	return fileSystemReady;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get archive types supported by this version
bool io_getArchivers ()
//----------------------------------------------------------------------------------------------------------------------
{
	const PHYSFS_ArchiveInfo **rc = PHYSFS_supportedArchiveTypes();
	const PHYSFS_ArchiveInfo **i;

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Packfile: Supported archive types." ));

	if ( *rc == NULL )
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Packfile: Error: No archive types found." ));
		return false;
	}

	else
	{
		for ( i = rc; *i != NULL; i++ )
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString( "Packfile: [ %s ] : [ %s ]", ( *i )->extension, ( *i )->description ));
		}
	}

	return true;
}

// ---------------------------------------------------------------------------
//
// Start the packfile system
bool io_startFileSystem()
// ---------------------------------------------------------------------------
{
	PHYSFS_Version compiled;
	PHYSFS_Version linked;

	if ( PHYSFS_init ( NULL ) == 0 )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("Error: Filesystem failed to start - [ %s ]", PHYSFS_getErrorByCode ( PHYSFS_getLastErrorCode() ) ));
		fileSystemReady = false;
		return false;
	}

	PHYSFS_VERSION ( &compiled );
	PHYSFS_getLinkedVersion ( &linked );

	log_logMessage(LOG_LEVEL_INFO, sys_getString("Compiled against PhysFS version %d.%d.%d.", compiled.major, compiled.minor, compiled.patch ));
	log_logMessage(LOG_LEVEL_INFO, sys_getString("Linked against PhysFS version %d.%d.%d.",  linked.major, linked.minor, linked.patch ));

	//
	// Setup directory to write if needed
	if (0 == PHYSFS_setWriteDir("data"))
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Failed to set write path [ %s ]", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("The directory [ data ] holding all the data files is not present. Check the installation."));
		}
	//
	// Set base directory
	if ( 0 == PHYSFS_mount ( "data", "/", 1 ) )
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to set search path - data [ %s ]", PHYSFS_getErrorByCode ( PHYSFS_getLastErrorCode() ) ));
		fileSystemReady = false;
//		return false;
	}
	//
	// Add directory for loading scripts - move to archive file
	if ( 0 == PHYSFS_mount ( "data//scripts","/", 1 ) )
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to set search path - scripts [ %s ]", PHYSFS_getErrorByCode ( PHYSFS_getLastErrorCode() ) ));
		fileSystemReady = false;
//		return false;
	}
	//
	// Add archive file
	if ( 0 == PHYSFS_mount ( "data//data.zip", "/", 1 ) )
	{
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Failed to set search path - data.zip - [ %s ]", PHYSFS_getErrorByCode ( PHYSFS_getLastErrorCode() ) ));
		fileSystemReady = false;
//		return false;
	}
	//
	// What compression types are available
	io_getArchivers();

	fileSystemReady = true;

	return true;
}


//-----------------------------------------------------------------------------
//
// Get the length of a file
PHYSFS_sint64 io_getFileSize ( const char *fileName )
//-----------------------------------------------------------------------------
{
	PHYSFS_file		*compFile;
	PHYSFS_sint64	fileLength;

	if (!fileSystemReady)
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("PHYSFS system has not been initialised. Can't process [ %s ].", fileName ));
		return -1;
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead ( fileName );

	if ( nullptr == compFile )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("Filesystem can't open file [ %s ] - [ %s ].", fileName, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return -1;
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength ( compFile );

	if ( -1 == fileLength )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("Unable to determine file length for [ %s ] - [ %s ].", fileName, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return -1;
	}

	//
	// Finished with it now so close it
	PHYSFS_close ( compFile );

	return fileLength;
}
// ---------------------------------------------------------------------------
//
// Load a file into a pointer
int io_getFileIntoMemory ( const char *fileName, void *results )
// ---------------------------------------------------------------------------
{
	PHYSFS_file		*compFile = nullptr;
	PHYSFS_sint64	fileLength;

	if ( !fileSystemReady )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("PHYSFS system has not been initialised. Can't load [ %s ].", fileName ));
		return -1;
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead ( fileName );

	if ( nullptr == compFile )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("Filesystem can't open file [ %s ] - [ %s ].", fileName, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return -1;
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength ( compFile );

	if ( -1 == fileLength )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("Unable to determine file length for [ %s ] - [ %s ].", fileName, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		PHYSFS_close (compFile);
		return -1;
	}

	//
	// Read contents of file into the pointer
	PHYSFS_sint64 returnCode = PHYSFS_readBytes ( compFile, ( void * ) results, fileLength);

	if ( -1 == returnCode )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("Filesystem read failed - [ %s ] for [ %s ].", PHYSFS_getErrorByCode ( PHYSFS_getLastErrorCode() ), fileName ));
		PHYSFS_close (compFile);
		return -1;
	}

	//
	// Finished with it now so close it
	PHYSFS_close ( compFile );

	return 1;
}

// ---------------------------------------------------------------------------
//
// Check if a file exists
bool io_doesFileExist(std::string fileName)
// ---------------------------------------------------------------------------
{
	if ( !fileSystemReady )
	{
		evt_pushEvent(0, PARA_EVENT_LOGFILE, LOGFILE_EVENT_LOG, LOG_LEVEL_INFO, 0, sys_getString("File system not ready. Can not check for file [ %s ]", fileName.c_str()));
		return false;
	}

	return static_cast<bool>(PHYSFS_exists( fileName.c_str()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the names of all the files in the scripts directory and store ready for loading
void io_getScriptFileNames()
//----------------------------------------------------------------------------------------------------------------------
{
	std::string tempFileName;

	char **rc = PHYSFS_enumerateFiles("scripts");
	char **i;
	//
	// Store the file names into an array
	for (i = rc; *i != nullptr; i++)
	{
		tempFileName = *i;
		scriptFileCache.push_back(tempFileName);

		log_logMessage(LOG_LEVEL_INFO, sys_getString("Script [ %s ]", tempFileName.c_str()));
	}

	PHYSFS_freeList(rc);
}