/*!
 * \file       filter_Plugin.cpp
 * Copyright   Eiosis 2014
 */

#include "filter/filter_Plugin.h"
#include "filter/filter_Editor.h"

namespace filter
{
    const char* FilterTypes::sNames[numFilters] =
    {
        "Bell", "Low Shelf", "High Shelf", "High Pass", "Low Pass",
    };

    // -------------------------------------------------------------------------

    FilterParametersInfo::FilterParametersInfo()
    {
        registerInfo(paramInGain, "Input Gain", 0.f,
                     new parameters::LinearParameterTaper(-18.f, +18.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(FilterPorts, mInputGain), sizeof(dsp::Gain::Port));
        registerInfo(paramFilterType, "Filter Type", FilterTypes::bell,
                     new parameters::EnumeratedParameterTaper(FilterTypes::numFilters),
                     new parameters::EnumeratedDisplayDelegate(FilterTypes::sNames,
                                                               FilterTypes::numFilters),
                     offsetof(FilterPorts, mIIR), sizeof(dsp::IIR::Port));
        registerInfo(paramFrequency, "Frequency", 1000.f,
                     new parameters::LogParameterTaper(10.f, 24000.f, 1000.f, .5f),
                     new parameters::ValueSuffixDisplayDelegate("Hz", 2, false),
                     offsetof(FilterPorts, mIIR), sizeof(dsp::IIR::Port));
        registerInfo(paramQ, "Q", 1.f,
                     new parameters::LinearParameterTaper(.1f, 7.f),
                     new parameters::ValueSuffixDisplayDelegate("", 2, false),
                     offsetof(FilterPorts, mIIR), sizeof(dsp::IIR::Port));
        registerInfo(paramGain, "Gain", 0.f,
                     new parameters::LinearParameterTaper(-18.f, +18.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(FilterPorts, mIIR), sizeof(dsp::IIR::Port));
        registerInfo(paramOutGain, "Output Gain", 0.f,
                     new parameters::LinearParameterTaper(-18.f, +18.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(FilterPorts, mOutputGain), sizeof(dsp::Gain::Port));
    }

    // -------------------------------------------------------------------------

    FilterProcessor::FilterProcessor()
        : plugin::Processor<numParameters, FilterPorts, FilterState, FilterProcessor>(gParametersInfo,
                                                                                      "Filter", true)
    {
        mFilterMappers[FilterTypes::bell]       = &FilterProcessor::internalMapBell;
        mFilterMappers[FilterTypes::lowShelf]   = &FilterProcessor::internalMapLS;
        mFilterMappers[FilterTypes::highShelf]  = &FilterProcessor::internalMapHS;
        mFilterMappers[FilterTypes::highPass]   = &FilterProcessor::internalMapHP;
        mFilterMappers[FilterTypes::lowPass]    = &FilterProcessor::internalMapLP;

        setMapper(paramInGain,      &FilterProcessor::mapInputGain);
        setMapper(paramFilterType,  &FilterProcessor::mapFilter);
        setMapper(paramFrequency,   &FilterProcessor::mapFilter);
        setMapper(paramQ,           &FilterProcessor::mapFilter);
        setMapper(paramGain,        &FilterProcessor::mapFilter);
        setMapper(paramOutGain,     &FilterProcessor::mapOutputGain);
    }

    FilterProcessor::~FilterProcessor()
    {

    }

    // -------------------------------------------------------------------------

    juce::AudioProcessorEditor* FilterProcessor::createEditor()
    {
        return new FilterEditor(this);
    }

    // -------------------------------------------------------------------------

    void FilterProcessor::mapInputGain(void* outPortData)
    {
        dsp::Gain::Port& gainPort = *reinterpret_cast<dsp::Gain::Port*>(outPortData);
        gainPort.mTargetGain = dsp::dBToLinear(getParameterPlain(paramInGain));
    }

    void FilterProcessor::mapFilter(void* outPortData)
    {
        const int type = static_cast<int>(getParameterPlain(paramFilterType));
        (this->*mFilterMappers[type])(*reinterpret_cast<dsp::IIR::Port*>(outPortData),
                                      getParameterPlain(paramFrequency),
                                      getParameterPlain(paramQ),
                                      getParameterPlain(paramGain));
    }

    void FilterProcessor::mapOutputGain(void* outPortData)
    {
        dsp::Gain::Port& gainPort = *reinterpret_cast<dsp::Gain::Port*>(outPortData);
        gainPort.mTargetGain = dsp::dBToLinear(getParameterPlain(paramOutGain));
    }

    // -------------------------------------------------------------------------

    void FilterProcessor::internalMapBell(dsp::IIR::Port& outIIR, float inFrequency,
                                          float inQ, float inGain)
    {
        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && inQ > 0.);

        const dsp::float32 clippedFreq      = std::min(inFrequency, .5f * samplerate - 100.f);
        const dsp::float32 a                = std::pow(10.f, inGain / 40.f);
        const dsp::float32 w                = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 alpha            = .5f * std::sin(w) / inQ;
        const dsp::float32 c2               = -2.f * std::cos(w);
        const dsp::float32 alphaTimesA      = alpha * a;
        const dsp::float32 alpheOverA       = alpha / a;
        const dsp::float32 ib0              = 1.f / (1.f + alpheOverA);

        outIIR.mCoefficients[0]             = ib0 * (1.f + alphaTimesA);
        outIIR.mCoefficients[1]             = ib0 * c2;
        outIIR.mCoefficients[2]             = ib0 * (1.f - alphaTimesA);
        outIIR.mCoefficients[3]             = 1.f;
        outIIR.mCoefficients[4]             = ib0 * c2;
        outIIR.mCoefficients[5]             = ib0 * (1.f - alpheOverA);
    }

    void FilterProcessor::internalMapLS(dsp::IIR::Port& outIIR, float inFrequency,
                                        float inQ, float inGain)
    {
        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && inQ > 0.);

        const dsp::float32 clippedFreq      = std::min(inFrequency, .5f * samplerate - 100.f);
        const dsp::float32 a                = std::pow(10.f, inGain / 40.f);
        const dsp::float32 aMinus1          = a - 1.f;
        const dsp::float32 aPlus1           = a + 1.f;
        const dsp::float32 w                = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 cosW             = std::cos(w);
        const dsp::float32 beta             = std::sin(w) * std::sqrt(a) / inQ;
        const dsp::float32 aMinus1TimesCosW = aMinus1 * cosW;
        const dsp::float32 ib0              = 1.f / (aPlus1 + aMinus1TimesCosW + beta);

        outIIR.mCoefficients[0]             = ib0 * (a * (aPlus1 - aMinus1TimesCosW + beta));
        outIIR.mCoefficients[1]             = ib0 * (a * 2.f * (aMinus1 - aPlus1 * cosW));
        outIIR.mCoefficients[2]             = ib0 * (a * (aPlus1 - aMinus1TimesCosW - beta));
        outIIR.mCoefficients[3]             = 1.f;
        outIIR.mCoefficients[4]             = ib0 * (-2.f * (aMinus1 + aPlus1 * cosW));
        outIIR.mCoefficients[5]             = ib0 * (aPlus1 + aMinus1TimesCosW - beta);
    }

    void FilterProcessor::internalMapHS(dsp::IIR::Port& outIIR, float inFrequency,
                                        float inQ, float inGain)
    {
        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && inQ > 0.);

        const dsp::float32 clippedFreq      = std::min(inFrequency, .5f * samplerate - 100.f);
        const dsp::float32 a                = std::pow(10.f, inGain / 40.f);
        const dsp::float32 aMinus1          = a - 1.f;
        const dsp::float32 aPlus1           = a + 1.f;
        const dsp::float32 w                = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 cosW             = std::cos(w);
        const dsp::float32 beta             = std::sin(w) * std::sqrt(a) / inQ;
        const dsp::float32 aMinus1TimesCosW = aMinus1 * cosW;
        const dsp::float32 ib0              = 1.f / (aPlus1 - aMinus1TimesCosW + beta);

        outIIR.mCoefficients[0]             = ib0 * (a * (aPlus1 + aMinus1TimesCosW + beta));
        outIIR.mCoefficients[1]             = ib0 * (a * -2.f * (aMinus1 + aPlus1 * cosW));
        outIIR.mCoefficients[2]             = ib0 * (a * (aPlus1 + aMinus1TimesCosW - beta));
        outIIR.mCoefficients[3]             = 1.f;
        outIIR.mCoefficients[4]             = ib0 * (2.f * (aMinus1 - aPlus1 * cosW));
        outIIR.mCoefficients[5]             = ib0 * (aPlus1 - aMinus1TimesCosW - beta);
    }

