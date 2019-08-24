#include <hdr/system/sys_gameEvents.h>
#include <hdr/system/sys_font.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/io/io_console.h>
#include "hdr/system/sys_shutdown.h"
#include "hdr/system/sys_scriptEngine.h"
#include "hdr/system/sys_physics.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Exit game and return to system
void sys_shutdownToSystem()
//----------------------------------------------------------------------------------------------------------------------
{
	evt_stopThreads();

	sys_freePhysicsEngine();
	fnt_destroySystemFont();
	if (eventQueue != nullptr)
		al_destroy_event_queue(eventQueue);
	al_shutdown_image_addon();

	evt_destroyMutexes();
	al_uninstall_system();
	sys_stopScriptEngine();
	con_stopConsole();

	sys_freeMemory();
}
