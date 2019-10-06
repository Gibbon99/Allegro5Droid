
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_bullet.h>
#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceImage.h>
#include "hdr/game/gam_particles.h"

#define DEBUG_PARTICLES 1

std::vector<_emitter> emitter;

int particleRenderType; // Set from start script

int numInitialParticleEmitters = 10;

int   particleVelocityExplosion;
int   particleVelocityExplosionAdd;
int   particleVelocitySpark;
int   particleVelocitySparkAdd;
int   particleNumberExplosion;
int   particleNumberExplosionAdd;
int   particleNumberTrail;
int   particleNumberTrailAdd;
int   particleTrailAlphaReset;
int   particleTrailAlphaResetAdd;
int   particleNumberSpark;
int   particleNumberSparkAdd;
float particleExplosionReduce;
float particleSparkReduce;
float particleTrailReduce;
int   particleTrailLimit;

float particleMass; //   0.05f
float particleFriction; //  0.2f
float particleElastic; //  0.9f
float particleSize; //  2.0f

double profileParticleRenderStart;
double profileParticleRenderEnd;
double profileParticleThinkStart;
double profileParticleThinkEnd;
bool   profileParticles; // Set from startup script

int particleBlendSrc;
int particleBlendDst;

ALLEGRO_COLOR particleSparkColor;
ALLEGRO_COLOR particleExplosionColor;
ALLEGRO_COLOR particleTrailColor;

typedef struct {
	b2Vec2 worldPos;
	int    type        = 0;
	int    whichBullet = 0;
}             _emitterQueue;

std::vector<_emitterQueue> emitterQueue;

//------------------------------------------------------------------
//
// Add an emitter to the queue for processing - worldPos is in physics coords
void par_addEmitter (b2Vec2 worldPos, int type, int whichBullet)
//------------------------------------------------------------------
{
	static bool   initDone = false;
	_emitterQueue tempQueueItem;

	if (!initDone)
		{
			emitter.reserve (numInitialParticleEmitters);
			for (auto &emitterItr : emitter)
				emitterItr.inUse = false;
		}

	tempQueueItem.worldPos    = worldPos;      // Physics coords passed for spark and explosion
	tempQueueItem.type        = type;
	tempQueueItem.whichBullet = whichBullet;

	emitterQueue.push_back (tempQueueItem);

#ifdef DEBUG_PARTICLES
	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Put new emitter request into queue - bullet [ %i ]", whichBullet));
#endif
}

//------------------------------------------------------------------
//
// Process emitter queue - done outside of physics stepWorld
void par_processEmitterQueue ()
//------------------------------------------------------------------
{
	if (emitterQueue.empty ())
		return;    // Nothing in queue to process

	for (auto &emitterItr : emitterQueue)
		{
			par_initEmitter (emitterItr.worldPos, emitterItr.type, emitterItr.whichBullet);

#ifdef DEBUG_PARTICLES
			log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Done initEmitter - bullet [ %i ]", emitterItr.whichBullet));
#endif

		}
	emitterQueue.clear ();
}

//------------------------------------------------------------------
//
// Set the particle colors from the script
void par_hostSetParticleColor (int whichParticle, float red, float green, float blue, float alpha)
//------------------------------------------------------------------
{
	switch (whichParticle)
		{
			case PARTICLE_TYPE_EXPLOSION:
				particleExplosionColor = al_map_rgba_f (red, green, blue, alpha);
			break;

			case PARTICLE_TYPE_SPARK:
				particleSparkColor = al_map_rgba_f (red, green, blue, alpha);
			break;

			case PARTICLE_TYPE_TRAIL:
				particleTrailColor = al_map_rgba_f (red, green, blue, alpha);
			break;

			default:
				log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unknown particle type passed from script [ %i ]", whichParticle));
			break;
		}
}

//------------------------------------------------------------------
//
// Debug the particle emitters
void par_debugParticles ()
//------------------------------------------------------------------
{
	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Emitter size [ %i ]", emitter.size ()));

	if (!emitter.empty ())
		{
			for (const auto &emitterItr : emitter)
				{
					log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Num particles [ %i ]", emitterItr.particle.size ()));
					log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Bullet link [ %i ]", emitterItr.bulletLink));
					log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Emitter type [ %i ]", emitterItr.type));
					log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("-----------------------"));
				}
		}
}