    void FilterProcessor::internalMapHP(dsp::IIR::Port& outIIR, float inFrequency,
                                        float inQ, float)
    {
        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && inQ > 0.);

        const dsp::float32 clippedFreq      = std::min(inFrequency, .5f * samplerate - 100.f);
        const dsp::float32 w0               = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 alpha            = std::sin(w0) / (2.f * inQ);
        const dsp::float32 ib0              = 1.f / (1.f + alpha);

        outIIR.mCoefficients[0]             = ib0 * (.5f * (1.f + std::cos(w0)));
        outIIR.mCoefficients[1]             = ib0 * (-(1.f + std::cos(w0)));
        outIIR.mCoefficients[2]             = ib0 * (.5f * (1.f + std::cos(w0)));
        outIIR.mCoefficients[3]             = 1.;
        outIIR.mCoefficients[4]             = ib0 * ( -2.f * std::cos(w0));
        outIIR.mCoefficients[5]             = ib0 * (1.f - alpha);
    }

    void FilterProcessor::internalMapLP(dsp::IIR::Port& outIIR, float inFrequency,
                                        float inQ, float)
    {
        const dsp::float32 samplerate = dsp::float32(getSampleRate());

        jassert(samplerate > 0. && inQ > 0.);

        const dsp::float32 clippedFreq      = std::min(inFrequency, .5f * samplerate - 100.f);
        const dsp::float32 w0               = dsp::twoPi_32 * clippedFreq / samplerate;
        const dsp::float32 alpha            = std::sin(w0) / (2.f * inQ);
        const dsp::float32 ib0              = 1.f / (1.f + alpha);

        outIIR.mCoefficients[0]             = ib0 * (.5f * (1.f - std::cos(w0)));
        outIIR.mCoefficients[1]             = ib0 * ((1.f - std::cos(w0)));
        outIIR.mCoefficients[2]             = ib0 * (.5f * (1.f - std::cos(w0)));
        outIIR.mCoefficients[3]             = 1.f;
        outIIR.mCoefficients[4]             = ib0 * (-2.f * std::cos(w0));
        outIIR.mCoefficients[5]             = ib0 * (1.f - alpha);
    }
}

// -----------------------------------------------------------------------------

REGISTER_PLUGIN_FILTER(filter::FilterProcessor)

