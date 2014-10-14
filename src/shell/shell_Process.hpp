/*!
 * \file       shell_Process.hpp
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_Processor.h"
#include "framework/framework_Cells.h"

namespace shell
{
    struct ShellPorts
    {
        dsp::Gain::Port mInputGain;
        dsp::Gain::Port mOutputGain;
    };

    struct ShellState : plugin::StateBase
    {
        dsp::Gain::State mInputGain[plugin::gNumMaxChannels];
        dsp::Gain::State mOutputGain[plugin::gNumMaxChannels];
    };
}

// -----------------------------------------------------------------------------

template<>
inline void resetState(shell::ShellState& ioState)
{
    for (unsigned int i = 0; i < plugin::gNumMaxChannels; ++i)
    {
        dsp::Gain::reset(ioState.mInputGain[i]);
        dsp::Gain::reset(ioState.mOutputGain[i]);
    }
}

template<>
inline void processState(const dsp::ProcessType*const* inInputChannels, int inNumInputChannels,
                         dsp::ProcessType*const* inOutputChannels, int inNumOutputChannels,
                         int inNumSamples, const shell::ShellPorts& inPorts,
                         shell::ShellState& ioState)
{
    for (int i = 0; i < inNumInputChannels; ++i)
    {
        dsp::Gain::process(inInputChannels[i], inOutputChannels[i], inNumSamples,
                           inPorts.mInputGain, ioState.mInputGain[i]);
        dsp::Gain::process(inOutputChannels[i], inOutputChannels[i], inNumSamples,
                           inPorts.mOutputGain, ioState.mOutputGain[i]);
    }
    (void)inNumOutputChannels;
}
