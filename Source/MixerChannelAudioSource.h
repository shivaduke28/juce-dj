#pragma once
#include <JuceHeader.h>

namespace juce_dj
{
    // audio source for a single mixer channel used in AudioMixer class
    class MixerChannelAudioSource : public juce::AudioSource
    {
    public:
        MixerChannelAudioSource() {

        }

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
        {
            cachedSampleRate = sampleRate;
            cachedSamplesPerBlockExpected = cachedSamplesPerBlockExpected;
            prepared = true;

            if (inputSource != nullptr)
            {
                inputSource->prepareToPlay(cachedSamplesPerBlockExpected, cachedSampleRate);
            }
        }

        void releaseResources() override
        {
            if (inputSource != nullptr)
            {
                inputSource->releaseResources();
            }
        }

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
        {
            if (inputSource == nullptr) return;
            inputSource->getNextAudioBlock(bufferToFill);

            // todo: avoid noise when gain changed...
            bufferToFill.buffer->applyGain(gain);
        }

        void setSource(juce::AudioSource* source)
        {
            inputSource = source;
            if (inputSource != nullptr && prepared)
            {
                inputSource->prepareToPlay(cachedSamplesPerBlockExpected, cachedSampleRate);
            }
        }

        void setGain(double newGain)
        {
            gain = newGain;
        }

    private:
        juce::AudioSource* inputSource = nullptr;

        double gain = 0;
        int cachedSamplesPerBlockExpected = 100;
        double cachedSampleRate = 44100;
        bool prepared = false;
    };
}