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

            juce::dsp::ProcessSpec spec;
            spec.sampleRate = sampleRate;
            spec.maximumBlockSize = samplesPerBlockExpected;
            spec.numChannels = 2;
            gainProcessor.prepare(spec);

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

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& info) override
        {
            if (inputSource == nullptr) return;
            inputSource->getNextAudioBlock(info);

            juce::dsp::AudioBlock<float> audioBlock(*info.buffer);
            juce::dsp::ProcessContextReplacing<float> context(audioBlock);

            gainProcessor.setGainLinear(gain);
            gainProcessor.setRampDurationSeconds(info.numSamples / cachedSampleRate);
            gainProcessor.process(context);
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

        juce::dsp::Gain<float> gainProcessor;
        double gain = 0;
        int cachedSamplesPerBlockExpected = 100;
        double cachedSampleRate = 44100;
        bool prepared = false;
    };
}