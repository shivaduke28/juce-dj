#pragma once
#include <JuceHeader.h>
#include "AudioMixer.h"

namespace juce_dj
{
    class AudioMixerChannelComponent : public juce::Component
    {
    public:
        AudioMixerChannelComponent(AudioMixer& audioMixer, AudioMixer::Channel channel) :
            audioMixer(audioMixer),
            channel(channel)
        {

            addAndMakeVisible(channelFader);
            channelFader.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
            channelFader.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            channelFader.setRange(0, 4);

            addAndMakeVisible(trimKnob);
            addAndMakeVisible(lowKnob);
            addAndMakeVisible(midKnob);
            addAndMakeVisible(highKnob);

            trimKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
            lowKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
            midKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
            highKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

            trimKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            lowKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            midKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            highKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

            trimKnob.setRange(-1, 1);
            lowKnob.setRange(-1, 1);
            midKnob.setRange(-1, 1);
            highKnob.setRange(-1, 1);

            trimKnob.setValue(0, juce::NotificationType::dontSendNotification);
            lowKnob.setValue(0, juce::NotificationType::dontSendNotification);
            midKnob.setValue(0, juce::NotificationType::dontSendNotification);
            highKnob.setValue(0, juce::NotificationType::dontSendNotification);
        }

        void resized() override
        {
            // min = (50, 300)
            auto bounds = getLocalBounds();
            trimKnob.setBounds(bounds.removeFromTop(50));
            highKnob.setBounds(bounds.removeFromTop(50));
            midKnob.setBounds(bounds.removeFromTop(50));
            lowKnob.setBounds(bounds.removeFromTop(50));
            channelFader.setBounds(bounds.removeFromTop(100));
        }

    private:
        AudioMixer& audioMixer;
        AudioMixer::Channel channel;

        juce::Slider channelFader, lowKnob, midKnob, highKnob, trimKnob;
    };
}