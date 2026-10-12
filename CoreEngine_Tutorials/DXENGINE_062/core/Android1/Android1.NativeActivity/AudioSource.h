// --------------------------------------------------------------------------------------------
// Filename: AudioSource.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2026
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// PURPOSE:
//
// ********************************************************************************************
//WomaIntegrityCheck = 1234526920;
#pragma once

namespace wavetablesynthesizer {
	class AudioSource {
	public:
		virtual ~AudioSource() = default;

		// Return 1 sample of audio to be played back
		virtual float getSample() = 0;

		// A callback invoked when the audio stream is stopped
		virtual void onPlaybackStopped() = 0;
	};
}  
// namespace wavetablesynthesizer