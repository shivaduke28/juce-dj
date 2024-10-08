#pragma once

#include "AudioMixer.h"
#include <JuceHeader.h>


namespace juce_dj
{
    class AudioPlayer
    {
    public:
        AudioPlayer(AudioMixer& mixer, AudioMixer::Channel channel) :
            resamplingSource(&transportSource, false, 2),
            mixer(mixer)
        {
            mixer.add(&resamplingSource, channel);
        }

        ~AudioPlayer()
        {
        }

        void setSource(juce::PositionableAudioSource* source, double sampleRate)
        {
            transportSource.setSource(source, 0, nullptr, sampleRate);
            readerSource.reset(source);
        }

        void start()
        {
            transportSource.start();
        }

        void stop()
        {
            transportSource.stop();
        }

        double getPosition()
        {
            return transportSource.getCurrentPosition();
        }

        void setPosition(double position)
        {
            transportSource.setPosition(position);
        }

        void setSpeed(double speed)
        {
            resamplingSource.setResamplingRatio(speed);
        }

        bool isPlaying()
        {
            return transportSource.isPlaying();
        }

    private:
        std::unique_ptr<juce::PositionableAudioSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resamplingSource;
        AudioMixer& mixer;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayer)
    };
}