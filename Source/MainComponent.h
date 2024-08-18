#pragma once

#include <JuceHeader.h>
#include "AudioPlayerComponent.h"

namespace juce_dj
{
    class MainComponent : public juce::AudioAppComponent
    {
    public:
        MainComponent() :
            playerComponent1(formatManager),
            playerComponent2(formatManager)
        {
            setSize(900, 600);

            if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
                && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
            {
                juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
                    [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
            }
            else
            {
                setAudioChannels(2, 2);
            }

            // mp3
            formatManager.registerBasicFormats();

            addAndMakeVisible(playerComponent1);
            addAndMakeVisible(playerComponent2);
        }

        ~MainComponent() override
        {
            shutdownAudio();
        }

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
        {
        }

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
        {
            bufferToFill.clearActiveBufferRegion();
        }

        void releaseResources() override
        {
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::black);
        }

        void resized() override
        {
            auto bounds = getLocalBounds();
            // thumbnail
            bounds.removeFromTop(50);
            // deck1
            playerComponent1.setBounds(bounds.removeFromLeft(400));
            // deck2
            bounds.removeFromLeft(100);
            playerComponent2.setBounds(bounds.removeFromLeft(400));
        }


    private:
        juce::AudioFormatManager formatManager;
        AudioPlayerComponent playerComponent1, playerComponent2;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
    };
}
