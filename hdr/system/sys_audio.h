#pragma once

#include <allegro5/allegro_audio.h>
#include "system/sys_main.h"

extern int numReserveSamples;

// Start the Audio system
bool snd_initAudioSystem();

// Play an Audio file
void snd_playSound (const std::string &key, ALLEGRO_PLAYMODE loop, float gain, float pan);

// Stop playing a sound - pass by string keyName
void snd_stopSound(const std::string &key);

// Return true if a sound is playing
bool snd_isPlaying (const std::string &key);