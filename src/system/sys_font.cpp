#include "hdr/system/sys_font.h"

ALLEGRO_FONT    *systemFont;

//----------------------------------------------------------------------------------------------------------------------
//
// Init the builtin system font
bool fnt_initSystemFont()
//----------------------------------------------------------------------------------------------------------------------
{
	systemFont = al_create_builtin_font();
	if (nullptr == systemFont)
	{
		quitProgram = true;
		al_show_native_message_box(nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create system font.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clean up the system font
void fnt_destroySystemFont()
//----------------------------------------------------------------------------------------------------------------------
{
	al_destroy_font(systemFont);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a line of text in systemfont
void fnt_printSystemFont(float posX, float posY, std::string textLine)
//----------------------------------------------------------------------------------------------------------------------
{
	al_draw_text(systemFont, al_map_rgb_f(0, 0, 0), posX, posY, 0, textLine.c_str());
}