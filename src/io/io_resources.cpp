#include <map>
#include "hdr/io/io_logFile.h"
#include "hdr/io/io_resources.h"

std::map<std::string, __bitmap> bitmap;
std::map<std::string, __audio>  audio;
std::map<std::string, __font>   fonts;
std::map<std::string, __level>  levels;
//std::map<std::string, __highScore> highScore;
std::map<std::string, __sprite> sprites;
//std::map<std::string, __tiles> tiles;

//---------------------------------------------------------------------------------------------------------------------------
//
// Load a bitmap file
void sys_loadResourceBitmap(std::string key, std::string fileName)
//---------------------------------------------------------------------------------------------------------------------------
{
	__bitmap tempBitmap;

	tempBitmap.common.fileName = fileName;
	tempBitmap.bitmap          = al_load_bitmap(fileName.c_str());
	if (nullptr == tempBitmap.bitmap)
	{
		tempBitmap.common.loaded = false;
		bitmap.insert(std::pair<std::string, __bitmap>(key, tempBitmap));
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Failed to load [ %s ]", fileName.c_str()));
		return;
	}

	tempBitmap.width         = al_get_bitmap_width(tempBitmap.bitmap);
	tempBitmap.height        = al_get_bitmap_height(tempBitmap.bitmap);
	tempBitmap.common.loaded = true;
	bitmap.insert(std::pair<std::string, __bitmap>(key, tempBitmap));

	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Loaded [ %s ]", fileName.c_str()));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Load an Audio file
void sys_loadResourceAudio(std::string key, std::string fileName)
//---------------------------------------------------------------------------------------------------------------------------
{
	__audio tempAudio;

	tempAudio.common.fileName = fileName;
	tempAudio.audio           = al_load_sample(fileName.c_str());
	if (nullptr == tempAudio.audio)
	{
		tempAudio.common.loaded = false;
		audio.insert(std::pair<std::string, __audio>(key, tempAudio));
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Failed to load [ %s ]", fileName.c_str()));
		return;
	}
	tempAudio.common.loaded   = true;
	audio.insert(std::pair<std::string, __audio>(key, tempAudio));

	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Loaded [ %s ]", fileName.c_str()));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Load a TTF file and generate font based on size
void sys_loadResourceFont(std::string key, std::string fileName, int size)
//---------------------------------------------------------------------------------------------------------------------------
{
	__font tempFont;

	tempFont.common.fileName = fileName;
	tempFont.size            = size;
	tempFont.font            = al_load_ttf_font(fileName.c_str(), size, 0);
	if (nullptr == tempFont.font)
	{
		tempFont.common.loaded = false;
		fonts.insert(std::pair<std::string, __font>(key, tempFont));
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Failed to load TTF [ %s ]", fileName.c_str()));
		return;
	}
	tempFont.common.loaded   = true;
	fonts.insert(std::pair<std::string, __font>(key, tempFont));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Load a sprite
void sys_loadResourceSprite(std::string key, std::string fileName, int numFrames)
//---------------------------------------------------------------------------------------------------------------------------
{
	__sprite tempSprite;

	tempSprite.common.fileName = fileName;
	tempSprite.numFrames       = numFrames;
	tempSprite.bitmap          = al_load_bitmap(fileName.c_str());

	if (nullptr == tempSprite.bitmap)
	{
		tempSprite.common.loaded = false;
		sprites.insert(std::pair<std::string, __sprite>(key, tempSprite));
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Failed to load sprite [ %s ]", fileName.c_str()));
		return;
	}

	tempSprite.frameWidth  = al_get_bitmap_width(tempSprite.bitmap) / numFrames;
	tempSprite.frameHeight = al_get_bitmap_height(tempSprite.bitmap);
	sprites.insert(std::pair<std::string, __sprite>(key, tempSprite));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Called from script to load a resource
void sys_loadResource(std::string key, std::string fileName, int type, int numFrames, int size)
//---------------------------------------------------------------------------------------------------------------------------
{
	switch (type)
	{
		case RESOURCE_BITMAP:
			sys_loadResourceBitmap(key, fileName);
			break;

		case RESOURCE_AUDIO:
			sys_loadResourceAudio(key, fileName);
			break;

		case RESOURCE_FONT:
			sys_loadResourceFont(key, fileName, size);
			break;

		case RESOURCE_LEVEL:
			break;

		case RESOURCE_SHIP_VIEW:
			break;

		case RESOURCE_HIGH_SCORE:
			break;

		case RESOURCE_SPRITE:
			sys_loadResourceSprite(key, fileName, numFrames);
			break;

		case RESOURCE_TILES:
			break;

		default:
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Unknown resource type [ %i ]", type));
			break;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Draw a bitmap
void sys_drawBitmap (std::string keyName, float posX, float posY, int drawMode)
//---------------------------------------------------------------------------------------------------------------------------
{
	switch (drawMode)
		{
			case RENDER_FULLSCREEN:
				al_draw_scaled_bitmap(bitmap.at(keyName).bitmap, 0, 0, bitmap.at (keyName).width, bitmap.at(keyName).height,
				                      0, 0, screenWidth, screenHeight, 0);
	break;

			case RENDER_SOURCE:
				al_draw_bitmap(bitmap.at(keyName).bitmap, 0.0f, 0.0f, 0);
				return;
		}

//	al_draw_bitmap (bitmap.at(keyName).bitmap, 0, 0, 0);
}
//---------------------------------------------------------------------------------------------------------------------------
//
// Play an Audio file
void snd_playSound(std::string key, ALLEGRO_PLAYMODE loop, float gain, float pan)
//---------------------------------------------------------------------------------------------------------------------------
{
	bool              playResult;
	ALLEGRO_SAMPLE_ID *sampleID = nullptr;

	try
	{
		playResult = al_play_sample(audio.at(key).audio, gain, pan, 1.0, loop, sampleID);

		if (!playResult)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Sample [ %s ] failed to play", key.c_str()));
			return;
		}
	}

	catch (const std::out_of_range &oor)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Audio [ %s ] not found – [ %s ]", key.c_str(), oor.what()));
		return;
	}

//
// Store the playing sample ID with the key – is it used by anything?
}