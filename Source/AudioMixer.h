#pragma once

#include <JuceHeader.h>

namespace juce_dj
{
    class AudioMixer
    {
    public:
        AudioMixer()
        {

        }

        ~AudioMixer()
        {

        }

        enum Channel
        {
            One,
            Two,
        };

        void add(juce::AudioSource* source, Channel channel)
        {
            if (channel == One)
            {
                if (source1 != nullptr)
                {
                    mixerSource.removeInputSource(source1);
                }
                source1 = source;
                mixerSource.addInputSource(source, false);
            }
            else if (channel == Two)
            {
                if (source2 != nullptr)
                {
                    mixerSource.removeInputSource(source2);
                }
                source2 = source;
                mixerSource.addInputSource(source, false);
            }
        }

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate)
        {
            mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
        }

        void releaseResources()
        {
            mixerSource.releaseResources();
        }

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
        {
            mixerSource.getNextAudioBlock(bufferToFill);
        }

    private:
        juce::MixerAudioSource mixerSource;
        juce::AudioSource* source1 = nullptr;
        juce::AudioSource* source2 = nullptr;
    };
}