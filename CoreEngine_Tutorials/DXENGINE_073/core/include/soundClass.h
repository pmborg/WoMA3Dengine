// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: soundclass.h
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
// PURPOSE: Add local/3D sound/effects.
// ORIGINAL VERSION: https://en.wikibooks.org/wiki/DirectX/10.0/Direct3D/Direct_Sound
// --------------------------------------------------------------------------------------------
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

//The SoundClass encapsulates the DirectSound functionality as well as the .wav audio loading and playing capabilities.
//The following libraries and headers are required for DirectSound to compile properly.

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
 
 ///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

extern IDirectSound8* m_DirectSound;
///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
///////////////////////////////////////////////////////////////////////////////
class SoundClass
{
private:

//The WaveHeaderType structure used here is for the .wav file format. When loading in .wav files 
//I first read in the header to determine the required information for loading in the .wav audio data. 
//If you are using a different format you will want to replace this header with the one required for your audio format.
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
 
public:
	SoundClass();
	~SoundClass();

//Initialize and Shutdown will handle everything needed for this tutorial. The Initialize function will initialize DirectSound and 
//load in the .wav audio file and then play it once. Shutdown will release the .wav file and shutdown DirectSound.
	bool Initialize(HWND hwnd, char* filename);
	void Shutdown();
 
private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	bool LoadWaveFile(char*, IDirectSoundBuffer8**, IDirectSound3DBuffer8**);
#else
	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
#endif

#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	void ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer);
#else
	void ShutdownWaveFile(IDirectSoundBuffer8**);
#endif

	IDirectSoundBuffer* m_primaryBuffer = NULL;
	IDirectSoundBuffer8* m_secondaryBuffer1 = NULL;	// For MUSIC

	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	IDirectSound3DBuffer8* m_secondary3DBuffer1;	// For Sound Effect
	#endif

	WaveHeaderType waveFileHeader = {};

public:
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	IDirectSound3DListener8* m_listener;
	#endif

#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	bool PlayWaveFile(float positionX, float positionY, float positionZ, bool loop, float range);
#else
	bool PlayWaveFile();
#endif
};
 
#endif

