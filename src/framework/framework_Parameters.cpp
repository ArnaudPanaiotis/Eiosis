/*!
 * \file       framework_Parameters.cpp
 * Copyright   Eiosis 2014
 */

#include "framework/framework_Parameters.h"
#include <cassert>

namespace parameters
{
    template<typename Type>
    inline Type ranged(Type inLow, Type inHigh, Type inValue)
    {
        assert(inLow <= inHigh);
        return (inValue < inLow) ? inLow : ((inHigh < inValue) ? inHigh : inValue);
    }

    // -------------------------------------------------------------------------

    LinearParameterTaper::LinearParameterTaper(float inStartValue, float inEndValue)
        : mStartValue(inStartValue)
        , mRange(inEndValue - inStartValue)
        , mIRange(1.f / mRange)
    {

    }

    LinearParameterTaper::~LinearParameterTaper()
    {

    }

    float LinearParameterTaper::getNormalized(float inPlainValue) const
    {
        return (inPlainValue - mStartValue) * mIRange;
    }

    float LinearParameterTaper::getPlain(float inNormalizedValue) const
    {
        return mStartValue + inNormalizedValue * mRange;
    }

    // -------------------------------------------------------------------------

    LogParameterTaper::LogParameterTaper(float inStartValue, float inEndValue,
                                         float inMidValue, float inMidPosition)
        : mInvert(inStartValue > inEndValue)
        , mMinValue(mInvert ? inEndValue : inStartValue)
        , mFullRange(std::abs(inEndValue - inStartValue))
        , mMapFactor(std::log10((inMidValue - mMinValue) / mFullRange) / std::log10(inMidPosition))
        , mNormalizeFactor(1.f / mMapFactor)
    {

    }

    LogParameterTaper::~LogParameterTaper()
    {

    }

    float LogParameterTaper::getNormalized(float inPlainValue) const
    {
        const float val = ranged(mMinValue, mMinValue + mFullRange, inPlainValue);
        const float res = std::pow((val - mMinValue) / mFullRange, mNormalizeFactor);
        return mInvert ? 1.f - res : res;
    }

    float LogParameterTaper::getPlain(float inNormalizedValue) const
    {
        if (mInvert)
        {
            inNormalizedValue = 1.f - inNormalizedValue;
        }
        return mFullRange * std::pow(inNormalizedValue, mMapFactor) + mMinValue;
    }

    // -------------------------------------------------------------------------

    EnumeratedParameterTaper::EnumeratedParameterTaper(int inNumValues)
        : mNumValues(inNumValues)
    {

    }

    EnumeratedParameterTaper::~EnumeratedParameterTaper()
    {

    }

    float EnumeratedParameterTaper::getNormalized(float inPlainValue) const
    {
        return float(2 * inPlainValue + 1) / float(2 * mNumValues);
    }

    float EnumeratedParameterTaper::getPlain(float inNormalizedValue) const
    {
        return float(std::min(mNumValues - 1,
                              static_cast<int>(std::floor(inNormalizedValue * mNumValues))));
    }

    // -------------------------------------------------------------------------

    ValueSuffixDisplayDelegate::ValueSuffixDisplayDelegate(const char* inSuffix,
                                                           int inNumDecimals,
                                                           bool inDisplaySign)
        : mSuffix(inSuffix)
        , mNumDecimals(inNumDecimals)
        , mDisplaySign(inDisplaySign)
    {

    }

    juce::String ValueSuffixDisplayDelegate::toText(float inValue) const
    {
        jassert(mTaper != 0);
        char buf[64];
        std::memset(buf, 0, 64);
        const float val = mTaper->getPlain(inValue);

        if (mDisplaySign)
        {
            sprintf(buf, "%s%.*f%s", val == 0.f ? "" : val > 0.f ? "+" : "-",
                    mNumDecimals, std::abs(val), mSuffix.c_str());
        }
        else
        {
            if (val < 1000.f)
            {
                sprintf(buf, "%.*f%s", mNumDecimals, val, mSuffix.c_str());
            }
            else
            {
                sprintf(buf, "%.*fk%s", mNumDecimals, val * .001f, mSuffix.c_str());
            }
        }
        return juce::String(buf, strlen(buf));
    }

    bool ValueSuffixDisplayDelegate::fromText(const juce::String& inText,
                                              float& outValue) const
    {
        char buffer[128];
        std::memset(buffer, 0, 128);
        std::strncpy(buffer, inText.toUTF8(), 128);

        char* temp = buffer;
        while ((temp = std::strchr(temp, ',')) != 0)
        {
            *temp++ = '.';
        }

        float factor = 1.;
        if (std::strchr(buffer, 'K') != 0 || std::strchr(buffer, 'k') != 0)
        {
            factor = 1000.;
        }

        float plainValue;
        if (sscanf(buffer, "%g %*s", &plainValue) == 1)
        {
            outValue = ranged(0.f, 1.f, mTaper->getNormalized(factor * plainValue));
            return true;
        }
        return false;
    }

    // -------------------------------------------------------------------------

    EnumeratedDisplayDelegate::EnumeratedDisplayDelegate(const char* inValues[],
                                                         unsigned inNumValues)
        : mValues(inValues)
        , mNumValues(inNumValues)
    {

    }

    juce::String EnumeratedDisplayDelegate::toText(float inValue) const
    {
        jassert(mTaper != 0);
        char buf[64];
        std::memset(buf, 0, 64);
        const unsigned val = static_cast<unsigned>(mTaper->getPlain(inValue));
        jassert(val < mNumValues)
        return mValues[val];
    }

    bool EnumeratedDisplayDelegate::fromText(const juce::String& inText,
                                             float& outValue) const
    {
        for (unsigned i = 0; i < mNumValues; ++i)
        {
            if (inText == mValues[i])
            {
                outValue = mTaper->getNormalized(float(i));
                return true;
            }
        }
        return false;
    }
}
