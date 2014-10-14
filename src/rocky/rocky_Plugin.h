/*!
 * \file       rocky_Plugin.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "rocky/rocky_Process.hpp"

namespace rocky
{
    enum RockyParameters
    {
        paramInGain = 0,
        paramHPFrequency,
        paramHPQ,
        paramLSFrequency,
        paramLSQ,
        paramLSGain,
        paramBell1Frequency,
        paramBell1Q,
        paramBell1Gain,
        paramBell2Frequency,
        paramBell2Q,
        paramBell2Gain,
        paramHSFrequency,
        paramHSQ,
        paramHSGain,
        paramLPFrequency,
        paramLPQ,
        paramOutGain,

        numParameters,
    };

    // -------------------------------------------------------------------------

    struct RockyParametersInfo : parameters::ParametersInfo<numParameters>
    {
        RockyParametersInfo();
    };

    static const RockyParametersInfo gParametersInfo;

    // -------------------------------------------------------------------------

    class RockyProcessor
        : public plugin::Processor<numParameters, RockyPorts, RockyState, RockyProcessor>
    {
    public:
        RockyProcessor();
        virtual ~RockyProcessor();

    public: // juce::AudioProcessor
        virtual juce::AudioProcessorEditor* createEditor();

    public:
        void savePresetTo(const juce::File& inFile);
        void loadPresetFrom(const juce::File& inFile);

    private:
        void mapInputGain(void* outPortData);
        void mapHP(void* outPortData);
        void mapLS(void* outPortData);
        void mapBell1(void* outPortData);
        void mapBell2(void* outPortData);
        void mapHS(void* outPortData);
        void mapLP(void* outPortData);
        void mapOutputGain(void* outPortData);

    private:
        inline void internalMapBell(void* outPortData, float inFrequency,
                                    float inQ, float inGain);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RockyProcessor)
    };
}
