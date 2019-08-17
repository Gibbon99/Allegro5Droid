#pragma once

#include "system/sys_main.h"

extern float velocity;
extern float prevCirclePosX, prevCirclePosY;
extern float circlePosX;
extern float circlePosY;

// Run a frame once
void sys_gameTickRun();

