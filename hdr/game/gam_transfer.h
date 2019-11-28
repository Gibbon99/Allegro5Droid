#pragma once

#include "system/sys_main.h"

extern int chooseSideTimeOut;  // From script
extern float chooseSideDelayTime; // From script

// Change to transfer game mode
void trn_changeToTransferGame (int targetDroid);

// Return the target droid from a collision in transfer mode
int trn_getTransferTargetDroid();

// Handle the keyboard for choosing a side for transfer game
void trn_handleTransferChooseSide ();

// Called to setup the transfer cell values
void trn_setupTransferCellValues();

// Process choose side for countdown
void trn_processChooseSide(float tickTime);

// Get the current choose side delay counter to display on HUD
int trn_getChooseSideCounter();

// Process active circuits time
void trn_processCircuits(float tickTime);

// Setup the default values for the transfer rows
void trn_initTransferValues();

// Return true if a transfer mistake is made
bool trn_makeTransferMistake (int whichDroidLevel);