/*!
 * \file       shell_Editor.cpp
 * Copyright   Eiosis 2014
 */

#include "shell/shell_Editor.h"
#include "shell/shell_Plugin.h"
#include "framework/framework_GUI.h"

namespace shell
{
    ShellEditor::ShellEditor(ShellProcessor* inProcessor)
        : juce::AudioProcessorEditor(inProcessor)
        , mInputLabel(new juce::Label("Input Gain", "Input Gain"))
        , mInputGain(new gui::Knob(shell::gParametersInfo[shell::paramInGain], inProcessor))
        , mOutputLabel(new juce::Label("Output Gain", "Output Gain"))
        , mOutputGain(new gui::Knob(shell::gParametersInfo[shell::paramOutGain], inProcessor))
        , mLogo(juce::ImageFileFormat::loadFrom(BinaryData::resources_EiosisLogo_png,
                                                BinaryData::resources_EiosisLogo_pngSize))
    {
        configureLabel(mInputLabel);
        configureLabel(mOutputLabel);

        addAndMakeVisible(mInputLabel);
        addAndMakeVisible(mInputGain);
        addAndMakeVisible(mOutputLabel);
        addAndMakeVisible(mOutputGain);

        setSize(400, 300);
    }

    ShellEditor::~ShellEditor()
    {
        deleteAllChildren();
    }

    // -------------------------------------------------------------------------

    void ShellEditor::resized()
    {
        const int offset    = 32;
        const int labelW    = 80;
        const int knobSide  = 64;
        const int labelH    = 24;

        const int ioY = (getHeight() - knobSide) >> 1;

        int x = offset;
        int y = ioY;

        mInputLabel->setBounds(x, y - labelH, labelW, labelH);
        mInputGain->setBounds(x + ((labelW - knobSide) >> 1), y, knobSide, knobSide);

        y = ioY;
        x = getWidth() - labelW - offset;
        mOutputLabel->setBounds(x, y - labelH, labelW, labelH);
        mOutputGain->setBounds(x + ((labelW - knobSide) >> 1), y, knobSide, knobSide);
    }

    void ShellEditor::paint(juce::Graphics& inGraphics)
    {
        inGraphics.fillAll(juce::Colour(0xff333333));
        inGraphics.drawImageAt(mLogo,
                               (getWidth() - mLogo.getWidth()) >> 1,
                               (getHeight() - mLogo.getHeight()) >> 1, false);
    }

    // -------------------------------------------------------------------------

    void ShellEditor::configureLabel(juce::Label* inLabel) const
    {
        inLabel->setFont(juce::Font(14.f, juce::Font::bold));
        inLabel->setJustificationType(juce::Justification::centred);
    }
}
