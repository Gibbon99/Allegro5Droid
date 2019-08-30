#pragma once

#include "system/sys_main.h"

//-----------------------------------------------------------------------------
//
// defines used for controlling doors - tile number
//-----------------------------------------------------------------------------
#define DOOR_ACROSS               14
#define DOOR_ACROSS_OPEN_1        15
#define DOOR_ACROSS_OPEN_2        16
#define DOOR_ACROSS_OPENED        NO_PASS_TILE + 10
#define DOOR_ACROSS_CLOSING_1     22
#define DOOR_ACROSS_CLOSING_2     21
#define DOOR_ACROSS_CLOSED        14

#define DOOR_UP                   11
#define DOOR_UP_OPEN_1            12
#define DOOR_UP_OPEN_2            13
#define DOOR_UP_OPENED            NO_PASS_TILE + 9
#define DOOR_UP_CLOSING_1         19
#define DOOR_UP_CLOSING_2         18
#define DOOR_UP_CLOSED            11

#define MAX_NUM_OF_DOORS        21
