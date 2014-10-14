/*!
 * \file       framework_GUI.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_LookAndFeel.h"

namespace parameters
{
    struct ParameterInfo;
}

namespace gui
{
    /*
        Control is the base class of any UI object that will interact with a parameter.
        Each UI object should notify and be notified of any parameter value changed through the Control API.
    */
    class Control
        : public juce::AudioProcessorListener
    {
    public:
        Control(const parameters::ParameterInfo& inParamInfo,
                juce::AudioProcessor* inProcessor);
        virtual ~Control();

    public: // juce::AudioProcessorListener
        virtual void audioProcessorChanged(juce::AudioProcessor* inProcessor);
        virtual void audioProcessorParameterChanged(juce::AudioProcessor* inProcessor,
                                                    int inIndex, float inValue);

    protected: // Control
        virtual void controlValueChanged(float inValue) = 0;

    protected:
        void setControlValue(float inValue);
        float getControlValue() const;
        juce::String getControlValueText(float inValue) const;
        bool getControlValueFromText(const juce::String& inText, float& outValue) const;
        float getNormalizedDefaultValue() const;
        float getPlainValue(float inNormalizedValue) const;
        float getNormalizedValue(float inPlainValue) const;

    private:
        const parameters::ParameterInfo& mParameterInfo;
        juce::AudioProcessor* const mProcessor;

    private:
        JUCE_DECLARE_NON_COPYABLE(Control);
    };

    // --------------------------------------------------------------------------

    class Knob
        : public Control
        , public juce::Slider
    {
    public:
        Knob(const parameters::ParameterInfo& inParamInfo,
             juce::AudioProcessor* inProcessor);
        virtual ~Knob();

    public: // Control
        virtual void controlValueChanged(float inValue);

    public: // juce::Slider
        virtual juce::String getTextFromValue(double inValue);
        virtual double getValueFromText(const juce::String& inText);
        virtual void valueChanged();

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob);
    };

    // --------------------------------------------------------------------------

    class Combo
        : public Control
        , public juce::ComboBox
        , public juce::ComboBox::Listener
    {
    public:
        Combo(const parameters::ParameterInfo& inParamInfo,
             juce::AudioProcessor* inProcessor);
        virtual ~Combo();

    public: // Control
        virtual void controlValueChanged(float inValue);

    public: // juce::ComboBox::Listener
        virtual void comboBoxChanged(juce::ComboBox* inCombo);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Combo);
    };

    // -------------------------------------------------------------------------

    class JuceHolder
    {
    public:
        struct Instance
        {
            Instance();
            ~Instance();
        };

    private:
        JuceHolder();

    public:
        ~JuceHolder();

    public:
        void registerInstance();
        void unregisterinstance();

    private:
        static JuceHolder sHolder;

    private:
        juce::CriticalSection mMutex;
        unsigned int mNumInstances;
        LookAndFeel* mLookAndFeel;

    private:
        JUCE_DECLARE_NON_COPYABLE(JuceHolder);
    };
}
