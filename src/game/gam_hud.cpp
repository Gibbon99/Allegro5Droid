#include <hdr/io/io_resources.h>
#include "hdr/game/gam_hud.h"

//---------------------------------------------------------------------------------
//
// Render the HUD
void hud_renderHUD()
//---------------------------------------------------------------------------------
{
	sys_drawBitmap ("hud", 0.0f, 0.0f, RENDER_SOURCE);
}