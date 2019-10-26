
//-----------------------------------------------------------------------------
//
// Setup the values for the particles
void as_initParticleValues()
//-----------------------------------------------------------------------------
{
	particleVelocityExplosion = 35;
	particleVelocityExplosionAdd = 25;

	particleVelocitySpark = 20;
	particleVelocitySparkAdd = 25;

	particleNumberExplosion = 40;
	particleNumberExplosionAdd = 20;

	particleNumberTrail = 10;
	particleNumberTrailAdd = 5;
	particleTrailAlphaReset = 1;
	particleTrailAlphaResetAdd = 1;

	particleNumberSpark = 3;
	particleNumberSparkAdd = 5;

	particleExplosionReduce = 2.30f;
	particleSparkReduce = 6.2f;
	particleTrailReduce = 15.20f;

	particleTrailLimit = 5;		// How many particles to spawn each frame

	as_setParticleColor ( PARTICLE_TYPE_SPARK, 0.5f, 0.5f, 0.5f, 1.0f );
	as_setParticleColor ( PARTICLE_TYPE_EXPLOSION, 1.0f, 0.0f, 0.0f, 1.0f );
	as_setParticleColor ( PARTICLE_TYPE_TRAIL, 0.5f, 0.2f, 0.2f, 1.0f );

	particleMass = 0.05f;
	particleFriction = 0.2f;
	particleElastic = 0.9f;
	particleSize = 2.0f;

	particleRenderType = PARTICLE_RENDER_PRIMITIVE;
	particleBlendSrc = 2;
	particleBlendDst = 3;

  }