#pragma once

#include <map>
#include "system/sys_main.h"

enum drawModes {
	RENDER_FULLSCREEN = 0,
	RENDER_SOURCE,
	RENDER_SCALE
};

typedef struct {
	bool        loaded = false;
	std::string fileName = "";
} __resourceCommon;

typedef struct {
	ALLEGRO_BITMAP   *bitmap = nullptr;
	std::string      fileName = "";
	int              width;
	int              height;
	__resourceCommon common;
} __bitmap;

typedef struct {
	ALLEGRO_SAMPLE          *audio = nullptr;
	ALLEGRO_SAMPLE_INSTANCE *sampleInstance;
	ALLEGRO_SAMPLE_ID       sampleID;
	float                   sampleTimeLength;
	__resourceCommon        common;
} __audio;

typedef struct {
	ALLEGRO_FONT     *font;
	int              size;
	__resourceCommon common;
} __font;

typedef struct {
	int *memLevel;
} __level;

typedef struct {
	ALLEGRO_BITMAP   *bitmap;
	int              numFrames;
	int              frameWidth;
	int              frameHeight;
	int              renderOffsetX;
	int              renderOffsetY;
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
void sys_loadResource (const std::string &key, const std::string &fileName, int type, int numFrames, int size);