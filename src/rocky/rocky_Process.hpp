/*!
 * \file       rocky_Process.hpp
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_Processor.h"
#include "framework/framework_Cells.h"

namespace rocky
{
    struct RockyPorts
    {
        dsp::Gain::Port mInputGain;
        dsp::IIR::Port mHP;
        dsp::IIR::Port mLS;
        dsp::IIR::Port mBell1;
        dsp::IIR::Port mBell2;
        dsp::IIR::Port mHS;
        dsp::IIR::Port mLP;
        dsp::Gain::Port mOutputGain;
    };

    struct RockyState : plugin::StateBase
    {
        dsp::Gain::State mInputGain[plugin::gNumMaxChannels];
        dsp::IIR::State mHP[plugin::gNumMaxChannels];
        dsp::IIR::State mLS[plugin::gNumMaxChannels];
        dsp::IIR::State mBell1[plugin::gNumMaxChannels];
        dsp::IIR::State mBell2[plugin::gNumMaxChannels];
        dsp::IIR::State mHS[plugin::gNumMaxChannels];
        dsp::IIR::State mLP[plugin::gNumMaxChannels];
        dsp::Gain::State mOutputGain[plugin::gNumMaxChannels];
    };
}

// -----------------------------------------------------------------------------

template<>
inline void resetState(rocky::RockyState& ioState)
{
    for (unsigned int i = 0; i < plugin::gNumMaxChannels; ++i)
    {
        dsp::Gain::reset(ioState.mInputGain[i]);
        dsp::IIR::reset(ioState.mHP[i]);
        dsp::IIR::reset(ioState.mLS[i]);
        dsp::IIR::reset(ioState.mBell1[i]);
        dsp::IIR::reset(ioState.mBell2[i]);
        dsp::IIR::reset(ioState.mHS[i]);
        dsp::IIR::reset(ioState.mLP[i]);
        dsp::Gain::reset(ioState.mOutputGain[i]);
    }
}

template<>
inline void processState(const dsp::ProcessType*const* inInputChannels, int inNumInputChannels,
                         dsp::ProcessType*const* inOutputChannels, int inNumOutputChannels,
                         int inNumSamples, const rocky::RockyPorts& inPorts,
                         rocky::RockyState& ioState)
{
    for (int i = 0; i < inNumInputChannels; ++i)
    {
        dsp::Gain::process(inInputChannels[i], inOutputChannels[i], inNumSamples,
                           inPorts.mInputGain, ioState.mInputGain[i]);
        dsp::IIR::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                          inPorts.mHP, ioState.mHP[i]);
        dsp::IIR::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                          inPorts.mLS, ioState.mLS[i]);
        dsp::IIR::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                          inPorts.mBell1, ioState.mBell1[i]);
        dsp::IIR::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                          inPorts.mBell2, ioState.mBell2[i]);
        dsp::IIR::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                          inPorts.mHS, ioState.mHS[i]);
        dsp::IIR::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                          inPorts.mLP, ioState.mLP[i]);
        dsp::Gain::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                           inPorts.mOutputGain, ioState.mOutputGain[i]);
    }
    (void)inNumOutputChannels;
}
