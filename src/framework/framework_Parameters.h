/*!
 * \file       framework_Parameters.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include <JuceHeader.h>

namespace parameters
{
    /*!
        A Taper is used to convert a Parameter value between plain and normalized values.
        Tapers are the link between the GUI controls (knobs etc.) and the processor
        and host value representation (which is a normalized value (0.f<->1.f))
    */
    class ParameterTaper
    {
    public:
        virtual ~ParameterTaper() {}

    public: // ParameterTaper
        virtual float getNormalized(float inPlainValue) const = 0;
        virtual float getPlain(float inNormalizedValue) const = 0;
    };

    // -------------------------------------------------------------------------

    /*
        This Taper implements an affine transform based taping.
    */
    class LinearParameterTaper : public ParameterTaper
    {
    public:
        LinearParameterTaper(float inStartValue, float inEndValue);
        virtual ~LinearParameterTaper();

    public: // ParameterTaper
        virtual float getNormalized(float inPlainValue) const;
        virtual float getPlain(float inNormalizedValue) const;

    private:
        const float mStartValue;
        const float mRange;
        const float mIRange;

    private:
        JUCE_DECLARE_NON_COPYABLE(LinearParameterTaper);
    };

    /*
        This Taper implements an log transform based taper.
    */
    class LogParameterTaper : public ParameterTaper
    {
    public:
        LogParameterTaper(float inStartValue, float inEndValue,
                          float inMidValue, float inMidPosition);
        virtual ~LogParameterTaper();

    public: // ParameterTaper
        virtual float getNormalized(float inPlainValue) const;
        virtual float getPlain(float inNormalizedValue) const;

    private:
        const bool mInvert;
        const float mMinValue;
        const float mFullRange;
        const float mMapFactor;
        const float mNormalizeFactor;

    private:
        JUCE_DECLARE_NON_COPYABLE(LogParameterTaper);
    };

    /*
        This Taper implements an enumerated to normalized taper.
    */
    class EnumeratedParameterTaper : public ParameterTaper
    {
    public:
        explicit EnumeratedParameterTaper(int inNumValues);
        virtual ~EnumeratedParameterTaper();

    public: // ParameterTaper
        virtual float getNormalized(float inPlainValue) const;
        virtual float getPlain(float inNormalizedValue) const;

    private:
        const int mNumValues;

    private:
        JUCE_DECLARE_NON_COPYABLE(EnumeratedParameterTaper);
    };

    // -------------------------------------------------------------------------

    /*!
        A DisplayDelegate performs the conversion between a normalized value and a value text.
    */
    class DisplayDelegate
    {
    public:
        DisplayDelegate()
            : mTaper(0)
        {}
        virtual ~DisplayDelegate()
        {}

    public: // DisplayDelegate
        virtual juce::String toText(float inValue) const                            = 0;
        virtual bool fromText(const juce::String& inText, float& outValue) const    = 0;

    public:
        void setTaper(const ParameterTaper* inTaper) { mTaper = inTaper; }

    protected:
        const ParameterTaper* mTaper;

    private:
        JUCE_DECLARE_NON_COPYABLE(DisplayDelegate);
    };

    // -------------------------------------------------------------------------

    /*!
        This display delegate will format the value display and add a suffix to it.
    */
    class ValueSuffixDisplayDelegate : public DisplayDelegate
    {
    public:
        explicit ValueSuffixDisplayDelegate(const char* inSuffix,
                                            int inNumDecimaals = 2,
                                            bool inDisplaySign = false);

    public: // DisplayDelegate
        virtual juce::String toText(float inValue) const;
        virtual bool fromText(const juce::String& inText, float& outValue) const;

    private:
        const std::string mSuffix;
        const int mNumDecimals;
        const bool mDisplaySign;

    private:
        JUCE_DECLARE_NON_COPYABLE(ValueSuffixDisplayDelegate);
    };

    /*!
        This display delegate will format the value display according to a string tables.
    */
    class EnumeratedDisplayDelegate : public DisplayDelegate
    {
    public:
        EnumeratedDisplayDelegate(const char* inValues[], unsigned inNumValues);

    public: // DisplayDelegate
        virtual juce::String toText(float inValue) const;
        virtual bool fromText(const juce::String& inText, float& outValue) const;

    private:
        const char*const* mValues;
        const unsigned mNumValues;

    private:
        JUCE_DECLARE_NON_COPYABLE(EnumeratedDisplayDelegate);
    };

    // -------------------------------------------------------------------------

    /*!
        A ParameterInfo structure describes a plugin parameter.
    */
    struct ParameterInfo
    {
        int mIndex;                                 //<! Parameter Index
        juce::String mName;                         //<! Parameter Name
        float mDefaultValue;                        //<! Parameter plain default value
        const ParameterTaper* mTaper;               //<! The Taper that this parameter should use to convert normalized<=>plain
        const DisplayDelegate* mDisplayDelegate;    //<! The DisplayDelegate this parameter should use to convert value<=>text
        size_t mPortId;                             //<! The offset of the port this parameter is bound to in the context structure
        size_t mPortSize;                           //<! The size of this parameter's associated port
    };

    /*!
        ParametersInfo structure stores all this plugin parameters.
        Each Plugin should instantiate and fill its own static ParameterInfo structure.
    */
    template<unsigned NumParameters>
    struct ParametersInfo
    {
        ParametersInfo();
        ~ParametersInfo();

        ParameterInfo mInfos[NumParameters];

        inline const ParameterInfo& operator[](unsigned inIndex) const
        {
            jassert(inIndex < NumParameters);
            return mInfos[inIndex];
        }

    protected:
        void registerInfo(unsigned inIndex, const char* inName, float inDefaultValue,
                          ParameterTaper* inTaper, DisplayDelegate* inDisplayDelegate,
                          size_t inPortId, size_t inPortSize);

    private:
        ParametersInfo(const ParametersInfo&);
        ParametersInfo& operator=(const ParametersInfo&);
    };

    // -------------------------------------------------------------------------

    template<unsigned NumParameters>
    ParametersInfo<NumParameters>::ParametersInfo()
    {

    }

    template<unsigned NumParameters>
    ParametersInfo<NumParameters>::~ParametersInfo()
    {
        for (unsigned i = 0; i < NumParameters; ++i)
        {
            delete mInfos[i].mDisplayDelegate;
            delete mInfos[i].mTaper;
        }
    }

    template<unsigned NumParameters>
    void ParametersInfo<NumParameters>::registerInfo(unsigned inIndex, const char* inName,
                                                     float inDefaultValue,
                                                     ParameterTaper* inTaper,
                                                     DisplayDelegate* inDisplayDelegate,
                                                     size_t inPortId, size_t inPortSize)
    {
        jassert(inIndex < NumParameters);
        ParameterInfo& info     = mInfos[inIndex];
        info.mIndex             = inIndex;
        info.mName              = inName;
        info.mDefaultValue      = inDefaultValue;
        info.mTaper             = inTaper;
        info.mDisplayDelegate   = inDisplayDelegate;
        info.mPortId            = inPortId;
        info.mPortSize          = inPortSize;
        inDisplayDelegate->setTaper(inTaper);
    }
}
