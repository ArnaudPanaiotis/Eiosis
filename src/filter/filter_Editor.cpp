/*!
 * \file       filter_Editor.cpp
 * Copyright   Eiosis 2014
 */

#include "filter/filter_Editor.h"
#include "filter/filter_Plugin.h"
#include "framework/framework_GUI.h"

namespace filter
{
    FilterEditor::FilterEditor(FilterProcessor* inProcessor)
        : juce::AudioProcessorEditor(inProcessor)
        , mInputLabel(new juce::Label("Input Gain", "Input Gain"))
        , mInputGain(new gui::Knob(filter::gParametersInfo[filter::paramInGain], inProcessor))
        , mFilterTypeLabel(new juce::Label("Filter Type", "Filter Type"))
        , mFilterType(new gui::Combo(filter::gParametersInfo[filter::paramFilterType], inProcessor))
        , mFrequencyLabel(new juce::Label("Frequency", "Frequency"))
        , mFrequency(new gui::Knob(filter::gParametersInfo[filter::paramFrequency], inProcessor))
        , mQLabel(new juce::Label("Q", "Q"))
        , mQ(new gui::Knob(filter::gParametersInfo[filter::paramQ], inProcessor))
        , mGainLabel(new juce::Label("Gain", "Gain"))
        , mGain(new gui::Knob(filter::gParametersInfo[filter::paramGain], inProcessor))
        , mOutputLabel(new juce::Label("Output Gain", "Output Gain"))
        , mOutputGain(new gui::Knob(filter::gParametersInfo[filter::paramOutGain], inProcessor))
        , mLogo(juce::ImageFileFormat::loadFrom(BinaryData::resources_EiosisLogo_png,
                                                BinaryData::resources_EiosisLogo_pngSize))
    {
        configureLabel(mInputLabel);
        configureLabel(mFilterTypeLabel);
        configureLabel(mFrequencyLabel);
        configureLabel(mQLabel);
        configureLabel(mGainLabel);
        configureLabel(mOutputLabel);

        addAndMakeVisible(mInputLabel);
        addAndMakeVisible(mInputGain);
        addAndMakeVisible(mFilterTypeLabel);
        addAndMakeVisible(mFilterType);
        addAndMakeVisible(mFrequencyLabel);
        addAndMakeVisible(mFrequency);
        addAndMakeVisible(mQLabel);
        addAndMakeVisible(mQ);
        addAndMakeVisible(mGainLabel);
        addAndMakeVisible(mGain);
        addAndMakeVisible(mOutputLabel);
        addAndMakeVisible(mOutputGain);

        setSize(380, 380);
    }

    FilterEditor::~FilterEditor()
    {
        deleteAllChildren();
    }

    // -------------------------------------------------------------------------

    void FilterEditor::resized()
    {
        const int offset    = 24;
        const int labelW    = 108;
        const int knobSide  = 64;
        const int labelH    = 24;

        const int x3        = getWidth() / 3;
        const int y3        = getHeight() / 3;

        const int ioY       = (getHeight() - knobSide) >> 1;
        const int filterY   = (y3 - knobSide) >> 1;

        int x = offset;
        int y = ioY;

        mInputLabel->setBounds(x, y - labelH, labelW, labelH);
        mInputGain->setBounds(x + ((labelW - knobSide) >> 1), y, knobSide, knobSide);

        x = offset;
        y = filterY;
        mFilterTypeLabel->setBounds(x, y - labelH, labelW, labelH);
        mFilterType->setBounds(x + ((labelW - labelW) >> 1), y, labelW, labelH);

        x = x3 + ((x3 - labelW) >> 1);
        y = filterY;
        mFrequencyLabel->setBounds(x, y - labelH, labelW, labelH);
        y += y3;
        mQLabel->setBounds(x, y - labelH, labelW, labelH);
        y += y3;
        mGainLabel->setBounds(x, y - labelH, labelW, labelH);

        x = x3 + ((x3 - knobSide) >> 1);
        y = filterY;
        mFrequency->setBounds(x, y, knobSide, knobSide);
        y += y3;
        mQ->setBounds(x, y, knobSide, knobSide);
        y += y3;
        mGain->setBounds(x, y, knobSide, knobSide);

        y = ioY;
        x = getWidth() - labelW - offset;
        mOutputLabel->setBounds(x, y - labelH, labelW, labelH);
        mOutputGain->setBounds(x + ((labelW - knobSide) >> 1), y, knobSide, knobSide);
    }

    void FilterEditor::paint(juce::Graphics& inGraphics)
    {
        inGraphics.fillAll(juce::Colour(0xff333333));
        inGraphics.drawImageAt(mLogo,
                               (getWidth() - mLogo.getWidth()) >> 1,
                               (getHeight() - mLogo.getHeight()) >> 1, false);
    }

    // -------------------------------------------------------------------------

    void FilterEditor::configureLabel(juce::Label* inLabel) const
    {
        inLabel->setFont(juce::Font(14.f, juce::Font::bold));
        inLabel->setJustificationType(juce::Justification::centred);
    }
}
