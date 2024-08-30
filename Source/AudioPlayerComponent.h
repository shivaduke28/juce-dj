#pragma once

#include "AudioPlayer.h"
#include "WaveformComponent.h"
#include <JuceHeader.h>


namespace juce_dj
{
    class AudioPlayerComponent : public juce::Component, public juce::Timer
    {
    public:
        AudioPlayerComponent(AudioPlayer& audioPlayer,
            juce::AudioFormatManager& formatManager,
            WaveformComponent& waveformComponent
        ) :
            formatManager(formatManager),
            audioPlayer(audioPlayer),
            waveformComponent(waveformComponent)
        {
            setSize(400, 400);
            addAndMakeVisible(openButton);
            openButton.setButtonText("open");
            openButton.onClick = [this] { onOpenButtonClicked(); };

            addAndMakeVisible(playPauseButton);
            playPauseButton.setButtonText("play/pause");
            playPauseButton.onClick = [this] { onPlayPauseButtonClicked(); };

            addAndMakeVisible(stopButton);
            stopButton.setButtonText("stop");
            stopButton.onClick = [this] { onStopButtonClicked(); };

            addAndMakeVisible(titleLabel);
            titleLabel.setText("No loaded", juce::NotificationType::dontSendNotification);

            addAndMakeVisible(tempoFader);
            tempoFader.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
            tempoFader.setRange(-8, 8);
            tempoFader.setValue(0);
            tempoFader.onValueChange = [this] { onTempoFaderChanged(); };

            startTimerHz(40);
        }

        ~AudioPlayerComponent() override
        {
        }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::grey);
        }

        void resized() override
        {
            auto bounds = getLocalBounds();
            openButton.setBounds(bounds.removeFromTop(50));
            titleLabel.setBounds(bounds.removeFromTop(50));
            playPauseButton.setBounds(bounds.removeFromTop(50));
            stopButton.setBounds(bounds.removeFromTop(50));
            tempoFader.setBounds(bounds.removeFromRight(100));
        }

        void timerCallback() override
        {
            // WaveformComponent may have a reference to AudioPlayer instead.
            waveformComponent.setPosition(audioPlayer.getPosition());
        }

    private:
        juce::TextButton openButton, playPauseButton, stopButton;
        juce::Slider tempoFader;
        juce::Label titleLabel;
        std::unique_ptr<juce::FileChooser> chooser;
        juce::AudioFormatManager& formatManager;

        AudioPlayer& audioPlayer;
        WaveformComponent& waveformComponent;

        void onOpenButtonClicked()
        {
            chooser = std::make_unique<juce::FileChooser>("Select a mp3 file to play...",
                juce::File{},
                "*.mp3");

            auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
            chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();

                    if (file != juce::File{})
                    {
                        auto* reader = formatManager.createReaderFor(file);

                        if (reader != nullptr)
                        {
                            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                            titleLabel.setText(file.getFileNameWithoutExtension(), juce::sendNotification);
                            audioPlayer.setSource(newSource.release(), reader->sampleRate);
                            waveformComponent.setSource(new juce::FileInputSource(file));
                        }
                    }
                });
        }

        void onPlayPauseButtonClicked()
        {
            if (audioPlayer.isPlaying())
            {
                audioPlayer.stop();
            }
            else
            {
                audioPlayer.start();
            }
        }

        void onStopButtonClicked()
        {
            audioPlayer.setPosition(0.0);
            audioPlayer.stop();
        }

        void onTempoFaderChanged()
        {
            auto speed = 1.0 + tempoFader.getValue() * 0.01;
            audioPlayer.setSpeed(speed);
            waveformComponent.setSpeed(speed);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayerComponent)
    };
}
