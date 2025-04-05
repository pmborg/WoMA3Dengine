///////////////////////////////////////////////////////////////////////////////
// Filename: soundClass.cpp
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
#include "platform.h"

//#define DX_ENGINE_LEVEL 71  SOUND EFFECT (Add Support): .................... /sound/soundClass.*
//#define DX_ENGINE_LEVEL 72  3D SOUND EFFECT (Add Support): ................. /sound/soundClass.*

#include "OSengine.h"
#include "soundclass.h"
#include "mem_leak.h"
#include "ApplicationClass.h"


//Use the class constructor to initialize the private member variables that are used inside the sound class.
SoundClass::SoundClass()
{
	m_DirectSound = NULL;
	m_primaryBuffer = NULL;
	m_secondaryBuffer1 = NULL;

	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	m_listener = NULL;
	m_secondary3DBuffer1 = NULL;
	#endif
}
 
SoundClass::~SoundClass() {Shutdown();}
  
bool SoundClass::Initialize(HWND hwnd, char* filename)
{
	bool result;

	//First initialize the DirectSound API as well as the primary buffer. 
	//Once that is initialized then the LoadWaveFile function can be called which will load in the .wav audio file and 
	//initialize the secondary buffer with the audio information from the .wav file. 
	//After loading is complete then PlayWaveFile is called which then plays the .wav file once.

	// Initialize direct sound and the primary sound buffer.
	result = InitializeDirectSound(hwnd);
	if(!result){return false;}
 
	// Load a wave audio file onto a secondary buffer.
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	result = LoadWaveFile(filename, &m_secondaryBuffer1, &m_secondary3DBuffer1);
	#else
	result = LoadWaveFile(filename, &m_secondaryBuffer1);
	#endif
	if(!result){return false;}

	//WOMA_LOGManager_DebugMSG( TEXT("Sound Class: Initialized\n") );
	return true;
}

//The Shutdown function first releases the secondary buffer which held the .wav file audio data using the ShutdownWaveFile function. 
//Once that completes this function then called ShutdownDirectSound which releases the primary buffer and the DirectSound interface.

void SoundClass::Shutdown()
{
	// Release the secondary buffer.
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	ShutdownWaveFile(&m_secondaryBuffer1, &m_secondary3DBuffer1);
	#else
	ShutdownWaveFile(&m_secondaryBuffer1);
	#endif

	// Shutdown the Direct Sound API.
	ShutdownDirectSound();
 
	return;
}

//InitializeDirectSound handles getting an interface pointer to DirectSound and the default primary sound buffer. 
//Note that you can query the system for all the sound devices and then grab the pointer to the primary sound buffer for 
//a specific device, however I've kept this tutorial simple and just grabbed the pointer to the primary buffer of the default sound device.

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result = { };
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	DSBUFFERDESC bufferDesc = {0};
	WAVEFORMATEX waveFormat = {0};
	#endif

	// [1] Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if(FAILED(result))
		return false;
 
	// [2] Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		result = m_DirectSound->SetCooperativeLevel(GetDesktopWindow(), DSSCL_PRIORITY);

	ASSERT(!FAILED(result));

	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// [3] Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if(FAILED(result))
		return false;

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec	= 44100; // waveFileHeader.sampleRate
	waveFormat.wBitsPerSample	= 16;    // waveFileHeader.bitsPerSample
	waveFormat.nChannels		= 2;     // waveFileHeader.numChannels
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// [4] Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result))
		return false;

	// Obtain a listener interface.
	result = m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener);
	if(FAILED(result))
		return false;

	// Set the initial position of the listener to be in the middle of the scene.
	m_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	#endif

	return true;
}

//The ShutdownDirectSound function handles releasing the primary buffer and DirectSound interfaces.
void SoundClass::ShutdownDirectSound()
{
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	SAFE_RELEASE (m_listener);
	#endif

	// Release the primary sound buffer pointer.
	SAFE_RELEASE (m_primaryBuffer);
 
	// Release the direct sound interface pointer.
	SAFE_RELEASE (m_DirectSound);
 
	return;
}

