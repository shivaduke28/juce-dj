#pragma once
#include <JuceHeader.h>

namespace juce_dj
{
    class AudioSpectrumComponent : public juce::Component, juce::Timer
    {
    public:
        AudioSpectrumComponent() :
            forwardFFT(fftOrder),
            window(fftSize, juce::dsp::WindowingFunction<float>::hann)
        {
            startTimerHz(60);
        }

        void setNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
        {
            if (bufferToFill.buffer->getNumChannels() > 0)
            {
                auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

                for (auto i = 0; i < bufferToFill.numSamples; ++i)
                    pushNextSampleIntoFifo(channelData[i]);
            }
        }

        // Set sample to fifo from audio thread.
        // If fifo is full, copy fifo to ffftData and set ready flag on.
        void pushNextSampleIntoFifo(float sample) noexcept
        {
            if (fifoIndex == fftSize)
            {
                if (!nextFFTBlockReady)
                {
                    juce::zeromem(fftData, sizeof(fftData));
                    memcpy(fftData, fifo, sizeof(fifo));
                    nextFFTBlockReady = true;
                }

                fifoIndex = 0;
            }

            fifo[fifoIndex++] = sample;
        }


        void timerCallback() override
        {
            if (nextFFTBlockReady)
            {
                drawNextFrameOfSpectrum();
                nextFFTBlockReady = false;
                repaint();
            }
        }

        void drawNextFrameOfSpectrum()
        {
            // first apply a windowing function to our data
            window.multiplyWithWindowingTable(fftData, fftSize);

            // then render our FFT data..
            forwardFFT.performFrequencyOnlyForwardTransform(fftData);

            auto mindB = -100.0f;
            auto maxdB = 0.0f;

            // TODO: modify (x,y)->(log10(x),y) here instead paint...
            for (int i = 0; i < scopeSize; ++i)
            {
                auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
                auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
                auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftData[fftDataIndex])
                    - juce::Decibels::gainToDecibels((float)fftSize)),
                    mindB, maxdB, 0.0f, 1.0f);

                scopeData[i] = level;
            }
        }

        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colours::white);

            for (int i = 1; i < scopeSize; ++i)
            {
                auto width = getLocalBounds().getWidth();
                auto height = getLocalBounds().getHeight();
                float x = i == 1 ? 0 : std::log10f(i - 1);
                float x2 = std::log10f(i);
                float xmax = std::log10f(scopeSize - 1);

                g.drawLine({ juce::jmap(x, 0.0f, xmax, 0.0f, (float)width),
                             juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                             juce::jmap(x2, 0.0f, xmax, 0.0f, (float)width),
                             juce::jmap(scopeData[i], 0.0f, 1.0f, (float)height, 0.0f) });
            }
        }


    private:
        static constexpr auto fftOrder = 11;
        static constexpr auto fftSize = 1 << fftOrder;
        static constexpr auto scopeSize = 512;

        juce::dsp::FFT forwardFFT;
        juce::dsp::WindowingFunction<float> window;

        float fifo[fftSize];
        float fftData[2 * fftSize];
        int fifoIndex = 0;
        bool nextFFTBlockReady = false;
        float scopeData[scopeSize];

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSpectrumComponent)
    };
}