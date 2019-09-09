#include <map>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resources.h>
#include "hdr/system/sys_font.h"

ALLEGRO_FONT *systemFont  = nullptr;
ALLEGRO_FONT *currentFont = nullptr;
PARA_COLOR currentFontColor;

bool useBuiltInFont = false;

//----------------------------------------------------------------------------------------------------------------------
//
// Init the builtin system font
bool fnt_initSystemFont ()
//----------------------------------------------------------------------------------------------------------------------
{
	systemFont = al_create_builtin_font ();
	if (nullptr == systemFont)
	{
		quitProgram = true;
		al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could not create system font.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clean up the system font
void fnt_destroySystemFont ()
//----------------------------------------------------------------------------------------------------------------------
{
	al_destroy_font (systemFont);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a line of text in systemfont
void fnt_printSystemFont (float posX, float posY, const std::string &textLine)
//----------------------------------------------------------------------------------------------------------------------
{
	al_draw_text (systemFont, currentFontColor, posX, posY, 0, textLine.c_str ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the current font to the passed in TTF
void fnt_setTTF (std::string fontName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (fonts.empty ())
	{
		useBuiltInFont = true;
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Unable to set font [ %s ] - no fonts loaded.", fontName.c_str ()));
		return;
	}
	auto ttrItr = fonts.find (fontName);

	if (ttrItr == fonts.end ())
	{
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Unable to set font [ %s ] - not found.", fontName.c_str ()));
		useBuiltInFont = true;
		return;
	}

	if (fonts.at (fontName).common.loaded)
	{
		currentFont    = fonts.at (fontName).font;
		useBuiltInFont = false;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the current font color - uses floats to get correct alpha colors
void fnt_setColor_f(float red, float green, float blue, float alpha)
//----------------------------------------------------------------------------------------------------------------------
{
	currentFontColor = al_map_rgba_f (red * alpha, green * alpha, blue * alpha, alpha);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a string using the TTF or builtin
void fnt_render (b2Vec2 renderPosition, const std::string &renderText)
//----------------------------------------------------------------------------------------------------------------------
{
	if (useBuiltInFont)
	{
		fnt_printSystemFont (renderPosition.x, renderPosition.y, renderText);
		return;
	}
	al_draw_text(currentFont, al_map_rgba_f(currentFontColor.r, currentFontColor.g, currentFontColor.b, currentFontColor.a), renderPosition.x, renderPosition.y, 0, renderText.c_str());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the width of a text string using the current font
float fnt_getWidth(const std::string& renderText)
//----------------------------------------------------------------------------------------------------------------------
{
	if (useBuiltInFont)
		return (float)al_get_text_width (systemFont, renderText.c_str());

	return (float)al_get_text_width (currentFont, renderText.c_str());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the height of a font
float fnt_getHeight()
//----------------------------------------------------------------------------------------------------------------------
{
	if (useBuiltInFont)
		return (float)al_get_font_line_height (systemFont);

	return (float)al_get_font_line_height (currentFont);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Free the fonts
void fnt_destroyFonts()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &fontItr : fonts)
	{
		if (fontItr.second.font != nullptr)
			al_destroy_font(fontItr.second.font);
	}
	fonts.clear();
}