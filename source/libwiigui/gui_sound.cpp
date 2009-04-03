/****************************************************************************
 * libwiigui
 * Tantric 2009
 *
 * gui_sound.cpp
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"

/**
 * Constructor for the GuiSound class.
 */
GuiSound::GuiSound(const u8 * snd, s32 len, int t)
{
	sound = snd;
	length = len;
	type = t;
	voice = -1;
	volume = 100;
}

/**
 * Destructor for the GuiSound class.
 */
GuiSound::~GuiSound()
{
}

void GuiSound::Play()
{
	int vol;

	switch(type)
	{
		case SOUND_PCM:
		vol = 255*(volume/100.0);
		voice = ASND_GetFirstUnusedVoice();
		if(voice >= 0)
			ASND_SetVoice(voice, VOICE_MONO_8BIT, 8000, 0,
				(u8 *)sound, length, vol, vol, NULL);
		break;

		case SOUND_OGG:
		voice = 0;
		PlayOgg(mem_open((char *)sound, length), 0, OGG_INFINITE_TIME);
		SetVolumeOgg(255*(volume/100.0));
		break;
	}
}

void GuiSound::Stop()
{
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND_PCM:
		ASND_StopVoice(voice);
		break;

		case SOUND_OGG:
		StopOgg();
		break;
	}
}

void GuiSound::Pause()
{
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND_PCM:
		ASND_PauseVoice(voice, 1);
		break;

		case SOUND_OGG:
		PauseOgg(1);
		break;
	}
}

void GuiSound::Resume()
{
	if(voice < 0)
		return;

	switch(type)
	{
		case SOUND_PCM:
		ASND_PauseVoice(voice, 0);
		break;

		case SOUND_OGG:
		PauseOgg(0);
		break;
	}
}

void GuiSound::SetVolume(int vol)
{
	volume = vol;

	if(voice < 0)
		return;

	int newvol = 255*(volume/100.0);

	switch(type)
	{
		case SOUND_PCM:
		ASND_ChangeVolumeVoice(voice, newvol, newvol);
		break;

		case SOUND_OGG:
		SetVolumeOgg(255*(volume/100.0));
		break;
	}
}
