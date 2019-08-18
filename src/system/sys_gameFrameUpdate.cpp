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
	switch (currentMode)
	{
		case MODE_CONSOLE:
			break;

		case MODE_GAME:
			if (velocity < -5.0f)
			{
				circlePosX = 50.0f;
				circlePosY = 100.0f;
				velocity   = 1.00f;
			}

			prevCirclePosX = circlePosX;
			prevCirclePosY = circlePosY;

			circlePosX += velocity;
			circlePosY     = 100.0f;

			if ((int)circlePosX > screenWidth - 32)
			{
				velocity = -1.0f;
			}

			if (circlePosX < 1.0f)
			{
				velocity = 1.0f;
			}

			break;

		default:
			break;
	}
}
