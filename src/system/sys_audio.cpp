#include <hdr/io/io_resources.h>
#include <hdr/io/io_logFile.h>
#include "system/sys_main.h"
#include "system/sys_audio.h"

int numReserveSamples;      // From config file

//---------------------------------------------------------------------------------------------------------------------------
//
// Start the Audio system
bool snd_initAudioSystem()
//---------------------------------------------------------------------------------------------------------------------------
{
	if (!al_init_acodec_addon())
		return false;

	if (!al_install_audio ())
		return false;

	printf("Audio - numSamples [ %i ]\n", numReserveSamples);

	if (!al_reserve_samples(numReserveSamples))
		return false;

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Stop playing a sound - pass by string keyName
void snd_stopSound(const std::string &key)
//---------------------------------------------------------------------------------------------------------------------------
{
	try
		{
			al_stop_sample (&audio.at (key).sampleID);
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Audio [ %s ] not found – [ %s ]", key.c_str (), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Play an Audio file
void snd_playSound (const std::string &key, ALLEGRO_PLAYMODE loop, float gain, float pan)
//---------------------------------------------------------------------------------------------------------------------------
{
	bool              playResult;

	try
		{
			playResult = al_play_sample (audio.at (key).audio, gain, pan, 1.0, loop, &audio.at (key).sampleID);

			if (!playResult)
				{
					log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sample [ %s ] failed to play", key.c_str ()));
					return;
				}
		}

	catch (const std::out_of_range &oor)
	{
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Audio [ %s ] not found – [ %s ]", key.c_str (), oor.what ()));
		return;
	}
}