//------------------------------------------------------------------
//
// Get random angle in a circle shape
b2Vec2 par_getCircleAngle (int particleType)
//------------------------------------------------------------------
{
	float  angle, velocity;
	b2Vec2 direction;
	float  a = 2.0f * 3.1415f;

	angle = ((float) rand () / (float) (RAND_MAX)) * a;
	switch (particleType)
		{
			case PARTICLE_TYPE_EXPLOSION:
				velocity = sys_getCappedRandomNum (particleVelocityExplosion) + particleVelocityExplosionAdd;
			break;

			case PARTICLE_TYPE_SPARK:
				velocity = sys_getCappedRandomNum (particleVelocitySpark) + particleVelocitySparkAdd;
			break;
		}

	direction.x = (velocity * cos (angle)) * DEGTORAD;
	direction.y = (velocity * sin (angle)) * DEGTORAD;

	direction.Normalize ();
	return direction;
}

//----------------------------------------------------------------------------------------------------------
//
// Create a new bullet - pass back to calling function
//
// worldPos is passed in as physics coords
_emitter par_setupNewEmitter (b2Vec2 worldPos, int type, int whichBullet)
//----------------------------------------------------------------------------------------------------------
{
	_emitter  tempEmitter;
	_particle tempParticle;
	int       particleNumber;
	int       particleNumberAdd;
	b2Vec2    particlePosition;

	tempEmitter.type  = type;
	tempEmitter.inUse = true;

	switch (type)
		{
			case PARTICLE_TYPE_EXPLOSION:
				tempEmitter.bulletLink  = -1;
			tempEmitter.usePhysics    = true;
			tempEmitter.useDirections = true;
			particleNumber    = particleNumberExplosion;
			particleNumberAdd = particleNumberExplosionAdd;
			particlePosition  = worldPos;
			tempParticle.color            = particleExplosionColor;
			tempEmitter.numParticlesAlive = sys_getCappedRandomNum (particleNumber) + particleNumberAdd;
			break;

			case PARTICLE_TYPE_TRAIL:
				tempEmitter.usePhysics  = false;
			tempEmitter.useDirections = false;
			tempEmitter.bulletLink    = whichBullet;
			particleNumber    = particleNumberTrail;
			particleNumberAdd = particleNumberTrailAdd;
			particlePosition.x            = bullets[whichBullet].body->GetPosition ().x;
			particlePosition.y            = bullets[whichBullet].body->GetPosition ().y;
			tempParticle.color            = particleTrailColor;
			tempParticle.color.a          = 1.0f; //sys_getCappedRandomNum (120);
			tempEmitter.numParticlesAlive = sys_getCappedRandomNum (particleNumber) + particleNumberAdd;

			printf ("Start trail emitter [ %i ]\n", tempEmitter.numParticlesAlive);

			break;

			case PARTICLE_TYPE_SPARK:
				tempEmitter.usePhysics  = true;
			tempEmitter.useDirections = true;
			tempEmitter.bulletLink    = -1;
			particleNumber    = particleNumberSpark;
			particleNumberAdd = particleNumberSparkAdd;
			particlePosition  = worldPos;

			printf ("Spark for bullet [ %i ] initEmitter [ %f %f ]\n", whichBullet, worldPos.x, worldPos.y);

			tempParticle.color            = particleSparkColor;
			tempEmitter.numParticlesAlive = sys_getCappedRandomNum (particleNumber) + particleNumberAdd;
			break;
		}

	for (unsigned int i = 0; i != tempEmitter.numParticlesAlive; ++i)
		{
			tempParticle.worldPos = particlePosition;
			tempParticle.isAlive  = true;

			switch (tempEmitter.type)
				{
					case PARTICLE_TYPE_EXPLOSION:
						tempParticle.direction = par_getCircleAngle (PARTICLE_TYPE_EXPLOSION);
					break;

					case PARTICLE_TYPE_SPARK:
						tempParticle.direction = par_getCircleAngle (PARTICLE_TYPE_SPARK);
					break;

					case PARTICLE_TYPE_TRAIL:
						tempParticle.worldPos.x -= (6 - sys_getCappedRandomNum (12)) / pixelsPerMeter;
					tempParticle.worldPos.y -= (6 - sys_getCappedRandomNum (12)) / pixelsPerMeter;

					tempParticle.color.a = 0.0f;

					break;
				}

			if (tempEmitter.usePhysics)
				{
					tempParticle.physicObject.bodyDef.type = b2_dynamicBody;
					tempParticle.physicObject.bodyDef.position.Set (worldPos.x, worldPos.y);
					tempParticle.physicObject.bodyDef.angle = 0;
					tempParticle.physicObject.body          = sys_getPhysicsWorld ()->CreateBody (&tempParticle.physicObject.bodyDef);

					tempParticle.physicObject.userData            = new _userData;
					tempParticle.physicObject.userData->userType  = PHYSIC_TYPE_PARTICLE;
					tempParticle.physicObject.userData->dataValue = -1;
					tempParticle.physicObject.body->SetUserData (tempParticle.physicObject.userData);

					tempParticle.physicObject.shape.m_radius = (float) 2 / pixelsPerMeter;
					tempParticle.physicObject.shape.m_p.Set (0, 0);

					tempParticle.physicObject.fixtureDef.shape               = &tempParticle.physicObject.shape;
					tempParticle.physicObject.fixtureDef.density             = particleMass;
					tempParticle.physicObject.fixtureDef.friction            = particleFriction;
					tempParticle.physicObject.fixtureDef.restitution         = particleElastic;
					tempParticle.physicObject.fixtureDef.filter.categoryBits = PHYSIC_TYPE_PARTICLE;
					tempParticle.physicObject.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL;
					tempParticle.physicObject.body->CreateFixture (&tempParticle.physicObject.fixtureDef);
				}
			tempEmitter.particle.push_back (tempParticle);
		}

	return tempEmitter;
}
//------------------------------------------------------------------
//
// Start an emitter at the passed in location
void par_initEmitter (b2Vec2 worldPos, int type, int whichBullet)
//------------------------------------------------------------------
{

#ifdef DEBUG_PARTICLES
	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Adding new emitter - type [ %i ]", type));
#endif

	if (PARTICLE_RENDER_OFF == particleRenderType)
		return;

	for (int i = 0; i != emitter.size (); i++)
		{
			if (!emitter.at (i).inUse)
				{
					emitter.at (i) = par_setupNewEmitter (worldPos, type, whichBullet);
					return;
				}
		}
	emitter.push_back (par_setupNewEmitter (worldPos, type, whichBullet));
}

