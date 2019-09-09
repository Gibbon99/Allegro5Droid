#include <map>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/gui/gui_sideView.h>
#include <hdr/io/io_resourceImage.h>
#include "hdr/io/io_logFile.h"
#include "hdr/io/io_resources.h"

std::map<std::string, __bitmap> bitmap;
std::map<std::string, __audio>  audio;
std::map<std::string, __font>   fonts;
std::map<std::string, __level>  levels;
//std::map<std::string, __highScore> highScore;
std::map<std::string, __sprite> sprites;

//---------------------------------------------------------------------------------------------------------------------------
//
// Load an Audio file
void sys_loadResourceAudio (const std::string &key, const std::string &fileName)
//---------------------------------------------------------------------------------------------------------------------------
{
	__audio tempAudio;

	tempAudio.common.fileName = fileName;
	tempAudio.audio           = al_load_sample (fileName.c_str ());
	if (nullptr == tempAudio.audio)
	{
		tempAudio.common.loaded = false;
		audio.insert (std::pair<std::string, __audio> (key, tempAudio));
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Failed to load [ %s ]", fileName.c_str ()));
		return;
	}
	tempAudio.common.loaded   = true;
	audio.insert (std::pair<std::string, __audio> (key, tempAudio));

	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Loaded [ %s ]", fileName.c_str ()));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Load a TTF file and generate font based on size
void sys_loadResourceFont (const std::string &key, const std::string &fileName, int size)
//---------------------------------------------------------------------------------------------------------------------------
{
	__font tempFont;

	tempFont.common.fileName = fileName;
	tempFont.size            = size;
	tempFont.font            = al_load_ttf_font (fileName.c_str (), size, 0);
	if (nullptr == tempFont.font)
	{
		tempFont.common.loaded = false;
		fonts.insert (std::pair<std::string, __font> (key, tempFont));
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Failed to load TTF [ %s ]", fileName.c_str ()));
		return;
	}
	tempFont.common.loaded   = true;
	fonts.insert (std::pair<std::string, __font> (key, tempFont));
	log_logMessage(LOG_LEVEL_INFO, sys_getString("TTF loaded [ %s ]", fileName.c_str()));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Called from script to load a resource
void sys_loadResource (const std::string &key, const std::string &fileName, int type, int numFrames, int size)
//---------------------------------------------------------------------------------------------------------------------------
{
	switch (type)
	{
		case RESOURCE_BITMAP:
			sys_loadResourceBitmap (key, fileName);
			break;

		case RESOURCE_AUDIO:
			sys_loadResourceAudio (key, fileName);
			break;

		case RESOURCE_FONT:
			sys_loadResourceFont (key, fileName, size);
			break;

		case RESOURCE_LEVEL:
			break;

		case RESOURCE_SHIP_VIEW:
			gui_loadSideViewData (fileName);
			break;

		case RESOURCE_HIGH_SCORE:
			break;

		case RESOURCE_SPRITE:
			io_loadResourceSprite (key, fileName, numFrames);
			break;

		case RESOURCE_TILES:
			break;

		default:
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Unknown resource type [ %i ]", type));
			break;
	}
}
