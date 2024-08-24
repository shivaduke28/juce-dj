#pragma once
#include <JuceHeader.h>
#include "AudioMixer.h"
#include "AudioMixerChannelComponent.h"

namespace juce_dj
{

    class AudioMixerComponent : public juce::Component {
    public:
        AudioMixerComponent(AudioMixer& audioMixer) :
            audioMixer(audioMixer),
            channelComponent1(audioMixer, AudioMixer::One),
            channelComponent2(audioMixer, AudioMixer::Two)
        {

            addAndMakeVisible(channelComponent1);
            addAndMakeVisible(channelComponent2);
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::grey);
        }

        void resized() override
        {
            channelComponent1.setBounds(0, 0, 50, 400);
            channelComponent2.setBounds(50, 0, 50, 400);
        }

    private:
        AudioMixer& audioMixer;
        AudioMixerChannelComponent channelComponent1, channelComponent2;
    };
}