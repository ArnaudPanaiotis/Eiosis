/*!
 * \file       framework_Processor.hpp
 * Copyright   Eiosis 2014
 */

#pragma once

namespace plugin
{
    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    Processor<NumParameters, PortsType, StateType, MappersType>::Processor(const parameters::ParametersInfo<NumParameters>& inParametersInfo,
                                                                           const juce::String& inName, bool inHasEditor)
        : mParametersInfo(inParametersInfo)
        , mName(inName)
        , mHasEditor(inHasEditor)
    {
        std::fill(mMappers, mMappers + NumParameters, (void (MappersType::*)(void*))0);

        mState.mMagic       = plugin::gStateMagic;
        mState.mDataSize    = sizeof(State);
        mState.mVersion     = plugin::gStateVersion;

        mContext.mState.mSamplerate = 0.;

        for (unsigned i = 0; i < NumParameters; ++i)
        {
            jassert(mParametersInfo[i].mTaper != 0 && mParametersInfo[i].mDisplayDelegate != 0);
            mState.mParameterValues[i] = mParametersInfo[i].mTaper->getNormalized(mParametersInfo[i].mDefaultValue);
        }
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    Processor<NumParameters, PortsType, StateType, MappersType>::~Processor()
    {

    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    const juce::String Processor<NumParameters, PortsType, StateType, MappersType>::getName() const
    {
        return mName;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    int Processor<NumParameters, PortsType, StateType, MappersType>::getNumParameters()
    {
        return int(NumParameters);
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    float Processor<NumParameters, PortsType, StateType, MappersType>::getParameter(int inIndex)
    {
        jassert(inIndex < int(NumParameters));
        if (inIndex < int(NumParameters))
        {
            return mState.mParameterValues[inIndex];
        }
        return 0.f;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::setParameter(int inIndex, float inValue)
    {
        jassert(inIndex < int(NumParameters));
        if (inIndex < int(NumParameters))
        {
            mState.mParameterValues[inIndex] = inValue;
            mapParameter(inIndex);
        }
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    const juce::String Processor<NumParameters, PortsType, StateType, MappersType>::getParameterName(int inIndex)
    {
        jassert(inIndex < int(NumParameters));
        if (inIndex < int(NumParameters))
        {
            return mParametersInfo[inIndex].mName;
        }
        return juce::String::empty;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    const juce::String Processor<NumParameters, PortsType, StateType, MappersType>::getParameterText(int inIndex)
    {
        jassert(inIndex < int(NumParameters));
        if (inIndex < int(NumParameters))
        {
            return mParametersInfo[inIndex].mDisplayDelegate->toText(mState.mParameterValues[inIndex]);
        }
        return juce::String::empty;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    const juce::String Processor<NumParameters, PortsType, StateType, MappersType>::getInputChannelName(int inIndex) const
    {
        return juce::String(inIndex + 1);
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    const juce::String Processor<NumParameters, PortsType, StateType, MappersType>::getOutputChannelName(int inIndex) const
    {
        return juce::String(inIndex + 1);
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    bool Processor<NumParameters, PortsType, StateType, MappersType>::isInputChannelStereoPair(int inIndex) const
    {
        return true;
        (void)inIndex;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    bool Processor<NumParameters, PortsType, StateType, MappersType>::isOutputChannelStereoPair(int inIndex) const
    {
        return true;
        (void)inIndex;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    bool Processor<NumParameters, PortsType, StateType, MappersType>::acceptsMidi() const
    {
        return false;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    bool Processor<NumParameters, PortsType, StateType, MappersType>::producesMidi() const
    {
        return false;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    bool Processor<NumParameters, PortsType, StateType, MappersType>::silenceInProducesSilenceOut() const
    {
        return false;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    double Processor<NumParameters, PortsType, StateType, MappersType>::getTailLengthSeconds() const
    {
        return 0.;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    juce::AudioProcessorEditor* Processor<NumParameters, PortsType, StateType, MappersType>::createEditor()
    {
        return 0;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    bool Processor<NumParameters, PortsType, StateType, MappersType>::hasEditor() const
    {
        return mHasEditor;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    int Processor<NumParameters, PortsType, StateType, MappersType>::getNumPrograms()
    {
        return 0;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    int Processor<NumParameters, PortsType, StateType, MappersType>::getCurrentProgram()
    {
        return 0;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::setCurrentProgram(int inIndex)
    {
        (void)inIndex;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    const juce::String Processor<NumParameters, PortsType, StateType, MappersType>::getProgramName(int inIndex)
    {
        return juce::String::empty;
        (void)inIndex;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::changeProgramName(int inIndex,
                                                                                        const juce::String& inName)
    {
        (void)inIndex;
        (void)inName;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::prepareToPlay(double inSamplerate,
                                                                                    int inBlockSize)
    {
        mContext.mState.mSamplerate = inSamplerate;
        resetState(mContext.mState);
        mapAllParameters();
        (void)inBlockSize;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::releaseResources()
    {
        mContext.mState.mSamplerate = 0.;
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::processBlock(juce::AudioSampleBuffer& ioAudioBuffer,
                                                                                   juce::MidiBuffer& ioMidiBuffer)
    {
        processState(ioAudioBuffer.getArrayOfReadPointers(), ioAudioBuffer.getNumChannels(),
                     ioAudioBuffer.getArrayOfWritePointers(), ioAudioBuffer.getNumChannels(),
                     ioAudioBuffer.getNumSamples(), mContext.mPorts.cacheCurrent(),
                     mContext.mState);
        (void)ioMidiBuffer;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::getStateInformation(juce::MemoryBlock& outData)
    {
        jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());
        mState.mMagic       = gStateMagic;
        mState.mDataSize    = sizeof(State);
        mState.mVersion     = gStateVersion;
        outData.append(&mState, sizeof(State));
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::setStateInformation(const void* inData,
                                                                                          int inDataSize)
    {
        jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());
        if (inDataSize != sizeof(State))
        {
            return;
        }

        const State& state = *reinterpret_cast<const State*>(inData);
        if (state.mMagic == gStateMagic && state.mVersion == gStateVersion)
        {
            mState = state;
            mapAllParameters();
        }
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::mapAllParameters()
    {
        jassert(mContext.mState.mSamplerate > 0.);
        for (int i = 0; i < int(NumParameters); ++i)
        {
            jassert(mMappers[i] != 0);
            (reinterpret_cast<MappersType*>(this)->*mMappers[i])((unsigned char*)mContext.mPorts.pending() +
                                                                 mParametersInfo[i].mPortId);
        }
        mContext.mPorts.swapAndCopy();
    }

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::mapParameter(int inIndex)
    {
        jassert(inIndex < int(NumParameters) && mMappers[inIndex] != 0);
        (reinterpret_cast<MappersType*>(this)->*mMappers[inIndex])((unsigned char*)mContext.mPorts.pending() +
                                                                   mParametersInfo[inIndex].mPortId);
        mContext.mPorts.swapAndCopy();
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    void Processor<NumParameters, PortsType, StateType, MappersType>::setMapper(int inIndex,
                                                                                void (MappersType::*inMapper)(void*))
    {
        mMappers[inIndex] = inMapper;
    }

    // -------------------------------------------------------------------------

    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    inline float Processor<NumParameters, PortsType, StateType, MappersType>::getParameterPlain(int inIndex)
    {
        jassert(inIndex < int(NumParameters));
        if (inIndex < int(NumParameters))
        {
            return mParametersInfo[inIndex].mTaper->getPlain(mState.mParameterValues[inIndex]);
        }
        return 0.f;
    }
}
