#pragma once

#include <JuceHeader.h>

namespace juce_dj
{
    class AudioPlayerComponent : public juce::Component
    {
    public:
        AudioPlayerComponent(juce::AudioFormatManager& formatManager) : formatManager(formatManager)
        {
            setSize(400, 400);
            addAndMakeVisible(openButton);
            openButton.setButtonText("open");
            openButton.onClick = [this] { onOpenButtonClicked(); };

            addAndMakeVisible(playPauseButton);
            playPauseButton.setButtonText("play/pause");
            addAndMakeVisible(stopButton);
            stopButton.setButtonText("stop");

            addAndMakeVisible(titleLabel);
            titleLabel.setText("No loaded", juce::NotificationType::dontSendNotification);
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
        }

    private:
        juce::TextButton openButton, playPauseButton, stopButton;
        juce::Label titleLabel;
        std::unique_ptr<juce::FileChooser> chooser;
        juce::AudioFormatManager& formatManager;

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
                            // todo: audioPlayer.setSource(newSource.release());
                            newSource.reset(); // todo: delete this
                        }
                    }
                });
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPlayerComponent)
    };
}
