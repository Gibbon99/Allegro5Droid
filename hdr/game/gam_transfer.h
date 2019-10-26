#pragma once

#include "system/sys_main.h"

// Change to transfer game mode
void gam_changeToTransferGame (int targetDroid);

// Return the target droid from a collision in transfer mode
int gam_getTransferTargetDroid();

// Handle the keyboard for choosing a side for transfer game
void trn_handleTransferChooseSide ();

// Called to setup the transfer cell values
void setupTransferCellValues();