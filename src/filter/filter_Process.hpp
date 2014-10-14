/*!
 * \file       filter_Process.hpp
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_Processor.h"
#include "framework/framework_Cells.h"

namespace filter
{
    struct FilterPorts
    {
        dsp::Gain::Port mInputGain;
        dsp::IIR::Port mIIR;
        dsp::Gain::Port mOutputGain;
    };

    struct FilterState : plugin::StateBase
    {
        dsp::Gain::State mInputGain[plugin::gNumMaxChannels];
        dsp::IIR::State mIIR[plugin::gNumMaxChannels];
        dsp::Gain::State mOutputGain[plugin::gNumMaxChannels];
    };
}

// -----------------------------------------------------------------------------

template<>
inline void resetState(filter::FilterState& ioState)
{
    for (unsigned int i = 0; i < plugin::gNumMaxChannels; ++i)
    {
        dsp::Gain::reset(ioState.mInputGain[i]);
        dsp::IIR::reset(ioState.mIIR[i]);
        dsp::Gain::reset(ioState.mOutputGain[i]);
    }
}

template<>
inline void processState(const dsp::ProcessType*const* inInputChannels, int inNumInputChannels,
                         dsp::ProcessType*const* inOutputChannels, int inNumOutputChannels,
                         int inNumSamples, const filter::FilterPorts& inPorts,
                         filter::FilterState& ioState)
{
    for (int i = 0; i < inNumInputChannels; ++i)
    {
        dsp::Gain::process(inInputChannels[i], inOutputChannels[i], inNumSamples,
                           inPorts.mInputGain, ioState.mInputGain[i]);
        dsp::IIR::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                          inPorts.mIIR, ioState.mIIR[i]);
        dsp::Gain::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                           inPorts.mOutputGain, ioState.mOutputGain[i]);
    }
    (void)inNumOutputChannels;
}
