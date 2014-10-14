/*!
 * \file       framework_GUI.cpp
 * Copyright   Eiosis 2014
 */

#include "framework/framework_GUI.h"
#include "framework/framework_Parameters.h"

namespace gui
{
    Control::Control(const parameters::ParameterInfo& inParamInfo,
                     juce::AudioProcessor* inProcessor)
        : mParameterInfo(inParamInfo)
        , mProcessor(inProcessor)
    {
        mProcessor->addListener(this);
    }

    Control::~Control()
    {
        mProcessor->removeListener(this);
    }

    // -------------------------------------------------------------------------

    void Control::audioProcessorChanged(juce::AudioProcessor*)
    {

    }

    void Control::audioProcessorParameterChanged(juce::AudioProcessor* inProcessor,
                                                 int inIndex, float inValue)
    {
        jassert(inProcessor == mProcessor);
        if (inIndex == mParameterInfo.mIndex)
        {
            controlValueChanged(inValue);
        }
        (void)inProcessor;
    }

    // -------------------------------------------------------------------------

    void Control::setControlValue(float inValue)
    {
        mProcessor->setParameterNotifyingHost(mParameterInfo.mIndex, inValue);
    }

    float Control::getControlValue() const
    {
        return mProcessor->getParameter(mParameterInfo.mIndex);
    }

    juce::String Control::getControlValueText(float inValue) const
    {
        return mParameterInfo.mDisplayDelegate->toText(inValue);
    }

    bool Control::getControlValueFromText(const juce::String& inText, float& outValue) const
    {
        return mParameterInfo.mDisplayDelegate->fromText(inText, outValue);
    }

    float Control::getNormalizedDefaultValue() const
    {
        return mParameterInfo.mTaper->getNormalized(mParameterInfo.mDefaultValue);
    }

    float Control::getPlainValue(float inNormalizedValue) const
    {
        return mParameterInfo.mTaper->getPlain(inNormalizedValue);
    }

    float Control::getNormalizedValue(float inPlainValue) const
    {
        return mParameterInfo.mTaper->getNormalized(inPlainValue);
    }

    // -------------------------------------------------------------------------

    Knob::Knob(const parameters::ParameterInfo& inParamInfo,
               juce::AudioProcessor* inProcessor)
        : Control(inParamInfo, inProcessor)
        , juce::Slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow)
    {
        setRange(0., 1., .005);
        setValue(double(getControlValue()), juce::dontSendNotification);
        setDoubleClickReturnValue(true, getNormalizedDefaultValue());
    }

    Knob::~Knob()
    {

    }

    // -------------------------------------------------------------------------

    void Knob::controlValueChanged(float inValue)
    {
        setValue(inValue, juce::dontSendNotification);
    }

    // -------------------------------------------------------------------------

    juce::String Knob::getTextFromValue(double inValue)
    {
        return getControlValueText(float(inValue));
    }

    double Knob::getValueFromText(const juce::String& inText)
    {
        float val = 0.f;
        if (getControlValueFromText(inText, val))
        {
            return double(val);
        }
        return double(getControlValue());
    }

    void Knob::valueChanged()
    {
        setControlValue(float(getValue()));
    }

    // -------------------------------------------------------------------------

    Combo::Combo(const parameters::ParameterInfo& inParamInfo,
                 juce::AudioProcessor* inProcessor)
        : Control(inParamInfo, inProcessor)
    {
        const int numValues = static_cast<int>(getPlainValue(1.f)) + 1;
        for (int i = 0; i < numValues; ++i)
        {
            addItem(getControlValueText(getNormalizedValue(float(i))), i + 1);
        }
        setJustificationType(juce::Justification::centred);
        setSelectedItemIndex(static_cast<int>(getPlainValue(getControlValue())),
                             juce::dontSendNotification);
        addListener(this);
    }

    Combo::~Combo()
    {
        removeListener(this);
    }

    // -------------------------------------------------------------------------

    void Combo::controlValueChanged(float inValue)
    {
        setSelectedItemIndex(static_cast<int>(getPlainValue(inValue)),
                             juce::dontSendNotification);
    }

    // -------------------------------------------------------------------------

    void Combo::comboBoxChanged(juce::ComboBox*)
    {
        setControlValue(getNormalizedValue(float(getSelectedItemIndex())));
    }

    // -------------------------------------------------------------------------

    JuceHolder JuceHolder::sHolder;

    JuceHolder::JuceHolder()
        : mNumInstances(0)
        , mLookAndFeel(0)
    {

    }

    JuceHolder::~JuceHolder()
    {
        jassert(mNumInstances == 0 && mLookAndFeel == 0);
    }

    // -------------------------------------------------------------------------

    void JuceHolder::registerInstance()
    {
        juce::ScopedLock lock(mMutex);
        if (mNumInstances == 0)
        {
            jassert(mLookAndFeel == 0);
            mLookAndFeel = new LookAndFeel;
            juce::LookAndFeel::setDefaultLookAndFeel(mLookAndFeel);
        }
        ++mNumInstances;
    }

    void JuceHolder::unregisterinstance()
    {
        juce::ScopedLock lock(mMutex);
        jassert(mNumInstances > 0);
        if (--mNumInstances == 0)
        {
            juce::LookAndFeel::setDefaultLookAndFeel(0);
            delete mLookAndFeel;
            mLookAndFeel = 0;
        }
    }

    // -------------------------------------------------------------------------

    JuceHolder::Instance::Instance()
    {
        sHolder.registerInstance();
    }

    JuceHolder::Instance::~Instance()
    {
        sHolder.unregisterinstance();
    }
}
