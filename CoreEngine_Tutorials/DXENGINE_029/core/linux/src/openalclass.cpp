// --------------------------------------------------------------------------------------------
// Filename: openalclass.cpp
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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut56.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567222;

#include "openalclass.h"

#if notes
	//sudo apt pulseaudio alsa-utils install snd-hda-intel sox libsox-fmt-all vlc-bin ffmpeg libopenal-dev libasound2-dev libasound2-plugins
	//vi ~/ .asoundrc
	//pcm.default pulse
	//ctl.default pulse
	//https://www.reddit.com/r/bashonubuntuonwindows/comments/hrn1lz/wsl_sound_through_pulseaudio_solved/?rdt=50845
	//https://www.linuxuprising.com/2021/03/how-to-get-sound-pulseaudio-to-work-on.html
#endif

OpenALClass::OpenALClass()
{
}

OpenALClass::~OpenALClass()
{
}

bool OpenALClass::Initialize()
{
    ALCdevice* device;
    ALCcontext* context;
    float position[3];
    bool result;
    
    
    // Select the default audio device on the system.
    device = alcOpenDevice(NULL);
    if(!device)
        return false;

    // Create a context on the device.
    context = alcCreateContext(device, NULL);

    // Open the context by setting it as the current context on the device.
    alcMakeContextCurrent(context);

    // Set the initial position of the listener.
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;

    // Clear any previous unaddressed error codes.
    alGetError();
    
    // Set the listener position.
    alListenerfv(AL_POSITION, position);
    if(alGetError() != AL_NO_ERROR)
		return false;

    return true;
}


void OpenALClass::Shutdown()
{
    ALCdevice* device;
    ALCcontext* context;

    
    // Retrieve the current context and device.
    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);
 
    // Unset our audio context as the current one.
    alcMakeContextCurrent(NULL);

    // Destroy our audio context.
    alcDestroyContext(context);

    // Close our audio device.
    alcCloseDevice(device);
    
    return;
}
