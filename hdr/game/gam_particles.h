#pragma once

#include "system/sys_main.h"
#include <list>
#include <hdr/system/sys_physics.h>

typedef struct {
	bool          isAlive = false;
	ALLEGRO_COLOR color;
	b2Vec2        direction;
	b2Vec2        worldPos;
	_physicObject physicObject;
} _particle;

typedef struct {
	bool                   inUse = false;
	b2Vec2                 worldPos;
	unsigned int           numParticlesAlive;
	unsigned int           type;
	int                    bulletLink;
	std::string            image;
	bool                   usePhysics;
	bool                   useDirections;
	std::vector<_particle> particle;
} _emitter;

extern std::vector<_emitter> emitter;
//extern std::list<_particle>::iterator itr;
extern int                 particleRenderType;

//
//------------ These are all set from the particle script
//
extern int   particleVelocityExplosion;
extern int   particleVelocityExplosionAdd;
extern int   particleVelocitySpark;
extern int   particleVelocitySparkAdd;
extern int   particleNumberExplosion;
extern int   particleNumberExplosionAdd;
extern int   particleNumberTrail;
extern int   particleNumberTrailAdd;
extern int   particleTrailAlphaReset;
extern int   particleTrailAlphaResetAdd;
extern int   particleNumberSpark;
extern int   particleNumberSparkAdd;
extern float particleExplosionReduce;
extern float particleSparkReduce;
extern float particleTrailReduce;
extern int   particleTrailLimit;

extern float particleMass; //   0.05f
extern float particleFriction; //  0.2f
extern float particleElastic; //  0.9f
extern float particleSize; //  2.0f

extern int particleBlendSrc;
extern int particleBlendDst;

extern double profileParticleRenderStart;
extern double profileParticleRenderEnd;
extern double profileParticleThinkStart;
extern double profileParticleThinkEnd;
extern bool   profileParticles; // Set from startup script

extern ALLEGRO_COLOR particleSparkColor;
extern ALLEGRO_COLOR particleExplosionColor;
extern ALLEGRO_COLOR particleTrailColor;

// Start an emitter at the passed in location
void par_initEmitter (b2Vec2 worldPos, int type, int whichBullet);

// Draw all emitters and particles
void par_renderParticles ();

// Move the particles
void par_animateParticles (float tickTime);

// Remove an emitter attached to a bullet
void par_removeEmitter (int whichEmitter);

// Debug the particle emitters
void par_debugParticles ();

// Set the particle colors from the script
void par_hostSetParticleColor (int whichParticle, float red, float green, float blue, float alpha);

// Process emitter queue - done outside of physics stepWorld
void par_processEmitterQueue ();

// Add an emitter to the queue for processing
void par_addEmitter (b2Vec2 worldPos, int type, int whichBullet);

// Reset all emitters ready for new game to start
void par_resetAllEmitters ();


