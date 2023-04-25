// --------------------------------------------------------------------------------------------
// Filename: AudioClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
#pragma once

#include "platform.h"
#include "Math3D.h"
#include <AL/al.h>
#include <AL/alc.h>

typedef int SoundID;
typedef int SoundSourceID;

#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "x64/Debug/OpenAL32_LIBX64_d.lib" )
#else
#pragma comment( lib, "OpenAL32_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, x64/Debug/"OpenAL32_LIB_d.lib" )
#else
#pragma comment( lib, "OpenAL32_LIB.lib" )
#endif
#endif

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "x64/Debug/ALUT_LIBX64_d.lib" )
#else
#pragma comment( lib, "ALUT_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "x64/Debug/ALUT_LIB_d.lib" )
#else
#pragma comment( lib, "ALUT_LIB.lib" )
#endif
#endif

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "x64/Debug/OGG_LIBX64_d.lib" )
#else
#pragma comment( lib, "OGG_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "x64/Debug/OGG_LIB_d.lib" )
#else
#pragma comment( lib, "OGG_LIB.lib" )
#endif
#endif

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "VORBIS_LIBX64_d.lib" )
#else
#pragma comment( lib, "VORBIS_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "VORBIS_LIB_d.lib" )
#else
#pragma comment( lib, "VORBIS_LIB.lib" )
#endif
#endif

#ifdef X64
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "VORBISFILE_LIBX64_d.lib" )
#else
#pragma comment( lib, "VORBISFILE_LIBX64.lib" )
#endif
#else
#if defined(_DEBUG) & !defined(NDEBUG)
#pragma comment( lib, "VORBISFILE_LIB_d.lib" )
#else
#pragma comment( lib, "VORBISFILE_LIB.lib" )
#endif
#endif

#endif

#define SOUND_NONE (-1)

// Sound source flags
#define LOOPING 0x1
#define RELATIVEPOS 0x2

struct Sound {
	ALenum format;
	ALuint buffer;

	short *samples;
	int sampleRate;
	int size;
};

struct SoundSrc {
	ALuint source;
	SoundID sound;
};

class AudioClass {
public:
	AudioClass();
	~AudioClass();

	void Shutdown();

	// SOUND:
	SoundID addSound(const TCHAR *fileName, UINT flags = 0);
	void deleteSound(const SoundID sound);

	// SOURCE:
	SoundSourceID addSoundSource(const SoundID sound, UINT flags = 0);
	void deleteSoundSource(const SoundSourceID source);

	// Basic(s):
	void play(const SoundSourceID source);
	void stop(const SoundSourceID source);
	void pause(const SoundSourceID source);
	bool isPlaying(const SoundSourceID source);

	// Setup Sound:
	void setSourceGain(const SoundSourceID source, const float gain);
	void setSourcePosition(const SoundSourceID source, const vec3 &position);
	void setSourceAttenuation(const SoundSourceID source, const float rollOff, const float refDistance);

	// Frame()
	void setListenerOrientation(const vec3 &position, const vec3 &zDir, const vec3 &velocity);

private:
	SoundID insertSound(Sound &sound);
	SoundID insertSoundSource(SoundSrc &source);

	ALCcontext* ctx;
	ALCdevice* SoundDevice;

	std::vector <Sound> sounds;
	std::vector <SoundSrc> soundSources;
};
