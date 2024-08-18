#pragma once

#include <JuceHeader.h>

namespace juce_dj
{
    // draw waveform of a single channel
    class WaveformComponent : public juce::Component, public juce::ChangeListener
    {
    public:
        WaveformComponent(juce::AudioFormatManager& formatManager) :
            thumbnailCache(5),
            thumbnail(256, formatManager, thumbnailCache),
            currentPosition(0.0),
            currentSpeed(1.0)
        {
            thumbnail.addChangeListener(this);
        }

        ~WaveformComponent()
        {

        }

        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds();
            if (thumbnail.getTotalLength() > 0.0)
            {
                g.setColour(juce::Colours::darkgrey);
                double offset = 4 * currentSpeed;
                thumbnail.drawChannels(g, bounds, currentPosition - offset, currentPosition + offset, 1);
            }

            auto centerX = bounds.getCentreX();
            auto centerY = bounds.getCentreY();
            auto height = bounds.getHeight();

            g.setColour(juce::Colours::white);
            g.drawLine(centerX, centerY + height * 0.5, centerX, centerY - height * 0.5);
        }

        // set from audio player component
        // take ownership of pointer
        void setSource(juce::InputSource* const source)
        {
            thumbnail.setSource(source);
        }

        void setPosition(double position)
        {
            if (position != currentPosition)
            {
                currentPosition = position;
                repaint();
            }
        }

        void setSpeed(double speed)
        {
            currentSpeed = speed;
        }

        void changeListenerCallback(juce::ChangeBroadcaster* source) override
        {
            if (source == &thumbnail)
            {
                repaint();
            }
        }

    private:
        juce::AudioThumbnailCache thumbnailCache;
        juce::AudioThumbnail thumbnail;
        double currentPosition;
        double currentSpeed;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformComponent)
    };
}
