/*!
 * \file       filter_Plugin.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "filter/filter_Process.hpp"

namespace filter
{
    enum FilterParameters
    {
        paramInGain = 0,
        paramFilterType,
        paramFrequency,
        paramQ,
        paramGain,
        paramOutGain,

        numParameters,
    };

    // -------------------------------------------------------------------------

    struct FilterTypes
    {
        enum Type
        {
            bell = 0,
            lowShelf,
            highShelf,
            highPass,
            lowPass,

            numFilters,
        };

        static const char* sNames[numFilters];
    };

    // -------------------------------------------------------------------------

    struct FilterParametersInfo : parameters::ParametersInfo<numParameters>
    {
        FilterParametersInfo();
    };

    static const FilterParametersInfo gParametersInfo;

    // -------------------------------------------------------------------------

    class FilterProcessor
        : public plugin::Processor<numParameters, FilterPorts, FilterState, FilterProcessor>
    {
    public:
        FilterProcessor();
        virtual ~FilterProcessor();

    public: // juce::AudioProcessor
        virtual juce::AudioProcessorEditor* createEditor();

    private:
        void mapInputGain(void* outPortData);
        void mapFilter(void* outPortData);
        void mapOutputGain(void* outPortData);

    private:
        void internalMapBell(dsp::IIR::Port& outIIR, float inFrequency,
                             float inQ, float inGain);
        void internalMapLS(dsp::IIR::Port& outIIR, float inFrequency,
                           float inQ, float inGain);
        void internalMapHS(dsp::IIR::Port& outIIR, float inFrequency,
                          float inQ, float inGain);
        void internalMapHP(dsp::IIR::Port& outIIR, float inFrequency,
                          float inQ, float inGain);
        void internalMapLP(dsp::IIR::Port& outIIR, float inFrequency,
                          float inQ, float inGain);

    private:
        typedef void (FilterProcessor::*Mapper)(dsp::IIR::Port&, float, float, float);
        Mapper mFilterMappers[FilterTypes::numFilters];

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterProcessor)
    };
}
