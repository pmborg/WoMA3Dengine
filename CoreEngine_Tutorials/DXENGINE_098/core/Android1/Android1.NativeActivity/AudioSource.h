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
}  // namespace wavetablesynthesizer