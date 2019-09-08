#pragma once

#include "system/sys_main.h"

typedef struct
{
	bool           inUse;
	int            inUseBy;
	float          width;
	float          height;
	b2Vec2         worldPosition = {0, 0};
	b2BodyDef      bodyDef;                      // Used for physics and collisions
	b2PolygonShape shape;
	b2FixtureDef   fixtureDef;
	b2Body         *body;
	_userData      *userData;
} __healingSensor;

extern float healingAnimSpeed;

// Find out where on the level the healing tiles are
// Remember for this level for animation
void gam_findHealingTiles (std::string levelName);

// Animate healing tiles on current level
void gam_animateHealing (const std::string levelName, float tickTime);

// Render current door frames onto map
void gam_renderHealingFrames (const std::string &levelName);

// Free memory and clear healing array
void gam_clearHealing ();

// Start this healing tile - pass in droid on tile and healing index. whichDroid == -1 is player
void gam_startHealing(int whichDroid, int whichTile);

// Stop this healing tile
void gam_stopHealing(int whichTile);

// Check status of healing tiles
void gam_processHealingTiles();

// Create healing sensors for this level
void gam_createHealingSensors(const std::string& levelName);
