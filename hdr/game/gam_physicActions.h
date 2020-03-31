#pragma once

#include "system/sys_main.h"

enum physicEventType
{
	PHYSIC_EVENT_TYPE_DROID = 0,
	PHYSIC_EVENT_TYPE_BULLET,
	PHYSIC_EVENT_TYPE_PARTICLE,
	PHYSIC_EVENT_TYPE_PLAYER,
	PHYSIC_EVENT_TYPE_NEW_BULLET,
	PHYSIC_EVENT_TYPE_REMOVE_BULLET,
	PHYSIC_EVENT_TYPE_COLLIDE
};

enum physicDamageType
{
	PHYSIC_DAMAGE_BULLET = 0,
	PHYSIC_DAMAGE_EXPLOSION,
	PHYSIC_DAMAGE_BUMP
};

enum physicActionType
{
	PHYSIC_ACTION_REMOVE = 0,
	PHYSIC_ACTION_NEW_PARTICLE
};

typedef struct
{
	int eventType = -1;
	int damageType = -1;
	int eventAction = 1;
	int targetIndex = -1;
	int sourceIndex = -1;
	b2Vec2 worldLocation = {0, 0};
} __physicAction;

// Clear the queue between game restart in case there are unprocessed events from previous run
void gam_initPhysicsActionQueue();

// Add an event to the queue to be processed out of Physic world step - this function run from main thread
void gam_addPhysicAction(int eventType, int damageType, int eventAction, int targetIndex, int sourceIndex, b2Vec2 worldLocation);

// Process the physic action queue
void gam_processPhysicActions();