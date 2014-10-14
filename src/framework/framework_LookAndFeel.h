/*!
 * \file       framework_LookAndFeel.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include <JuceHeader.h>

namespace gui
{
    class LookAndFeel : public juce::LookAndFeel_V3
    {
    public:
        LookAndFeel();
        virtual ~LookAndFeel();

    public: // juce::Slider::LookAndFeelMethods
        virtual juce::Font getSliderPopupFont(juce::Slider& inSlider);

    public: // juce::ComboBox::LookAndFeelMethods
        virtual juce::Font getComboBoxFont(juce::ComboBox& inComboBox);

    public: // juce::PopupMenu::LookAndFeelMethods
        virtual juce::Font getPopupMenuFont();

    private:
        JUCE_DECLARE_NON_COPYABLE(LookAndFeel);
    };
}
