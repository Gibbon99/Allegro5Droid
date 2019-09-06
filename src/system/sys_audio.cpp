#include <hdr/io/io_resources.h>
#include <hdr/io/io_logFile.h>
#include "system/sys_main.h"
#include "system/sys_audio.h"

//---------------------------------------------------------------------------------------------------------------------------
//
// Play an Audio file
void snd_playSound (const std::string &key, ALLEGRO_PLAYMODE loop, float gain, float pan)
//---------------------------------------------------------------------------------------------------------------------------
{
	bool              playResult;
	ALLEGRO_SAMPLE_ID *sampleID = nullptr;

	try
	{
		playResult = al_play_sample (audio.at (key).audio, gain, pan, 1.0, loop, sampleID);

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

//
// Store the playing sample ID with the key – is it used by anything?
}