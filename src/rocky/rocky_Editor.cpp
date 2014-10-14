/*!
 * \file       rocky_Editor.cpp
 * Copyright   Eiosis 2014
 */

#include "rocky/rocky_Editor.h"
#include "rocky/rocky_Plugin.h"

namespace rocky
{
    static const int gMainLabelHeight   = 28;
    static const int gAuxLabelHeight    = 24;

    class RockyEditor::Gain : public juce::Component
    {
    public:
        Gain(RockyEditor* inEditor, const juce::String& inName, int inParamId)
            : mLabel(new juce::Label(inName, inName))
            , mGainLabel(new juce::Label("Gain", "Gain"))
            , mGain(new gui::Knob(rocky::gParametersInfo[inParamId],
                                  inEditor->getAudioProcessor()))
        {
            inEditor->configureLabel(mLabel,        true);
            inEditor->configureLabel(mGainLabel,    false);

            addAndMakeVisible(mLabel);
            addAndMakeVisible(mGainLabel);
            addAndMakeVisible(mGain);

            inEditor->addAndMakeVisible(this);
        }
        virtual ~Gain()
        {
            deleteAllChildren();
        }

    public: // juce::Component
        virtual void resized()
        {
            int x = 0;
            int y = 0;
            int w = getWidth();
            int h = gMainLabelHeight;

            mLabel->setBounds(x, y, w, h);
            y = gMainLabelHeight + ((gAuxLabelHeight + w) << 1);
            h = gAuxLabelHeight;
            mGainLabel->setBounds(x, y, w, h);
            y += gAuxLabelHeight;
            h = w;
            mGain->setBounds(x, y, w, h);
        }
        virtual void paint(juce::Graphics&)
        {

        }

    private:
        juce::Label* const mLabel;
        juce::Label* const mGainLabel;
        gui::Knob* const mGain;

    private:
        JUCE_DECLARE_NON_COPYABLE(Gain);
    };

    class RockyEditor::Cut : public juce::Component
    {
    public:
        Cut(RockyEditor* inEditor, const juce::String& inName,
            int inFrequencyParamId, int inQParamId)
            : mLabel(new juce::Label(inName, inName))
            , mFrequencyLabel(new juce::Label("Frequency", "Frequency"))
            , mFrequency(new gui::Knob(rocky::gParametersInfo[inFrequencyParamId],
                                       inEditor->getAudioProcessor()))
            , mQLabel(new juce::Label("Q", "Q"))
            , mQ(new gui::Knob(rocky::gParametersInfo[inQParamId],
                               inEditor->getAudioProcessor()))
        {
            inEditor->configureLabel(mLabel,            true);
            inEditor->configureLabel(mFrequencyLabel,   false);
            inEditor->configureLabel(mQLabel,           false);

            addAndMakeVisible(mLabel);
            addAndMakeVisible(mFrequencyLabel);
            addAndMakeVisible(mFrequency);
            addAndMakeVisible(mQLabel);
            addAndMakeVisible(mQ);

            inEditor->addAndMakeVisible(this);
        }
        virtual ~Cut()
        {
            deleteAllChildren();
        }

    public: // juce::Component
        virtual void resized()
        {
            int x = 0;
            int y = 0;
            int w = getWidth();
            int h = gMainLabelHeight;

            mLabel->setBounds(x, y, w, h);
            y += h;
            h = gAuxLabelHeight;
            mFrequencyLabel->setBounds(x, y, w, h);
            y += h;
            h = w;
            mFrequency->setBounds(x, y, w, h);
            y += h;
            h = gAuxLabelHeight;
            mQLabel->setBounds(x, y, w, h);
            y += h;
            h = w;
            mQ->setBounds(x, y, w, h);
        }
        virtual void paint(juce::Graphics&)
        {

        }

    private:
        juce::Label* const mLabel;
        juce::Label* const mFrequencyLabel;
        gui::Knob* const mFrequency;
        juce::Label* const mQLabel;
        gui::Knob* const mQ;

    private:
        JUCE_DECLARE_NON_COPYABLE(Cut);
    };

    class RockyEditor::Band : public juce::Component
    {
    public:
        Band(RockyEditor* inEditor, const juce::String& inName,
             int inFrequencyParamId, int inQParamId, int inGainParamId)
            : mLabel(new juce::Label(inName, inName))
            , mFrequencyLabel(new juce::Label("Frequency", "Frequency"))
            , mFrequency(new gui::Knob(rocky::gParametersInfo[inFrequencyParamId],
                                       inEditor->getAudioProcessor()))
            , mQLabel(new juce::Label("Q", "Q"))
            , mQ(new gui::Knob(rocky::gParametersInfo[inQParamId],
                               inEditor->getAudioProcessor()))
            , mGainLabel(new juce::Label("Gain", "Gain"))
            , mGain(new gui::Knob(rocky::gParametersInfo[inGainParamId],
                                  inEditor->getAudioProcessor()))
        {
            inEditor->configureLabel(mLabel,            true);
            inEditor->configureLabel(mFrequencyLabel,   false);
            inEditor->configureLabel(mQLabel,           false);
            inEditor->configureLabel(mGainLabel,        false);

            addAndMakeVisible(mLabel);
            addAndMakeVisible(mFrequencyLabel);
            addAndMakeVisible(mFrequency);
            addAndMakeVisible(mQLabel);
            addAndMakeVisible(mQ);
            addAndMakeVisible(mGainLabel);
            addAndMakeVisible(mGain);

            inEditor->addAndMakeVisible(this);
        }
        virtual ~Band()
        {
            deleteAllChildren();
        }