//-----------------------------------------------------
//
// Reset all emitters ready for new game to start
void par_resetAllEmitters ()
//-----------------------------------------------------
{
	if (PARTICLE_RENDER_OFF == particleRenderType)
		return;

#ifdef DEBUG_PARTICLES
	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Clearing out emitters"));
#endif

	for (auto &emitterItr : emitter)
		{
			if (emitterItr.usePhysics)
				{
					for (auto &particleItr : emitterItr.particle)
						{
							if (particleItr.physicObject.body != nullptr)
								{
									sys_getPhysicsWorld ()->DestroyBody (particleItr.physicObject.body);
									particleItr.physicObject.body = nullptr;
								}
						}
				}
			emitterItr.inUse = false;
		}
}

//-----------------------------------------------------
//
// Remove an emitter attached to a bullet
void par_removeEmitter (int whichBullet)
//-----------------------------------------------------
{
#ifdef DEBUG_PARTICLES
	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Removing bullet emitter - [ %i ]", whichBullet));
#endif

	int advanceCount = 0;

	if (PARTICLE_RENDER_OFF == particleRenderType)
		return;

	for (auto &emitterItr : emitter)
		{
			if (whichBullet == emitterItr.bulletLink)
				{
					for (auto &particleItr : emitterItr.particle)
						{
							if (emitterItr.usePhysics)
								{
									if (particleItr.physicObject.body != nullptr)
										{
											sys_getPhysicsWorld ()->DestroyBody (particleItr.physicObject.body);
											particleItr.physicObject.body = nullptr;
										}
								}
							particleItr.isAlive = false;
						}
					emitterItr.inUse = false;
					return;
				}
			advanceCount++;
		}
}

