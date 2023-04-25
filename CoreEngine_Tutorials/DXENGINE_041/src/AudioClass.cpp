// --------------------------------------------------------------------------------------------
// Filename: AudioClass.cpp
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
// PURPOSE: The AudioClass is the Interface Audio to Client EXE 
//			 No need to Link audio LIBs on client EXE
// --------------------------------------------------------------------------------------------

#include "platform.h"
#if defined USE_SOUND_MANAGER || defined USE_PLAY_MUSIC
#include "AudioClass.h"
#include "log.h"
#include "mem_leak.h"

//
// Private Part
//

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
//
// Private Part
//

SoundID AudioClass::insertSound(Sound &sound)
{
	for (UINT i = 0; i < sounds.size(); i++){
		if (sounds[i].samples == NULL){
			sounds[i] = sound;
			return i;
		}
	}

	sounds.push_back(sound);
	return (int)sounds.size()-1;
}

SoundID AudioClass::insertSoundSource(SoundSrc &source)
{
	for (UINT i = 0; i < soundSources.size(); i++){
		if (soundSources[i].sound == SOUND_NONE){
			soundSources[i] = source;
			return i;
		}
	}

	soundSources.push_back(source);
	return (int)soundSources.size()-1;
}

//
// Public Part
//

AudioClass::AudioClass()
{
	CLASSLOADER();
	_CrtSetDbgFlag(_CRTDBG_CHECK_DEFAULT_DF);
	ctx			= NULL;
	SoundDevice = NULL;

	SoundDevice = alcOpenDevice(NULL);
	IF_NOT_THROW_EXCEPTION (SoundDevice);

	INT Caps[] = { ALC_FREQUENCY, 44100, 0 };	// Expected
	ctx = alcCreateContext((ALCdevice*)SoundDevice, Caps);
	if (!ctx)
		ctx = alcCreateContext((ALCdevice*)SoundDevice, NULL);
	IF_NOT_THROW_EXCEPTION (ctx);

	alcMakeContextCurrent((ALCcontext*)ctx);
	ASSERT( alGetError() == AL_NO_ERROR);

	//alDistanceModel(AL_INVERSE_DISTANCE);
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	//alListenerf(AL_GAIN, 0.0f);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF| _CRTDBG_LEAK_CHECK_DF);
}

AudioClass::~AudioClass() 
{
	Shutdown();
	CLASSDELETE();
}

void AudioClass::Shutdown()
{
	if (SoundDevice) {
		// Free sounds:
		//int index = (int)sounds.size();
		//while (index--){
		for (int index = 0; index < (int)sounds.size(); index++)
		{
			//free(sounds[index].samples);
			deleteSound(index);
		}
		//}

		// Free soundSources:
		//index = (int)soundSources.size();
		for (int index = 0; index < (int)soundSources.size(); index++)
		{
			alSourceStop(index);
			deleteSoundSource(index);
		}

		if (ctx) {
			// Free ctx:
			alcProcessContext( (ALCcontext*)ctx );
			alcMakeContextCurrent(NULL); // Disable context
			alcDestroyContext((ALCcontext*)ctx);
		}

		// Free SoundDevice:
		if (SoundDevice)
		{
			alcCloseDevice((ALCdevice*)SoundDevice);
			SoundDevice= NULL;
		}
		ctx = NULL;
	}
}

#pragma warning (push)
#pragma warning (disable : 4996)
// SOUND:
SoundID AudioClass::addSound(const TCHAR *fileName, unsigned int flags)
{
	Sound sound;

	// Clear error flag
	alGetError();
	
	const TCHAR* ext = _tcsrchr(fileName, '.');// ++sizeof(TCHAR);
	TCHAR str[256];
	ext++;
	if (_tcsicmp(ext, TEXT("ogg")) == 0)
	{
		FILE *file = _tfopen(fileName, TEXT("rb"));
		if (file == NULL){
			StringCchPrintf(str, sizeof(str), TEXT("Couldn't open \"%s\""), fileName);
			WomaFatalExceptionW(str);
			return SOUND_NONE;
		}
		
		OggVorbis_File vf;
		memset(&vf, 0, sizeof(vf));
		if (ov_open(file, &vf, NULL, 0) < 0){
			fclose(file);
			StringCchPrintf(str, sizeof(str), TEXT("\"%s\" is not an ogg file"), fileName);
			WomaFatalExceptionW(str);
			return SOUND_NONE;
		}

		vorbis_info *vi = ov_info(&vf, -1);

		int nSamples = (UINT) ov_pcm_total(&vf, -1);
		int nChannels = vi->channels;
		sound.format = nChannels == 1? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		sound.sampleRate = vi->rate;

		sound.size = nSamples * nChannels;

		sound.samples = NEW short[sound.size];
		sound.size *= sizeof(short);

		int samplePos = 0;
		while (samplePos < sound.size){
			char *dest = ((char *) sound.samples) + samplePos;

			int bitStream, readBytes = ov_read(&vf, dest, sound.size - samplePos, 0, 2, 1, &bitStream);
			if (readBytes <= 0) break;
			samplePos += readBytes;
		}

		ov_clear(&vf);

	} else {
		ALboolean al_bool;
		ALvoid *data;
		alutLoadWAVFile((ALbyte*)fileName, &sound.format, &data, &sound.size, &sound.sampleRate, &al_bool);
		sound.samples = (short *) data;
	}

	alGenBuffers(1, &sound.buffer);
	alBufferData(sound.buffer, sound.format, sound.samples, sound.size, sound.sampleRate);
	if (alGetError() != AL_NO_ERROR){
		alDeleteBuffers(1, &sound.buffer);

		StringCchPrintf(str, sizeof(str), TEXT("Couldn't open \"%s\""), fileName);
		WomaFatalExceptionW(str);
		return SOUND_NONE;
	}

	return insertSound(sound);
}

