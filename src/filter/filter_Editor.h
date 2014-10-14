/*!
 * \file       filter_Editor.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_GUI.h"

namespace filter
{
    class FilterProcessor;

    class FilterEditor
        : public juce::AudioProcessorEditor
    {
    public:
        explicit FilterEditor(FilterProcessor* inProcessor);
        virtual ~FilterEditor();

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
        juce::Label* const mFilterTypeLabel;
        gui::Combo* const mFilterType;
        juce::Label* const mFrequencyLabel;
        gui::Knob* const mFrequency;
        juce::Label* const mQLabel;
        gui::Knob* const mQ;
        juce::Label* const mGainLabel;
        gui::Knob* const mGain;
        juce::Label* const mOutputLabel;
        gui::Knob* const mOutputGain;

    private:
        const juce::Image mLogo;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEditor);
    };
}
