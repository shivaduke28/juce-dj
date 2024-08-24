#pragma once

#include <JuceHeader.h>
#include "AudioMixerChannelSource.h"

namespace juce_dj
{
    class AudioMixer
    {
    public:
        AudioMixer()
        {
            mixerSource.addInputSource(&channelSource1, false);
            mixerSource.addInputSource(&channelSource2, false);
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
            getChannelSource(channel).setSource(source);
        }

        void setGain(Channel channel, double gain)
        {
            getChannelSource(channel).setGain(gain);
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
        AudioMixerChannelSource& getChannelSource(Channel channel)
        {
            if (channel == One)
            {
                return  channelSource1;
            }
            else if (channel == Two)
            {
                return  channelSource2;
            }

        }


        juce::MixerAudioSource mixerSource;
        juce::AudioSource* source1 = nullptr;
        juce::AudioSource* source2 = nullptr;
        AudioMixerChannelSource channelSource1;
        AudioMixerChannelSource channelSource2;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioMixer)
    };
}