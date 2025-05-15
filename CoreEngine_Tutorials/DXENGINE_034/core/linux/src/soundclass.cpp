// --------------------------------------------------------------------------------------------
// Filename: soundclass.cpp
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
//WomaIntegrityCheck = 1234567155;

#include "soundclass.h"
#include "woma_macros.h"

SoundClass::SoundClass()
{
    m_waveData = 0;
}

SoundClass::~SoundClass()
{
}

bool SoundClass::LoadTrack(char* filename, float volume)
{
    bool result;
    
    // Initialize the error state through a reset.
    alGetError();

    // Generate an audio buffer.
    alGenBuffers(1, &m_audioBufferId);
    if(alGetError() != AL_NO_ERROR)
		return false;


    // Load the wave file for the sound.
    result = LoadStereoWaveFile(filename);
    if(!result)
        return false;

    // Copy the wav data into the audio buffer.
    alBufferData(m_audioBufferId, AL_FORMAT_STEREO16, m_waveData, m_waveSize, 44100);
    if(alGetError() != AL_NO_ERROR)
		return false;

    // Release the wave data since it was copied into the OpenAL buffer.
    ReleaseWaveFile();

    // Generate an audio source.
    alGenSources(1, &m_audioSourceId);
    if(alGetError() != AL_NO_ERROR)
		return false;

    // Attach the buffer to the source.
    alSourcei(m_audioSourceId, AL_BUFFER, m_audioBufferId);
    if(alGetError() != AL_NO_ERROR)
		return false;

    // Set the volume to max.
    alSourcef(m_audioSourceId, AL_GAIN, 1.0f);
    if(alGetError() != AL_NO_ERROR)
		return false;
 
    return true;
}


void SoundClass::ReleaseTrack()
{
    // Release the audio source.
    alDeleteSources(1, &m_audioSourceId);
  
    // Release the audio buffer.
    alDeleteBuffers(1, &m_audioBufferId);

    return;
}


bool SoundClass::PlayTrack(bool looping)
{
    // Initialize the error state through a reset.
    alGetError();
    
    if(looping == true)
    {
        // Set it to looping.
        alSourcei(m_audioSourceId, AL_LOOPING, AL_TRUE);
        if(alGetError() != AL_NO_ERROR)
            return false;
    }
    else
    {
        // Set it to not looping.
        alSourcei(m_audioSourceId, AL_LOOPING, AL_FALSE);
        if(alGetError() != AL_NO_ERROR)
            return false;
    }

    // If looping is on then play the contents of the sound buffer in a loop, otherwise just play it once.
    alSourcePlay(m_audioSourceId);
    if(alGetError() != AL_NO_ERROR)
        return false;
	
    return true;
}


bool SoundClass::StopTrack()
{
    // Initialize the error state through a reset.
    alGetError();
    
    // Stop the sound from playing.
    alSourceStop(m_audioSourceId);
    if(alGetError() != AL_NO_ERROR)
        return false;

    return true;
}


bool SoundClass::LoadStereoWaveFile(char* filename)
{
    FILE* filePtr;
    RiffWaveHeaderType riffWaveFileHeader;
    SubChunkHeaderType subChunkHeader;
    FmtType fmtData;
    unsigned int count, seekSize;
    bool foundFormat, foundData;
    
    
    // Open the wave file for reading in binary.
    filePtr = fopen(filename, "rb");
    if(filePtr == NULL)
        return false;

    // Read in the riff wave file header.
    count = fread(&riffWaveFileHeader, sizeof(riffWaveFileHeader), 1, filePtr);
    if(count != 1)
        return false;

    // Check that the chunk ID is the RIFF format.
    if((riffWaveFileHeader.chunkId[0] != 'R') || (riffWaveFileHeader.chunkId[1] != 'I') || (riffWaveFileHeader.chunkId[2] != 'F') || (riffWaveFileHeader.chunkId[3] != 'F'))
        return false;

    // Check that the file format is the WAVE format.
    if((riffWaveFileHeader.format[0] != 'W') || (riffWaveFileHeader.format[1] != 'A') || (riffWaveFileHeader.format[2] != 'V') || (riffWaveFileHeader.format[3] != 'E'))
        return false;

    // Read in the sub chunk headers until you find the format chunk.
    foundFormat = false;
    while(foundFormat == false)
    {
        // Read in the sub chunk header.
        count = fread(&subChunkHeader, sizeof(subChunkHeader), 1, filePtr);
        if(count != 1)
            return false;

        // Determine if it is the fmt header.  If not then move to the end of the chunk and read in the next one.
        if((subChunkHeader.subChunkId[0] == 'f') && (subChunkHeader.subChunkId[1] == 'm') && (subChunkHeader.subChunkId[2] == 't') && (subChunkHeader.subChunkId[3] == ' '))
        {
            foundFormat = true;
        }
        else
        {
            fseek(filePtr, subChunkHeader.subChunkSize, SEEK_CUR);
        }
    }

    // Read in the format data.
    count = fread(&fmtData, sizeof(fmtData), 1, filePtr);
    if(count != 1)
        return false;

    // Check that the audio format is WAVE_FORMAT_PCM (1).
    if(fmtData.audioFormat != 1)
        return false;

    // Check that the wave file was recorded in stereo format.
    if(fmtData.numChannels != 2)
        return false;

    // Check that the wave file was recorded at a sample rate of 44.1 KHz.
    if(fmtData.sampleRate != 44100)
        return false;

    // Ensure that the wave file was recorded in 16 bit format.
    if(fmtData.bitsPerSample != 16)
        return false;

    // Seek up to the next sub chunk.
    seekSize = subChunkHeader.subChunkSize - 16;
    fseek(filePtr, seekSize, SEEK_CUR);

    // Read in the sub chunk headers until you find the data chunk.
    foundData = false;
    while(foundData == false)
    {
        // Read in the sub chunk header.
        count = fread(&subChunkHeader, sizeof(subChunkHeader), 1, filePtr);
        if(count != 1)
            return false;

        // Determine if it is the data header.  If not then move to the end of the chunk and read in the next one.
        if((subChunkHeader.subChunkId[0] == 'd') && (subChunkHeader.subChunkId[1] == 'a') && (subChunkHeader.subChunkId[2] == 't') && (subChunkHeader.subChunkId[3] == 'a'))
        {
            foundData = true;
        }
        else
        {
            fseek(filePtr, subChunkHeader.subChunkSize, SEEK_CUR);
        }
    }

    // Store the size of the data chunk.
    m_waveSize = subChunkHeader.subChunkSize;

    // Create a temporary buffer to hold the wave file data.
    m_waveData = new unsigned char[m_waveSize];

    // Read in the wave file data into the newly created buffer.
    count = fread(m_waveData, 1, m_waveSize, filePtr);
    if(count != m_waveSize)
        return false;
    
    // Close the file once done reading.
    fclose(filePtr);

    return true;
}


void SoundClass::ReleaseWaveFile()
{
    // Release the wave data.
	SAFE_DELETE_ARRAY(m_waveData);

    return;
}
