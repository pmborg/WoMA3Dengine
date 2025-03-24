// --------------------------------------------------------------------------------------------
// Filename: linuxaudio.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: DEFINE COMMON WorldOfMiddleAge 3D ENGINE MACROS
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#if notes
	//sudo apt pulseaudio alsa-utils install snd-hda-intel sox libsox-fmt-all vlc-bin ffmpeg libopenal-dev libasound2-dev libasound2-plugins
	//vi ~/ .asoundrc
	//pcm.default pulse
	//ctl.default pulse
	//https://www.reddit.com/r/bashonubuntuonwindows/comments/hrn1lz/wsl_sound_through_pulseaudio_solved/?rdt=50845
	//https://www.linuxuprising.com/2021/03/how-to-get-sound-pulseaudio-to-work-on.html
#endif

#include "OSengine.h"
#if DX_ENGINE_LEVEL >= 29

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <unistd.h>

#include "woma_macros.h"
#include "openalclass.h"
#include "soundclass.h"
#include "OSmain_dir.h"

SoundClass* linux_play_wav(std::string filename)
{
	bool result = true;

	//std::string fullfilename="~/projects/LinuxWoma029/CoreEngine_Tutorials/DXENGINE_029/";
	STRING fullfilename = WOMA::Home;
	fullfilename.append("/projects/LinuxWoma0");
	fullfilename.append(std::to_string(DX_ENGINE_LEVEL));
	fullfilename.append("/WoMA3Dengine/CoreEngine_Tutorials/DXENGINE_0");
	fullfilename.append(std::to_string(DX_ENGINE_LEVEL));
	fullfilename.append("/");
	fullfilename.append(filename);

	SoundClass* sound;

	// Create and initialize the OpenAL object.
	if (!SystemHandle->m_OpenAL)
		SystemHandle->m_OpenAL = new OpenALClass;
	result = SystemHandle->m_OpenAL->Initialize();
	if (!result)
	{
		cout << "Error: Could not initialize the OpenAL object." << endl;
		return NULL;
	}

	// Create and initialize the sound object.
	sound = new SoundClass;
	result = sound->LoadTrack((char*)fullfilename.c_str(), 1.0f);
	if (!result)
	{
		cout << "Error: Could not initialize the test sound 1 object." << endl;
		return NULL;
	}

	sound->PlayTrack(true);	// Play the sound.

	return sound;
}

void linux_stop_wav(SoundClass* sound)
{
	if (sound)
		sound->StopTrack();	// Stop the sound if it was still playing.
	SAFE_DELETE(sound);	// Release the sound object.

	return;
}

void linux_audio_shutdown()
{
	SAFE_DELETE(SystemHandle->m_OpenAL);	// Release the OpenAL object.

	return;
}
#endif