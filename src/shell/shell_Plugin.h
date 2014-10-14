/*!
 * \file       shell_Plugin.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "shell/shell_Process.hpp"

namespace shell
{
    enum ShellParameters
    {
        paramInGain = 0,
        paramOutGain,

        numParameters,
    };

    // -------------------------------------------------------------------------

    struct ShellParametersInfo : parameters::ParametersInfo<numParameters>
    {
        ShellParametersInfo();
    };

    static const ShellParametersInfo gParametersInfo;

    // -------------------------------------------------------------------------

    class ShellProcessor
        : public plugin::Processor<numParameters, ShellPorts, ShellState, ShellProcessor>
    {
    public:
        ShellProcessor();
        virtual ~ShellProcessor();

    public: // juce::AudioProcessor
        virtual juce::AudioProcessorEditor* createEditor();

    private:
        void mapInputGain(void* outPortData);
        void mapOutputGain(void* outPortData);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShellProcessor)
    };
}