//The LoadWaveFile function is what handles loading in a .wav audio file and then copies the data onto a new secondary buffer. 
//If you are looking to do different formats you would replace this function or write a similar one.
#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
#else
bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
#endif
{
	int error=0;
	FILE* filePtr=NULL;
	unsigned int count=0;
	WAVEFORMATEX waveFormat = {};
	DSBUFFERDESC bufferDesc = {};
	HRESULT result = {};
	IDirectSoundBuffer* tempBuffer=NULL;
	unsigned char* waveData=NULL;
	unsigned char *bufferPtr=NULL;
	unsigned long bufferSize=0;

	//To start first open the .wav file and read in the header of the file. 
	//The header will contain all the information about the audio file so we can use that to create a secondary buffer to 
	//accommodate the audio data. The audio file header also tells us where the data begins and how big it is. 
	//You will notice I check for all the needed tags to ensure the audio file is not corrupt and is the proper wave file format 
	//containing RIFF, WAVE, fmt, data, and WAVE_FORMAT_PCM tags. I also do a couple other checks to ensure it is a 
	//44.1KHz stereo 16bit audio file. If it is mono, 22.1 KHZ, 8bit, or anything else then it will fail ensuring we are only 
	//loading the exact format we want.

	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
		return false;
	 
	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1)
		return false;
 
	// Check that the chunk ID is the RIFF format.
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || 
	   (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		return false;
 
	// Check that the file format is the WAVE format.
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
	   (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
		return false;
 
	// Check that the sub chunk ID is the fmt format.
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
	   (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		return false;
 
	// Check that the audio format is WAVE_FORMAT_PCM.
	ASSERT(waveFileHeader.audioFormat == WAVE_FORMAT_PCM);

	// Check that the wave file was recorded in stereo format.
	//ASSERT(waveFileHeader.numChannels == 2);
 
	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	ASSERT(waveFileHeader.sampleRate == 44100);
 
	// Ensure that the wave file was recorded in 16 bit format.
	ASSERT(waveFileHeader.bitsPerSample == 16);



//Now that the wave header file has been verified we can setup the secondary buffer we will load the audio data onto. 
//We have to first set the wave format and buffer description of the secondary buffer similar to how we did for the primary buffer. There are some changes though since this is secondary and not primary in terms of the dwFlags and dwBufferBytes.

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = waveFileHeader.sampleRate;		//44100;
	waveFormat.wBitsPerSample = waveFileHeader.bitsPerSample;	//16;
	waveFormat.nChannels = waveFileHeader.numChannels;			//2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
 
	//DWORD           dwSize;
	//DWORD           dwFlags;
	//DWORD           dwBufferBytes;
	//DWORD           dwReserved;
	//LPWAVEFORMATEX  lpwfxFormat;
	//#if DIRECTSOUND_VERSION >= 0x0700
	//GUID            guid3DAlgorithm;
	//#endif

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	#else
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	#endif

	//bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		bufferDesc.dwBufferBytes = waveFileHeader.chunkSize;
	else
		bufferDesc.dwBufferBytes = waveFileHeader.dataSize;

	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

//Now the way to create a secondary buffer is fairly strange. First step is that you create a temporary IDirectSoundBuffer with 
//the sound buffer description you setup for the secondary buffer. 
//If this succeeds then you can use that temporary buffer to create a IDirectSoundBuffer8 secondary buffer by calling QueryInterface 
//with the IID_IDirectSoundBuffer8 parameter. If this succeeds then you can release the temporary buffer and the secondary buffer is ready for use.

	// [3] Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	/*
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	WomaMessageBox((TCHAR*)lpMsgBuf, TEXT("Error"));
	LocalFree(lpMsgBuf);
	*/
	if(FAILED(result))
		return false;
 
	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);	
	if(FAILED(result))
		return false;

	// Release the temporary buffer.
	SAFE_RELEASE (tempBuffer);

//Now that the secondary buffer is ready we can load in the wave data from the audio file. 
//First I load it into a memory buffer so I can check and modify the data if I need to. 
//Once the data is in memory you then lock the secondary buffer, copy the data to it using a memcpy, and then unlock it. 
//This secondary buffer is now ready for use. Note that locking the secondary buffer can actually take in two pointers and 
//two positions to write to. This is because it is a circular buffer and if you start by writing to the middle of it you will need 
//the size of the buffer from that point so that you don't write outside the bounds of it. 
//This is useful for streaming audio and such. In this tutorial we create a buffer that is the same size as the audio file and 
//write from the beginning to make things simple.

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);
 
	// Create a temporary buffer to hold the wave file data.
	waveData = NEW unsigned char[bufferDesc.dwBufferBytes];
	if(!waveData)return false;
 
	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, bufferDesc.dwBufferBytes, filePtr);
	//if(count != bufferDesc.dwBufferBytes)
	//	return false;
	 
	// Close the file once done reading.
	if (filePtr)
		error = fclose(filePtr);
	if(error != 0)
		return false;
 
	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, bufferDesc.dwBufferBytes, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))
		return false;
 
	// Copy the wave data into the buffer. (waveData -> bufferPtr)
	memcpy(bufferPtr, waveData, bufferDesc.dwBufferBytes);
 
	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result))
		return false;
	
	// Release the wave data since it was copied into the secondary buffer.
	SAFE_DELETE_ARRAY ( waveData);
 
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D && defined SOUND3D
	// Get the 3D interface to the secondary sound buffer.
	result = (*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer);
	if(FAILED(result))
		return false;
	#endif

	return true;
}

