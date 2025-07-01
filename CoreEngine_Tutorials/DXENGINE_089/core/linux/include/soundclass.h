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
// ORIGINAL: Rastertek Tutorial 14: Font Engine : https://www.rastertek.com/gl4linuxtut56.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525256;

#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "openalclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
////////////////////////////////////////////////////////////////////////////////
class SoundClass
{
private:
    struct RiffWaveHeaderType
    {
        char chunkId[4];
        unsigned int chunkSize;
        char format[4];
    };

    struct SubChunkHeaderType
    {
        char subChunkId[4];
        unsigned int subChunkSize;
    };

    struct FmtType
    {
        unsigned short audioFormat;
        unsigned short numChannels;
        unsigned int sampleRate;
        unsigned int bytesPerSecond;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
    };
  
public:
    SoundClass();
    ~SoundClass();

    bool LoadTrack(char*, float);
    void ReleaseTrack();

    bool PlayTrack(bool);
    bool StopTrack();
    
private:
    bool LoadStereoWaveFile(char*);
    void ReleaseWaveFile();
  
private:
    unsigned int m_audioBufferId, m_audioSourceId;
    unsigned char* m_waveData;
    unsigned int m_waveSize;
};

#endif