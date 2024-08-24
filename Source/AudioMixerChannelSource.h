#pragma once
#include <JuceHeader.h>

namespace juce_dj
{
    // audio source for a single mixer channel used in AudioMixer class
    class AudioMixerChannelSource : public juce::AudioSource
    {
    public:
        using StereoFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;

        AudioMixerChannelSource()
        {
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
            lowShelf.prepare(spec);
            midPeak.prepare(spec);
            highShelf.prepare(spec);

            *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 10000.0f, 0.707f, 1.0f);
            *midPeak.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 1000.0f, 0.707f, 1.0f);
            *lowShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(cachedSampleRate, 300.f, 0.707f, 1.0f);

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
            lowShelf.process(context);
            midPeak.process(context);
            highShelf.process(context);
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

        void setEqLow(double value)
        {
            *lowShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(cachedSampleRate, 300.f, 0.707f, 1.0f + value);
        }

        void setEqMid(double value)
        {
            *midPeak.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(cachedSampleRate, 1000.0f, 0.707f, 1.0f + value);
        }

        void setEqHigh(double value)
        {
            *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(cachedSampleRate, 10000.0f, 0.707f, 1.0f + value);
        }

    private:
        juce::AudioSource* inputSource = nullptr;

        juce::dsp::Gain<float> gainProcessor;

        StereoFilter lowShelf;
        StereoFilter midPeak;
        StereoFilter highShelf;

        double gain = 0;
        int cachedSamplesPerBlockExpected = 100;
        double cachedSampleRate = 44100;
        bool prepared = false;
    };
}