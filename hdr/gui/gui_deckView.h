#pragma once

#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_resources.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_init.h>
#include "system/sys_main.h"

// Draw the current level onto the entire screen
void gui_showCurrentLevel ();

// Get the deckview bitmap
PARA_BITMAP *gam_getDeckViewBitmap();
