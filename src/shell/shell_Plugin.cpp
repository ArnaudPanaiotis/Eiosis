/*!
 * \file       shell_Plugin.cpp
 * Copyright   Eiosis 2014
 */

#include "shell/shell_Plugin.h"
#include "shell/shell_Editor.h"

namespace shell
{
    ShellParametersInfo::ShellParametersInfo()
    {
        registerInfo(paramInGain, "Input Gain", 0.f,
                     new parameters::LinearParameterTaper(-18.f, +18.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(ShellPorts, mInputGain), sizeof(dsp::Gain::Port));
        registerInfo(paramOutGain, "Output Gain", 0.f,
                     new parameters::LinearParameterTaper(-18.f, +18.f),
                     new parameters::ValueSuffixDisplayDelegate("dB", 1, true),
                     offsetof(ShellPorts, mOutputGain), sizeof(dsp::Gain::Port));
    }

    // -------------------------------------------------------------------------

    ShellProcessor::ShellProcessor()
        : plugin::Processor<numParameters, ShellPorts, ShellState, ShellProcessor>(gParametersInfo,
                                                                                   "Shell", true)
    {
        setMapper(paramInGain,  &ShellProcessor::mapInputGain);
        setMapper(paramOutGain, &ShellProcessor::mapOutputGain);
    }

    ShellProcessor::~ShellProcessor()
    {

    }

    // -------------------------------------------------------------------------

    juce::AudioProcessorEditor* ShellProcessor::createEditor()
    {
        return new ShellEditor(this);
    }

    // -------------------------------------------------------------------------

    void ShellProcessor::mapInputGain(void* outPortData)
    {
        dsp::Gain::Port& gainPort = *reinterpret_cast<dsp::Gain::Port*>(outPortData);
        gainPort.mTargetGain = dsp::dBToLinear(getParameterPlain(paramInGain));
    }

    void ShellProcessor::mapOutputGain(void* outPortData)
    {
        dsp::Gain::Port& gainPort = *reinterpret_cast<dsp::Gain::Port*>(outPortData);
        gainPort.mTargetGain = dsp::dBToLinear(getParameterPlain(paramOutGain));
    }
}

// -----------------------------------------------------------------------------

REGISTER_PLUGIN_FILTER(shell::ShellProcessor)

