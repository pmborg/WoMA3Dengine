#include "OboeAudioPlayer.h"

#include <utility>
#include "AudioSource.h"

using namespace oboe;

namespace wavetablesynthesizer {
    OboeAudioPlayer::OboeAudioPlayer(std::shared_ptr<AudioSource> source,
        int samplingRate)
        : _source(std::move(source)), _samplingRate(samplingRate) {}

    OboeAudioPlayer::~OboeAudioPlayer() {
        // Ensure that the playback is stopped when the AudioPlayer is destroyed
        OboeAudioPlayer::stop();
    }

    int32_t OboeAudioPlayer::play() {
        // Create an AudioStream using the Oboe's builder
        AudioStreamBuilder builder;
        const auto result =
            builder.setPerformanceMode(PerformanceMode::LowLatency)
            // we don't want to record the sound, just play back
            ->setDirection(Direction::Output)
            ->setSampleRate(_samplingRate)
            // pass this instance as the audio callback
            // this ensures that onAudioReady is called at regular intervals
            // to generate audio
            ->setDataCallback(this)
            // no other app should play back sound simultaneously
            ->setSharingMode(SharingMode::Exclusive)
            ->setFormat(AudioFormat::Float)
            ->setChannelCount(channelCount)
            // if the audio device does not support the requested sampling
            // rate natively, it will have to resample the output;
            // the better the resampling quality the larger the workload
            ->setSampleRateConversionQuality(
                SampleRateConversionQuality::Best)
            // open the stream for playback
            ->openStream(_stream);

        if (result != Result::OK) {
            // indicate that stream creation has failed
            return static_cast<int32_t>(result);
        }

        // request a playback start but don't wait for it to actually start
        const auto playResult = _stream->requestStart();

        return static_cast<int32_t>(playResult);
    }

    void OboeAudioPlayer::stop() {
        // if there is an active stream, stop, close, and destroy it
        if (_stream) {
            _stream->stop();
            _stream->close();
            _stream.reset();
        }
        // notify the AudioSource that the playback stopped
        _source->onPlaybackStopped();
    }

    DataCallbackResult
        OboeAudioPlayer::onAudioReady(oboe::AudioStream* audioStream,
            void* audioData,
            int32_t framesCount) {
        // we requested floating-point processing, thus, we treat the given
        // memory block as an array of floats
        // WARNING: the sample format may differ from the requested one.
        // Please, refer to Oboe's documentation for details.
        auto* floatData = reinterpret_cast<float*>(audioData);

        // Let's fill the array with samples.
        // This code works for any number of interleaved channels 
        // and any number of frames.
        for (auto frame = 0; frame < framesCount; ++frame) {
            // retrieve a sample from the AudioSource
            // (in our case, it's a WavetableOscillator)
            const auto sample = _source->getSample();
            // copy the samples to all channels of this frame
            for (auto channel = 0; channel < channelCount; ++channel) {
                floatData[frame * channelCount + channel] = sample;
            }
        }
        // indicate to the Oboe library that the playback should continue
        return oboe::DataCallbackResult::Continue;
    }
}  // namespace wavetablesynthesizer