#pragma once

#include <map>
#include "system/sys_main.h"

enum drawModes
{
	RENDER_FULLSCREEN = 0,
	RENDER_SOURCE
};


typedef struct
{
	bool        loaded;
	std::string fileName;
} __resourceCommon;

typedef struct
{
	ALLEGRO_BITMAP   *bitmap;
	std::string      fileName;
	int              width;
	int              height;
	__resourceCommon common;
} __bitmap;

typedef struct
{
	ALLEGRO_SAMPLE   *audio;
	__resourceCommon  common;
} __audio;

typedef struct
{
	ALLEGRO_FONT     *font;
	int              size;
	__resourceCommon common;
} __font;

typedef struct
{
	int *memLevel;
} __level;

typedef struct
{
	ALLEGRO_BITMAP   *bitmap;
	int              numFrames;
	int              frameWidth;
	int              frameHeight;
	__resourceCommon common;
} __sprite;

extern std::map<std::string, __bitmap> bitmap;
extern std::map<std::string, __audio>  audio;
extern std::map<std::string, __font>   fonts;
extern std::map<std::string, __level>  levels;
//extern std::map<std::string, __highScore> highScore;
extern std::map<std::string, __sprite> sprites;
//extern std::map<std::string, __tiles> tiles;

// Called from script to load a resource
void sys_loadResource(std::string key, std::string fileName, int type, int numFrames, int size);

// Draw a bitmap
void sys_drawBitmap (std::string keyName, float posX, float posY, int drawMode);