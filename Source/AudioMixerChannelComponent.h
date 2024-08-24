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
            channelFader.onValueChange = [this] {onChannelFaderChanged(); };

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

            trimKnob.setRange(-0.9999, 0.9999);
            lowKnob.setRange(-0.9999, 0.9999);
            midKnob.setRange(-0.9999, 0.9999);
            highKnob.setRange(-0.9999, 0.9999);

            trimKnob.setValue(0, juce::NotificationType::dontSendNotification);
            lowKnob.setValue(0, juce::NotificationType::dontSendNotification);
            midKnob.setValue(0, juce::NotificationType::dontSendNotification);
            highKnob.setValue(0, juce::NotificationType::dontSendNotification);

            lowKnob.onValueChange = [this] {onlowChanged(); };
            midKnob.onValueChange = [this] {onMidChanged(); };
            highKnob.onValueChange = [this] {onHighChanged(); };
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


        void onChannelFaderChanged()
        {
            audioMixer.setGain(channel, channelFader.getValue());
        }

        void onlowChanged()
        {
            audioMixer.setEqLow(channel, lowKnob.getValue());
        }

        void onMidChanged()
        {
            audioMixer.setEqMid(channel, midKnob.getValue());
        }

        void onHighChanged()
        {
            audioMixer.setEqHigh(channel, highKnob.getValue());
        }

    };
}