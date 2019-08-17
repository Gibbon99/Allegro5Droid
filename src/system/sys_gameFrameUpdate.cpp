#include "hdr/system/sys_gameFrameUpdate.h"

float prevCirclePosX, prevCirclePosY;
float circlePosX = 50.0f;
float circlePosY = 100.0f;
float velocity   = -10.0f;

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun()
//----------------------------------------------------------------------------------------------------------------------
{
	if (velocity < -5.0f)
	{
		circlePosX = 50.0f;
		circlePosY = 100.0f;
		velocity   = 4.00f;
	}

	prevCirclePosX = circlePosX;
	prevCirclePosY = circlePosY;

	circlePosX += velocity;
	circlePosY     = 100.0f;

	if (circlePosX > windowWidth - 128)
	{
		velocity = -4.50f;
	}

	if (circlePosX < 1)
	{
		velocity = 4.50f;
	}
}