    public: // juce::Component
        virtual void resized()
        {
            int x = 0;
            int y = 0;
            int w = getWidth();
            int h = gMainLabelHeight;

            mLabel->setBounds(x, y, w, h);
            y += h;
            h = gAuxLabelHeight;
            mFrequencyLabel->setBounds(x, y, w, h);
            y += h;
            h = w;
            mFrequency->setBounds(x, y, w, h);
            y += h;
            h = gAuxLabelHeight;
            mQLabel->setBounds(x, y, w, h);
            y += h;
            h = w;
            mQ->setBounds(x, y, w, h);
            y += h;
            h = gAuxLabelHeight;
            mGainLabel->setBounds(x, y, w, h);
            y += h;
            h = w;
            mGain->setBounds(x, y, w, h);
        }
        virtual void paint(juce::Graphics&)
        {

        }

    private:
        juce::Label* const mLabel;
        juce::Label* const mFrequencyLabel;
        gui::Knob* const mFrequency;
        juce::Label* const mQLabel;
        gui::Knob* const mQ;
        juce::Label* const mGainLabel;
        gui::Knob* const mGain;

    private:
        JUCE_DECLARE_NON_COPYABLE(Band);
    };

    // -------------------------------------------------------------------------

    RockyEditor::RockyEditor(RockyProcessor* inProcessor)
        : juce::AudioProcessorEditor(inProcessor)
        , mInputGain(new Gain(this, "Input", rocky::paramInGain))
        , mHP(new Cut(this, "High Pass", rocky::paramHPFrequency, rocky::paramHPQ))
        , mLS(new Band(this, "Low Shelf", rocky::paramLSFrequency, rocky::paramLSQ, rocky::paramLSGain))
        , mBell1(new Band(this, "Bell 1", rocky::paramBell1Frequency, rocky::paramBell1Q, rocky::paramBell1Gain))
        , mBell2(new Band(this, "Bell 2", rocky::paramBell2Frequency, rocky::paramBell2Q, rocky::paramBell2Gain))
        , mHS(new Band(this, "High Shelf", rocky::paramHSFrequency, rocky::paramHSQ, rocky::paramHSGain))
        , mLP(new Cut(this, "Low Pass", rocky::paramLPFrequency, rocky::paramLPQ))
        , mOutputGain(new Gain(this, "Output", rocky::paramOutGain))
        , mSavePresetButton(new juce::TextButton("Save Preset", "Save Preset"))
        , mLoadPresetButton(new juce::TextButton("Load Preset", "Load Preset"))
        , mLogo(juce::ImageFileFormat::loadFrom(BinaryData::resources_EiosisLogo_png,
                                                BinaryData::resources_EiosisLogo_pngSize))
    {
        addAndMakeVisible(mSavePresetButton);
        addAndMakeVisible(mLoadPresetButton);

        mSavePresetButton->addListener(this);
        mLoadPresetButton->addListener(this);

        setSize(750, 410);
    }

    RockyEditor::~RockyEditor()
    {
        deleteAllChildren();
    }

    // -------------------------------------------------------------------------

    void RockyEditor::resized()
    {
        const int offset = 6;

        const int buttonW   = 96;
        const int buttonH   = 24;

        int x = ((getWidth() >> 1) - buttonW) >> 1;
        int y = offset;

        mSavePresetButton->setBounds(x, y, buttonW, buttonH);
        x += getWidth() >> 1;
        mLoadPresetButton->setBounds(x, y, buttonW, buttonH);

        const int w = (getWidth() - 9 * offset) / 8;
        const int h = getHeight() - y;

        y += buttonH + offset;
        x = offset;

        mInputGain->setBounds(x, y, w, h);
        x += w + offset;
        mHP->setBounds(x, y, w, h);
        x += w + offset;
        mLS->setBounds(x, y, w, h);
        x += w + offset;
        mBell1->setBounds(x, y, w, h);
        x += w + offset;
        mBell2->setBounds(x, y, w, h);
        x += w + offset;
        mHS->setBounds(x, y, w, h);
        x += w + offset;
        mLP->setBounds(x, y, w, h);
        x += w + offset;
        mOutputGain->setBounds(x, y, w, h);
    }

    void RockyEditor::paint(juce::Graphics& inGraphics)
    {
        inGraphics.fillAll(juce::Colour(0xff333333));
        inGraphics.drawImageAt(mLogo,
                               (getWidth() - mLogo.getWidth()) >> 1,
                               (getHeight() - mLogo.getHeight()) >> 1, false);
    }

    // -------------------------------------------------------------------------

    void RockyEditor::buttonClicked(juce::Button* inButton)
    {
        juce::FileChooser fc("Save Preset",
                             juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                             "*.*", true);
        if (inButton == mSavePresetButton)
        {
            if (fc.browseForFileToSave(true))
            {
				reinterpret_cast<RockyProcessor*>(getAudioProcessor())->savePresetTo(fc.getResult());
            }
        }
        else if (inButton == mLoadPresetButton)
        {
            if (fc.browseForFileToOpen())
            {
                reinterpret_cast<RockyProcessor*>(getAudioProcessor())->loadPresetFrom(fc.getResult());
            }
        }
    }

    // -------------------------------------------------------------------------

    void RockyEditor::configureLabel(juce::Label* inLabel, bool inIsMain) const
    {
        if (inIsMain)
        {
            inLabel->setFont(juce::Font(14.f, juce::Font::bold));
        }
        else
        {
            inLabel->setFont(juce::Font(12.f));
        }
        inLabel->setJustificationType(juce::Justification::centred);
    }
}