void AudioClass::deleteSound(const SoundID sound)
{
	if (sounds[sound].samples){
		alDeleteBuffers(1, &sounds[sound].buffer);

		alutUnloadWAV(sounds[sound].format, sounds[sound].samples, sounds[sound].size, sounds[sound].sampleRate);
		sounds[sound].samples = NULL;
	}
}

// SOURCE:
SoundSourceID AudioClass::addSoundSource(const SoundID sound, UINT flags)
{
	SoundSrc soundSource;

	soundSource.sound = sound;

	alGenSources(1, &soundSource.source);
	alSourcei(soundSource.source, AL_LOOPING, (flags & LOOPING)? AL_TRUE : AL_FALSE);
	alSourcei(soundSource.source, AL_SOURCE_RELATIVE, (flags & RELATIVEPOS)? AL_TRUE : AL_FALSE);
	alSourcei(soundSource.source, AL_BUFFER, sounds[sound].buffer);
	alSourcef(soundSource.source, AL_MIN_GAIN, 0.0f);
	alSourcef(soundSource.source, AL_MAX_GAIN, 1.0f);

	return insertSoundSource(soundSource);
}

void AudioClass::deleteSoundSource(const SoundSourceID source)
{
	if (soundSources[source].sound != SOUND_NONE){
		alDeleteSources(1, &soundSources[source].source);
		soundSources[source].sound = SOUND_NONE;
	}
}

// Basic(s):
void AudioClass::play(const SoundSourceID source){
	alSourcePlay(soundSources[source].source);
}

void AudioClass::stop(const SoundSourceID source){
	alSourceStop(soundSources[source].source);
	alSourcei( soundSources[source].source, AL_BUFFER, NULL );
}

void AudioClass::pause(const SoundSourceID source){
	alSourcePause(soundSources[source].source);
}

bool AudioClass::isPlaying(const SoundSourceID source)
{
	ALint state;
	alGetSourcei(soundSources[source].source, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

// Setup Sound:
void AudioClass::setSourceGain(const SoundSourceID source, const float gain)
{
	alSourcef(soundSources[source].source, AL_GAIN, gain);
}

void AudioClass::setSourcePosition(const SoundSourceID source, const vec3 &position)
{
	ALfloat pos[3]={position.x, position.y, position.z};
	alSourcefv(soundSources[source].source, AL_POSITION, pos);
}

void AudioClass::setSourceAttenuation(const SoundSourceID source, const float rollOff, const float refDistance)
{
	alSourcef(soundSources[source].source, AL_REFERENCE_DISTANCE, refDistance);
	alSourcef(soundSources[source].source, AL_ROLLOFF_FACTOR, rollOff);
}

// Frame()
void AudioClass::setListenerOrientation(const vec3 &position, const vec3 &zDir, const vec3 &vel)
{
	ALfloat pos[3]={position.x, position.z, position.y};	// Z/Y swap?
	alListenerfv(AL_POSITION, pos);

	ALfloat orient[] = { zDir.x, zDir.z, zDir.y, 0, -1, 0 };// Z/Y swap?
	alListenerfv(AL_ORIENTATION, orient);

	ALfloat Velocity[] = { vel.x, vel.z, vel.y};			// Z/Y swap?
	alListenerfv( AL_VELOCITY, Velocity);
}

#pragma warning (pop)

#endif
