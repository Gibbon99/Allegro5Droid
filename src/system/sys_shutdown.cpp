#include <hdr/system/sys_gameEvents.h>
#include <hdr/system/sys_font.h>
#include "hdr/system/sys_shutdown.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Exit game and return to system
void sys_shutdownToSystem()
//----------------------------------------------------------------------------------------------------------------------
{
	fnt_destroySystemFont();
	al_destroy_event_queue(eventQueue);
	al_uninstall_system();
}
