#pragma once
#include <JuceHeader.h>
#include "AudioMixer.h"

namespace juce_dj
{

    class AudioMixerComponent : public juce::Component {
    public:
        AudioMixerComponent(AudioMixer& audioMixer) : audioMixer(audioMixer) {

            addAndMakeVisible(channelFader1);
            addAndMakeVisible(channelFader2);

            channelFader1.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
            channelFader2.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

            channelFader1.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            channelFader2.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

            channelFader1.setRange(0, 4);
            channelFader2.setRange(0, 4);

            channelFader1.onValueChange = [this] {onChannelFaderChanged(AudioMixer::One, channelFader1.getValue()); };
            channelFader2.onValueChange = [this] {onChannelFaderChanged(AudioMixer::Two, channelFader2.getValue()); };
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::grey);
        }

        void resized() override
        {
            auto bounds = getLocalBounds();
            channelFader1.setBounds(0, 0, 50, 200);
            channelFader2.setBounds(50, 0, 50, 200);
        }

    private:
        AudioMixer& audioMixer;
        juce::Slider channelFader1;
        juce::Slider channelFader2;

        void onChannelFaderChanged(AudioMixer::Channel channel, double gain)
        {
            audioMixer.setGain(channel, gain);
        }
    };
}