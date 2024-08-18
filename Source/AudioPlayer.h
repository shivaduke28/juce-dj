#pragma once

#include <JuceHeader.h>

namespace juce_dj
{
    class AudioPlayer
    {
    public:
        AudioPlayer() :
            resamplingSource(&transportSource, false, 2)
        {

        }

        ~AudioPlayer()
        {
        }

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
        {
            resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        }

        void releaseResources()
        {
            transportSource.releaseResources();
            resamplingSource.releaseResources();
        }

        void setSource(juce::PositionableAudioSource* source, double sampleRate)
        {
            readerSource.reset(source);
            transportSource.setSource(source, 0, nullptr, sampleRate);
        }

        void start()
        {
            transportSource.start();
        }

        void stop()
        {
            transportSource.stop();
        }

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
        {
            resamplingSource.getNextAudioBlock(bufferToFill);
        }

    private:
        std::unique_ptr<juce::PositionableAudioSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resamplingSource;
    };
}