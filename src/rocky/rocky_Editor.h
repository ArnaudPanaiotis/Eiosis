/*!
 * \file       rocky_Editor.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_GUI.h"

namespace rocky
{
    class RockyProcessor;

    class RockyEditor
        : public juce::AudioProcessorEditor
        , public juce::Button::Listener
    {
    public:
        explicit RockyEditor(RockyProcessor* inProcessor);
        virtual ~RockyEditor();

    public: // juce::Component
        virtual void resized();
        void paint(juce::Graphics& inGraphics);

    public: // juce::Button::Listener
        virtual void buttonClicked(juce::Button* inButton);

    private:
        void configureLabel(juce::Label* inLabel, bool inIsMain) const;

    private:
        gui::JuceHolder::Instance mJuceHolder;

    private:
        class Gain;
        class Cut;
        class Band;

    private:
        Gain* const mInputGain;
        Cut* const mHP;
        Band* const mLS;
        Band* const mBell1;
        Band* const mBell2;
        Band* const mHS;
        Cut* const mLP;
        Gain* const mOutputGain;

    private:
        juce::TextButton* const mSavePresetButton;
        juce::TextButton* const mLoadPresetButton;

    private:
        const juce::Image mLogo;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RockyEditor);
    };
}
