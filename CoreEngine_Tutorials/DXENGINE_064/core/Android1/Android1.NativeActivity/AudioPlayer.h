// --------------------------------------------------------------------------------------------
// Filename: AudioPlayer.h
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
	class AudioPlayer {
	public:
		virtual ~AudioPlayer() = default;

		// Start the audio device
		virtual int32_t play() = 0;

		// Stop the audio device
		virtual void stop() = 0;
	};
}
// namespace wavetablesynthesizer