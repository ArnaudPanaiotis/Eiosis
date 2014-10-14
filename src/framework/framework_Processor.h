/*!
 * \file       framework_Processor.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_Parameters.h"
#include "framework/framework_Plugin.h"
#include "framework/framework_DSP.h"

/*!
    This function is called whenever the algorithm state needs to be reset.
*/
template<class StateType>
inline void resetState(StateType& ioState);

/*!
    This is the main audio process callback.
    This function is specialized in concrete plugins,
    and is called for processing audio from and to the given buffer,
    the process configuration is passed through the read-only PortsType,
    and the algorithm persistent data is stored in the StateType structure.
*/
template<class PortsType, class StateType>
inline void processState(const dsp::ProcessType*const* inInputChannels, int inNumInputChannels,
                         dsp::ProcessType*const* inOutputChannels, int inNumOutputChannels,
                         int inNumSamples, const PortsType& inPorts, StateType& ioState);

// -----------------------------------------------------------------------------

namespace plugin
{
    /*!
        This is the main Plugin component.
        The Processor is in charge of:
         * keeping a track of the plugin current state (Parameters)
         * initializing and processing the audio stream
         * saving/loading chunks and presets
         * handle the conversion between Parameter Values and Algorithm Ports
        This class is template on its number of parameters, its Ports data structure,
        its state data structure, and on its concrete Mappers type,
        for which it will store one function per parameter.
    */
    template<unsigned NumParameters, class PortsType, class StateType, class MappersType>
    class Processor
        : public juce::AudioProcessor
    {
    public:
        Processor(const parameters::ParametersInfo<NumParameters>& inParametersInfo,
                  const juce::String& inName, bool inHasEditor);
        virtual ~Processor();

    public: // juce::AudioProcessor
        virtual const juce::String getName() const;

    public: // juce::AudioProcessor
        virtual void prepareToPlay(double inSamplerate, int inBlockSize);
        virtual void releaseResources();

    public: // juce::AudioProcessor
        virtual void processBlock(juce::AudioSampleBuffer& ioAudioBuffer,
                                 juce::MidiBuffer& ioMidiBuffer);

    public: // juce::AudioProcessor
        virtual int getNumParameters();
        virtual float getParameter(int inIndex);
        virtual void setParameter(int inIndex, float inValue);
        virtual const juce::String getParameterName(int inIndex);
        virtual const juce::String getParameterText(int inIndex);

    public: // juce::AudioProcessor
        virtual const juce::String getInputChannelName(int inChannelIndex) const;
        virtual const juce::String getOutputChannelName(int inChannelIndex) const;
        virtual bool isInputChannelStereoPair(int inChannelIndex) const;
        virtual bool isOutputChannelStereoPair(int inChannelIndex) const;

    public: // juce::AudioProcessor
        virtual bool acceptsMidi() const;
        virtual bool producesMidi() const;
        virtual bool silenceInProducesSilenceOut() const;
        virtual double getTailLengthSeconds() const;

    public: // juce::AudioProcessor
        virtual juce::AudioProcessorEditor* createEditor();
        virtual bool hasEditor() const;

    public: // juce::AudioProcessor
        virtual int getNumPrograms();
        virtual int getCurrentProgram();
        virtual void setCurrentProgram(int inIndex);
        virtual const juce::String getProgramName(int inIndex);
        virtual void changeProgramName(int inIndex, const juce::String& inName);

    public: // juce::AudioProcessor
        virtual void getStateInformation(juce::MemoryBlock& outData);
        virtual void setStateInformation(const void* inData, int inDataSize);

    public:
        const parameters::ParameterInfo& getParameterInfo(int inParamIndex) const;

    private:
        inline void mapAllParameters();
        inline void mapParameter(int inIndex);

    protected:
        inline void setMapper(int inIndex, void (MappersType::*inMapper)(void*));

    protected:
        inline float getParameterPlain(int inIndex);

    private:
        typedef parameters::ParametersInfo<NumParameters>   ParametersInfo;
        typedef State<NumParameters>                        State;
        typedef Context<PortsType, StateType>               Context;

    private:
        const ParametersInfo& mParametersInfo;
        const juce::String mName;
        const bool mHasEditor;
        State mState;
        Context mContext;
        void (MappersType::*mMappers[NumParameters])(void*);

    private:
        JUCE_DECLARE_NON_COPYABLE(Processor)
    };
}

#include "framework/framework_Processor.hpp"
