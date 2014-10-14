/*!
 * \file       shell_Editor.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_GUI.h"

namespace shell
{
    class ShellProcessor;

    class ShellEditor
        : public juce::AudioProcessorEditor
    {
    public:
        explicit ShellEditor(ShellProcessor* inProcessor);
        virtual ~ShellEditor();

    public: // juce::Component
        virtual void resized();
        void paint(juce::Graphics& inGraphics);

    private:
        void configureLabel(juce::Label* inLabel) const;

    private:
        gui::JuceHolder::Instance mJuceHolder;

    private:
        juce::Label* const mInputLabel;
        gui::Knob* const mInputGain;
        juce::Label* const mOutputLabel;
        gui::Knob* const mOutputGain;

    private:
        const juce::Image mLogo;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShellEditor);
    };
}