//ShutdownWaveFile just does a release of the secondary buffer.
#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
#else
void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
#endif
{
	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	SAFE_RELEASE (*secondary3DBuffer);
	#endif

	// Release the secondary sound buffer.
	SAFE_RELEASE (*secondaryBuffer);

	return;
}

//The PlayWaveFile function will play the audio file stored in the secondary buffer. 
//The moment you use the Play function it will automatically mix the audio into the primary buffer and start it playing 
//if it wasn't already. Also note that we set the position to start playing at the beginning of the secondary sound buffer 
//otherwise it will continue from where it last stopped playing. And since we set the capabilities of the buffer to allow us 
//to control the sound we set the volume to maximum here.
#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
bool SoundClass::PlayWaveFile(float positionX, float positionY, float positionZ, bool loop, float range)
#else
bool SoundClass::PlayWaveFile()
#endif
{
 	// Set position at the beginning of the sound buffer.
	if(FAILED(m_secondaryBuffer1->SetCurrentPosition(0)))
		return false;
 
	if(FAILED(m_secondary3DBuffer1->SetMinDistance(range, DS3D_IMMEDIATE)))
		return false;
	
	// Set volume of the buffer to 100%.
	// DSBVOLUME_MIN               -10000
	// DSBVOLUME_MAX               0
	if(FAILED(m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX)))
		return false;

	#if DX_ENGINE_LEVEL >= 72 && defined SOUND3D
	// Set the 3D position of the sound.
	m_secondary3DBuffer1->SetPosition(positionX, positionY, positionZ, DS3D_IMMEDIATE);

	// Set the initial position of the listener to be in the middle of the scene.
	if (SystemHandle->m_Application->m_Position.size() == 0) {
		m_listener->SetPosition(0, 0, 0, DS3D_IMMEDIATE);
	} else {
		m_listener->SetPosition(SystemHandle->m_Application->m_Position[g_NetID]->m_positionX, 
			SystemHandle->m_Application->m_Position[g_NetID]->m_positionY, 
			SystemHandle->m_Application->m_Position[g_NetID]->m_positionZ, DS3D_IMMEDIATE);
	}
	#endif

	// Play the contents of the secondary sound buffer.
	if(FAILED(m_secondaryBuffer1->Play(0, 0, (loop)?DSBPLAY_LOOPING:0)))
		return false;

	return true;
}