//-----------------------------------------------------
//
// Move the particles
void par_animateParticles (float tickTime)
//-----------------------------------------------------
{
	float particleReduce = 0.0f;
	int   particleIndex  = 0;
	int   emitterIndex   = 0;

	if (PARTICLE_RENDER_OFF == particleRenderType)
		return;

	if (profileParticles)
		profileParticleThinkStart = al_get_time ();

	emitterIndex  = 0;
	particleIndex = 0;

	for (auto &emitterItr  : emitter)
		{
			if (emitterItr.inUse)
				{
					switch (emitterItr.type)
						{
							case PARTICLE_TYPE_EXPLOSION:
								particleReduce = particleExplosionReduce;
							break;

							case PARTICLE_TYPE_TRAIL:
								particleReduce = sys_getCappedRandomNum (particleTrailReduce);
							break;

							case PARTICLE_TYPE_SPARK:
								particleReduce = particleSparkReduce;
							break;
						}

					particleIndex = 0;

					for (auto &particleItr : emitterItr.particle)
						{
							particleItr.color.a -= particleReduce * tickTime;

							if (emitterItr.type == PARTICLE_TYPE_TRAIL)
								{
									printf ("particle alpha [ %f ]\n", particleItr.color.a);
								}

							if (particleItr.color.a < 0.0f)
								{
									particleItr.isAlive = false;
									if (emitterItr.type == PARTICLE_TYPE_TRAIL)
										{
											particleItr.isAlive    = true;
											//
											// Change to pixel coords
											particleItr.worldPos.x = bullets[emitterItr.bulletLink].body->GetPosition ().x;
											particleItr.worldPos.y = bullets[emitterItr.bulletLink].body->GetPosition ().y;

											particleItr.worldPos.x -= (6 - sys_getCappedRandomNum (12)) / pixelsPerMeter;
											particleItr.worldPos.y -= (6 - sys_getCappedRandomNum (12)) / pixelsPerMeter;

											//
											// Put trail a little behind the bullet a little
											auto tempPos = bullets[emitterItr.bulletLink].velocity;
											tempPos.Normalize ();
											tempPos.operator*= (12.0f / pixelsPerMeter);
											particleItr.worldPos -= tempPos;

											particleItr.color.a = (float) sys_getCappedRandomNum (100) / 100.0f;
										}
								}
							else    // Animate / move the particles as they are still alive
								{
									switch (emitterItr.type)
										{
											case PARTICLE_TYPE_EXPLOSION:
											case PARTICLE_TYPE_SPARK:
												particleItr.physicObject.body->ApplyForce (particleItr.direction, particleItr.physicObject.body->GetWorldCenter (), true);
											break;

											case PARTICLE_TYPE_TRAIL:
												particleItr.color = al_map_rgba_f (particleTrailColor.r, particleTrailColor.g, particleTrailColor.b, particleItr.color.a);
											break;

										}
								}
							particleIndex++;
						}

					particleIndex = 0;

					for (auto &eraseItr : emitterItr.particle)
						{
							if (!eraseItr.isAlive)
								{
									if (eraseItr.physicObject.body != nullptr)
										{
											sys_getPhysicsWorld ()->DestroyBody (eraseItr.physicObject.body);
											eraseItr.physicObject.body = nullptr;
											eraseItr.isAlive           = false;
										}
								}
						}
				}   // End of inuse test
			emitterIndex++;
		}   // End of loop for each emitter

	if (profileParticles)
		profileParticleThinkEnd = al_get_time ();
}

//-----------------------------------------------------
//
// Draw all emitters and particles
void par_renderParticles ()
//-----------------------------------------------------
{
	b2Vec2 vecPos1;

	int op, src, dst;

	if (PARTICLE_RENDER_OFF == particleRenderType)
		return;

	if (profileParticles)
		profileParticleRenderStart = al_get_time ();

	if (PARTICLE_RENDER_BITMAP == particleRenderType)
		{
			if (useHoldBitmap)
				al_hold_bitmap_drawing (true);
		}

	al_get_blender (&op, &src, &dst);
	al_set_blender (ALLEGRO_ADD, particleBlendSrc, particleBlendDst);

	for (auto &emitterItr : emitter)
		{
			for (unsigned int i = 0; i != emitterItr.particle.size (); i++)
				{
					for (auto &particleItr : emitterItr.particle)
						{
							if (particleItr.isAlive)
								{
									switch (emitterItr.type)
										{
											case PARTICLE_TYPE_EXPLOSION:
											case PARTICLE_TYPE_SPARK:
												vecPos1 = particleItr.physicObject.body->GetPosition ();
											vecPos1.x *= pixelsPerMeter;       // Change to pixels from physics meters
											vecPos1.y *= pixelsPerMeter;
											break;

											case PARTICLE_TYPE_TRAIL:
												vecPos1 = particleItr.worldPos;     // In pixel world coordinates
											vecPos1.x *= pixelsPerMeter;       // Change to pixels from physics meters
											vecPos1.y *= pixelsPerMeter;
											break;
										}

									//
									// Translate from world to screen position
									vecPos1 = sys_worldToScreen (vecPos1, particleSize);

									switch (particleRenderType)
										{
											case PARTICLE_RENDER_BITMAP:
												al_draw_tinted_bitmap (bitmap["particleSpark"].bitmap, particleItr.color, vecPos1.x, vecPos1.y, 0);
											break;

											case PARTICLE_RENDER_PRIMITIVE:
												al_draw_filled_circle (vecPos1.x, vecPos1.y, particleSize, particleItr.color);
											break;
										}
								}
						}
				}
		}

	al_set_blender (op, src, dst);

	if (PARTICLE_RENDER_BITMAP == particleRenderType)
		{
			if (true == useHoldBitmap)
				al_hold_bitmap_drawing (false);
		}

	if (true == profileParticles)
		profileParticleRenderEnd = al_get_time ();
}
