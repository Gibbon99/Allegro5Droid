#pragma once

#include <allegro5/allegro_audio.h>
#include "system/sys_main.h"

#define MAX_NUM_RESERVE_SAMPLES     16
#define MAX_VOLUME_LEVEL            10

extern int  numReserveSamples;
extern bool enableSound;           // From config file
extern bool soundInitDone;         // Was sound done at startup
extern int  volumeLevel;            // From config file
extern std::string volumeLevelStr;    // Pass to script

// Start the Audio system
bool snd_initAudioSystem ();

// Play an Audio file
void snd_playSound (const std::string &key, ALLEGRO_PLAYMODE loop, float gain, float pan);

// Stop playing a sound - pass by string keyName
void snd_stopSound (const std::string &key);

// Return true if a sound is playing
bool snd_isPlaying (const std::string &key);