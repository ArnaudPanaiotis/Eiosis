/*!
 * \file       framework_LookAndFeel.cpp
 * Copyright   Eiosis 2014
 */

#include "framework/framework_LookAndFeel.h"

namespace gui
{
    LookAndFeel::LookAndFeel()
    {
        setColour(juce::Slider::rotarySliderOutlineColourId,        juce::Colour(0xeecccccc));
        setColour(juce::Slider::rotarySliderFillColourId,           juce::Colour(0xff666666));

        setColour(juce::Slider::textBoxTextColourId,                juce::Colour(0xeecccccc));
        setColour(juce::Slider::textBoxBackgroundColourId,          juce::Colour(0x00000000));
        setColour(juce::Slider::textBoxHighlightColourId,           juce::Colour(0xeeeeeeee));
        setColour(juce::Slider::textBoxOutlineColourId,             juce::Colour(0x00000000));

        setColour(juce::Label::textColourId,                        juce::Colour(0xeecccccc));

        setColour(juce::PopupMenu::backgroundColourId,              juce::Colour(0xdd333333));
        setColour(juce::PopupMenu::textColourId,                    juce::Colour(0xeecccccc));
        setColour(juce::PopupMenu::headerTextColourId,              juce::Colour(0xeecccccc));
        setColour(juce::PopupMenu::highlightedBackgroundColourId,   juce::Colour(0xdd222222));
        setColour(juce::PopupMenu::highlightedTextColourId,         juce::Colour(0xeeeeeeee));
        
        setColour(juce::ComboBox::backgroundColourId,               juce::Colour(0x00000000));
        setColour(juce::ComboBox::textColourId,                     juce::Colour(0xeecccccc));
        setColour(juce::ComboBox::outlineColourId,                  juce::Colour(0xeecccccc));
        setColour(juce::ComboBox::buttonColourId,                   juce::Colour(0xeecccccc));
        setColour(juce::ComboBox::arrowColourId,                    juce::Colour(0xeecccccc));
    }

    LookAndFeel::~LookAndFeel()
    {

    }

    // -------------------------------------------------------------------------

    juce::Font LookAndFeel::getSliderPopupFont(juce::Slider&)
    {
        return juce::Font(13.f);
    }

    // -------------------------------------------------------------------------

    juce::Font LookAndFeel::getComboBoxFont(juce::ComboBox&)
    {
        return juce::Font(13.f);
    }

    // -------------------------------------------------------------------------

    juce::Font LookAndFeel::getPopupMenuFont()
    {
        return juce::Font(13.f);
    }
}
