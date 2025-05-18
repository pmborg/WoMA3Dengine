#pragma once

#include <memory>

namespace wavetablesynthesizer {
	class AudioSource;
	class AudioPlayer;

	constexpr auto samplingRate = 48000;

	class WavetableSynthesizer {
	public:
		WavetableSynthesizer();

		~WavetableSynthesizer();

		void play();

		void stop();

		bool isPlaying() const;

		void setFrequency(float frequencyInHz);

		void setVolume(float volumeInDb);

		void setWavetable(Wavetable wavetable);

	private:
		bool _isPlaying = false;
		std::shared_ptr<AudioSource> _oscillator;
		std::unique_ptr<AudioPlayer> _audioPlayer;
	};
}  // namespace wavetablesynthesizer