/*!
 * \file       rocky_Plugin.cpp
 * Copyright   Eiosis 2014
 */

#include "rocky/rocky_Plugin.h"
#include "rocky/rocky_Editor.h"
#include "rocky/rocky_Engine.h"

namespace rocky
{
    RockyParametersInfo::RockyParametersInfo()
    {
        registerInfo(paramInGain, "Input Gain", 0.f,
                     new parameters::LinearParameterTaper(-24.f, +24.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(RockyPorts, mInputGain), sizeof(dsp::Gain::Port));
        registerInfo(paramHPFrequency, "High Pass Frequency", 1000.f,
                     new parameters::LogParameterTaper(1.f, 25000.f, 1000.f, .5f),
                     new parameters::ValueSuffixDisplayDelegate("Hz", 2, false),
                     offsetof(RockyPorts, mHP), sizeof(dsp::IIR::Port));
        registerInfo(paramHPQ, "High Pass Q", 1.f,
                     new parameters::LinearParameterTaper(.1f, 12.f),
                     new parameters::ValueSuffixDisplayDelegate("", 2, false),
                     offsetof(RockyPorts, mHP), sizeof(dsp::IIR::Port));
        registerInfo(paramLSFrequency, "Low Shelf Frequency", 1000.f,
                     new parameters::LogParameterTaper(10.f, 25000.f, 1000.f, .5f),
                     new parameters::ValueSuffixDisplayDelegate("Hz", 2, false),
                     offsetof(RockyPorts, mLS), sizeof(dsp::IIR::Port));
        registerInfo(paramLSQ, "Low Shelf Q", 1.f,
                     new parameters::LinearParameterTaper(.35f, 12.f),
                     new parameters::ValueSuffixDisplayDelegate("", 2, false),
                     offsetof(RockyPorts, mLS), sizeof(dsp::IIR::Port));
        registerInfo(paramLSGain, "Low Shelf Gain", 0.f,
                     new parameters::LinearParameterTaper(-24.f, +24.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 4, true),
                     offsetof(RockyPorts, mLS), sizeof(dsp::IIR::Port));
        registerInfo(paramBell1Frequency, "Bell 1 Frequency", 1000.f,
                     new parameters::LogParameterTaper(10.f, 25000.f, 1000.f, .5f),
                     new parameters::ValueSuffixDisplayDelegate("Hz", 2, false),
                     offsetof(RockyPorts, mBell1), sizeof(dsp::IIR::Port));
        registerInfo(paramBell1Q, "Bell 1 Q", 1.f,
                     new parameters::LinearParameterTaper(.1f, 12.f),
                     new parameters::ValueSuffixDisplayDelegate("", 2, false),
                     offsetof(RockyPorts, mBell1), sizeof(dsp::IIR::Port));
        registerInfo(paramBell1Gain, "Bell 1 Gain", 0.f,
                     new parameters::LinearParameterTaper(-24.f, +24.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(RockyPorts, mBell1), sizeof(dsp::IIR::Port));
        registerInfo(paramBell2Frequency, "Bell 2 Frequency", 1000.f,
                     new parameters::LogParameterTaper(10.f, 25000.f, 1000.f, .5f),
                     new parameters::ValueSuffixDisplayDelegate("Hz", 2, false),
                     offsetof(RockyPorts, mBell2), sizeof(dsp::IIR::Port));
        registerInfo(paramBell2Q, "Bell 2 Q", 1.f,
                     new parameters::LinearParameterTaper(.1f, 12.f),
                     new parameters::ValueSuffixDisplayDelegate("", 2, false),
                     offsetof(RockyPorts, mBell2), sizeof(dsp::IIR::Port));
        registerInfo(paramBell2Gain, "Bell 2 Gain", 0.f,
                     new parameters::LinearParameterTaper(-24.f, +24.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(RockyPorts, mBell2), sizeof(dsp::IIR::Port));
        registerInfo(paramHSFrequency, "High Shelf Frequency", 1000.f,
                     new parameters::LogParameterTaper(10.f, 25000.f, 1000.f, .5f),
                     new parameters::ValueSuffixDisplayDelegate("Hz", 2, false),
                     offsetof(RockyPorts, mHS), sizeof(dsp::IIR::Port));
        registerInfo(paramHSQ, "High Shelf Q", 1.f,
                     new parameters::LinearParameterTaper(.35f, 12.f),
                     new parameters::ValueSuffixDisplayDelegate("", 2, false),
                     offsetof(RockyPorts, mHS), sizeof(dsp::IIR::Port));
        registerInfo(paramHSGain, "High Shelf Gain", 0.f,
                     new parameters::LinearParameterTaper(-24.f, +24.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(RockyPorts, mHS), sizeof(dsp::IIR::Port));
        registerInfo(paramLPFrequency, "Low Pass Frequency", 1000.f,
                     new parameters::LogParameterTaper(10.f, 25000.f, 1000.f, .5f),
                     new parameters::ValueSuffixDisplayDelegate("Hz", 2, false),
                     offsetof(RockyPorts, mLP), sizeof(dsp::IIR::Port));
        registerInfo(paramLPQ, "Low Pass Q", 1.f,
                     new parameters::LinearParameterTaper(.1f, 12.f),
                     new parameters::ValueSuffixDisplayDelegate("", 2, false),
                     offsetof(RockyPorts, mLP), sizeof(dsp::IIR::Port));
        registerInfo(paramOutGain, "Output Gain", 0.f,
                     new parameters::LinearParameterTaper(-24.f, +24.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(RockyPorts, mOutputGain), sizeof(dsp::Gain::Port));
    }

    // -------------------------------------------------------------------------

    RockyProcessor::RockyProcessor()
        : plugin::Processor<numParameters, RockyPorts, RockyState, RockyProcessor>(gParametersInfo,
                                                                                   "Rocky", true)
    {
        setMapper(paramInGain,          &RockyProcessor::mapInputGain);
        setMapper(paramHPFrequency,     &RockyProcessor::mapHP);
        setMapper(paramHPQ,             &RockyProcessor::mapHP);
        setMapper(paramLSFrequency,     &RockyProcessor::mapLS);
        setMapper(paramLSQ,             &RockyProcessor::mapLS);
        setMapper(paramLSGain,          &RockyProcessor::mapLS);
        setMapper(paramBell1Frequency,  &RockyProcessor::mapBell1);
        setMapper(paramBell1Q,          &RockyProcessor::mapBell1);
        setMapper(paramBell1Gain,       &RockyProcessor::mapBell1);
        setMapper(paramBell2Frequency,  &RockyProcessor::mapBell2);
        setMapper(paramBell2Q,          &RockyProcessor::mapBell2);
        setMapper(paramBell2Gain,       &RockyProcessor::mapBell2);
        setMapper(paramHSFrequency,     &RockyProcessor::mapHS);
        setMapper(paramHSQ,             &RockyProcessor::mapHS);
        setMapper(paramHSGain,          &RockyProcessor::mapHS);
        setMapper(paramLPFrequency,     &RockyProcessor::mapLP);
        setMapper(paramLPQ,             &RockyProcessor::mapLP);
        setMapper(paramOutGain,         &RockyProcessor::mapOutputGain);
    }

    RockyProcessor::~RockyProcessor()
    {

    }

    // -------------------------------------------------------------------------

    juce::AudioProcessorEditor* RockyProcessor::createEditor()
    {
        return new RockyEditor(this);
    }

    // -------------------------------------------------------------------------

    void RockyProcessor::savePresetTo(const juce::File& inFile)
    {
		RockyPresetSaver saver(*this, inFile);
		saver();
    }

    void RockyProcessor::loadPresetFrom(const juce::File& inFile)
    {
        RockyPresetLoader loader(*this, inFile);
        loader();
    }

    // -------------------------------------------------------------------------

    void RockyProcessor::mapInputGain(void* outPortData)
    {
        dsp::Gain::Port& gainPort = *reinterpret_cast<dsp::Gain::Port*>(outPortData);
        gainPort.mTargetGain = dsp::dBToLinear(getParameterPlain(paramInGain));
    }

    void RockyProcessor::mapHP(void* outPortData)
    {
        const dsp::float32 frequency  = getParameterPlain(paramHPFrequency);
        const dsp::float32 q          = getParameterPlain(paramHPQ);

        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && q > 0.);

        const dsp::float32 clippedFreq      = std::min(frequency, .5f * samplerate - 100.f);
        const dsp::float32 w0               = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 alpha            = std::sin(w0) / (2.f * q);
        const dsp::float32 ib0              = 1.f / (1.f + alpha);

        dsp::IIR::Port& iirPort             = *reinterpret_cast<dsp::IIR::Port*>(outPortData);
        iirPort.mCoefficients[0]            = ib0 * (.5f * (1.f + std::cos(w0)));
        iirPort.mCoefficients[1]            = ib0 * (-(1.f + std::cos(w0)));
        iirPort.mCoefficients[2]            = ib0 * (.5f * (1.f + std::cos(w0)));
        iirPort.mCoefficients[3]            = 1.;
        iirPort.mCoefficients[4]            = ib0 * ( -2.f * std::cos(w0));
        iirPort.mCoefficients[5]            = ib0 * (1.f - alpha);
    }

    void RockyProcessor::mapLS(void* outPortData)
    {
        const dsp::float32 frequency  = getParameterPlain(paramLSFrequency);
        const dsp::float32 q          = getParameterPlain(paramLSQ);
        const dsp::float32 gain       = getParameterPlain(paramLSGain);

        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && q > 0.);

        const dsp::float32 clippedFreq      = std::min(frequency, .5f * samplerate - 100.f);
        const dsp::float32 a                = std::pow(10.f, gain / 40.f);
        const dsp::float32 aMinus1          = a - 1.f;
        const dsp::float32 aPlus1           = a + 1.f;
        const dsp::float32 w                = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 cosW             = std::cos(w);
        const dsp::float32 beta             = std::sin(w) * std::sqrt(a) / q;
        const dsp::float32 aMinus1TimesCosW = aMinus1 * cosW;
        const dsp::float32 ib0              = 1.f / (aPlus1 + aMinus1TimesCosW + beta);

        dsp::IIR::Port& iirPort             = *reinterpret_cast<dsp::IIR::Port*>(outPortData);
        iirPort.mCoefficients[0]            = ib0 * (a * (aPlus1 - aMinus1TimesCosW + beta));
        iirPort.mCoefficients[1]            = ib0 * (a * 2.f * (aMinus1 - aPlus1 * cosW));
        iirPort.mCoefficients[2]            = ib0 * (a * (aPlus1 - aMinus1TimesCosW - beta));
        iirPort.mCoefficients[3]            = 1.f;
        iirPort.mCoefficients[4]            = ib0 * (-2.f * (aMinus1 + aPlus1 * cosW));
        iirPort.mCoefficients[5]            = ib0 * (aPlus1 + aMinus1TimesCosW - beta);
    }

    void RockyProcessor::mapBell1(void* outPortData)
    {
        const dsp::float32 frequency  = getParameterPlain(paramBell1Frequency);
        const dsp::float32 q          = getParameterPlain(paramBell2Q);
        const dsp::float32 gain       = getParameterPlain(paramBell1Gain);
        internalMapBell(outPortData, frequency, q, gain);
    }

    void RockyProcessor::mapBell2(void* outPortData)
    {
        const dsp::float32 frequency  = getParameterPlain(paramBell2Frequency);
        const dsp::float32 q          = getParameterPlain(paramBell2Q);
        const dsp::float32 gain       = getParameterPlain(paramBell2Gain);
        internalMapBell(outPortData, frequency, q, gain);
    }

    inline void RockyProcessor::internalMapBell(void* outPortData, float inFrequency,
                                                float inQ, float inGain)
    {
        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && inQ > 0.);

        const dsp::float32 a                = std::pow(10.f, inGain / 40.f);
        const dsp::float32 w                = (dsp::twoPi_32 * std::min(inFrequency, .5f * samplerate - 100.f)) / samplerate;
        const dsp::float32 alpha            = .5f * std::sin(w) / inQ;
        const dsp::float32 c2               = -2.f * std::cos(w);
        const dsp::float32 alphaTimesA      = alpha * a;
        const dsp::float32 alpheOverA       = alpha / a;
        const dsp::float32 ib0              = 1.f / (1.f + alpheOverA);

        dsp::IIR::Port& iirPort             = *reinterpret_cast<dsp::IIR::Port*>(outPortData);
        iirPort.mCoefficients[0]            = ib0 * (1.f + alphaTimesA);
        iirPort.mCoefficients[1]            = ib0 * c2;
        iirPort.mCoefficients[2]            = ib0 * (1.f - alphaTimesA);
        iirPort.mCoefficients[3]            = 1.f;
        iirPort.mCoefficients[4]            = ib0 * c2;
        iirPort.mCoefficients[5]            = ib0 * (1.f - alpheOverA);
    }

    void RockyProcessor::mapHS(void* outPortData)
    {
        const dsp::float32 frequency  = getParameterPlain(paramHSFrequency);
        const dsp::float32 q          = getParameterPlain(paramHSQ);
        const dsp::float32 gain       = getParameterPlain(paramHSGain);

        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && q > 0.);

        const dsp::float32 clippedFreq      = frequency;
        const dsp::float32 a                = std::pow(10.f, gain / 40.f);
        const dsp::float32 aMinus1          = a - 1.f;
        const dsp::float32 aPlus1           = a + 1.f;
        const dsp::float32 w                = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 cosW             = std::cos(w);
        const dsp::float32 beta             = std::sin(w) * std::sqrt(a) / q;
        const dsp::float32 aMinus1TimesCosW = aMinus1 * cosW;
        const dsp::float32 ib0              = 1.f / (aPlus1 - aMinus1TimesCosW + beta);

        dsp::IIR::Port& iirPort             = *reinterpret_cast<dsp::IIR::Port*>(outPortData);
        iirPort.mCoefficients[0]            = ib0 * (a * (aPlus1 + aMinus1TimesCosW + beta));
        iirPort.mCoefficients[1]            = ib0 * (a * -2.f * (aMinus1 + aPlus1 * cosW));
        iirPort.mCoefficients[2]            = ib0 * (a * (aPlus1 + aMinus1TimesCosW - beta));
        iirPort.mCoefficients[3]            = 1.f;
        iirPort.mCoefficients[4]            = ib0 * (2.f * (aMinus1 - aPlus1 * cosW));
        iirPort.mCoefficients[5]            = ib0 * (aPlus1 - aMinus1TimesCosW - beta);
    }

    void RockyProcessor::mapLP(void* outPortData)
    {
        const dsp::float32 frequency  = getParameterPlain(paramLPFrequency);
        const dsp::float32 q          = getParameterPlain(paramLPQ);

        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && q > 0.);

        const dsp::float32 clippedFreq      = std::min(frequency, .5f * samplerate - 1e-6f);
        const dsp::float32 w0               = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 alpha            = std::sin(w0) / (2.f * q);
        const dsp::float32 ib0              = 1.f / (1.f + alpha);

        dsp::IIR::Port& iirPort             = *reinterpret_cast<dsp::IIR::Port*>(outPortData);
        iirPort.mCoefficients[0]            = ib0 * (.5f * (1.f - std::cos(w0)));
        iirPort.mCoefficients[1]            = ib0 * ((1.f - std::cos(w0)));
        iirPort.mCoefficients[2]            = ib0 * (.5f * (1.f - std::cos(w0)));
        iirPort.mCoefficients[3]            = 1.f;
        iirPort.mCoefficients[4]            = ib0 * (-2.f * std::cos(w0));
        iirPort.mCoefficients[5]            = ib0 * (1.f - alpha);
    }

    void RockyProcessor::mapOutputGain(void* outPortData)
    {
        dsp::Gain::Port& gainPort = *reinterpret_cast<dsp::Gain::Port*>(outPortData);
        gainPort.mTargetGain = dsp::dBToLinear(getParameterPlain(paramOutGain));
    }
}

// -----------------------------------------------------------------------------

REGISTER_PLUGIN_FILTER(rocky::RockyProcessor)

