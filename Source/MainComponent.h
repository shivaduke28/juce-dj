#pragma once

#include <JuceHeader.h>
#include "AudioMixer.h"
#include "AudioPlayer.h"
#include "AudioPlayerComponent.h"
#include "WaveformComponent.h"
#include "AudioMixerComponent.h"
#include "AudioSpectrumComponent.h"

namespace juce_dj
{
    class MainComponent : public juce::AudioAppComponent
    {
    public:
        MainComponent() :
            player1(mixer, AudioMixer::One),
            player2(mixer, AudioMixer::Two),
            waveformComponent1(formatManager),
            waveformComponent2(formatManager),
            playerComponent1(player1, formatManager, waveformComponent1),
            playerComponent2(player2, formatManager, waveformComponent2),
            audioMixerComponent(mixer)
        {
            setSize(900, 800);

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
            addAndMakeVisible(waveformComponent1);
            addAndMakeVisible(waveformComponent2);
            addAndMakeVisible(audioMixerComponent);
            addAndMakeVisible(audioSpectrumComponent);
        }

        ~MainComponent() override
        {
            shutdownAudio();
        }

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
        {
            mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
        }

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
        {
            mixer.getNextAudioBlock(bufferToFill);
            audioSpectrumComponent.setNextAudioBlock(bufferToFill);
        }

        void releaseResources() override
        {
            mixer.releaseResources();
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::black);
        }

        void resized() override
        {
            auto bounds = getLocalBounds();
            // thumbnail
            waveformComponent1.setBounds(bounds.removeFromTop(50));
            waveformComponent2.setBounds(bounds.removeFromTop(50));
            audioSpectrumComponent.setBounds(bounds.removeFromTop(200));
            // deck1
            playerComponent1.setBounds(bounds.removeFromLeft(400));
            // mixer
            audioMixerComponent.setBounds(bounds.removeFromLeft(100));
            // deck2
            playerComponent2.setBounds(bounds.removeFromLeft(400));
        }


    private:
        juce::AudioFormatManager formatManager;
        AudioMixer mixer;
        AudioPlayer player1, player2;
        AudioPlayerComponent playerComponent1, playerComponent2;
        WaveformComponent waveformComponent1;
        WaveformComponent waveformComponent2;
        AudioMixerComponent audioMixerComponent;
        AudioSpectrumComponent audioSpectrumComponent;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
    };
}
