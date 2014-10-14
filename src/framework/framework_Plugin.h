/*!
 * \file       framework_Plugin.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include <JuceHeader.h>

namespace plugin
{
    static const unsigned int gStateMagic       = 0xdeadbeef;
    static const unsigned int gStateVersion     = 0x0001;
    static const unsigned int gNumMaxChannels   = 2;

    /*!
        A State is what is stored in an host chunk or a preset.
        It is a data structured that contains the state of all the plugin's parameters.
    */
    template<unsigned NumParameters>
    struct State
    {
        unsigned int mMagic;
        unsigned int mDataSize;
        unsigned int mVersion;
        float mParameterValues[NumParameters];
    };

    // -------------------------------------------------------------------------

    /*!
        This DoubleBuffer class is used to store the plugin ports.
        Pending member stores the next algorithm configuration,
        while the Current member stores the configuration that is currently used by the plugin.
        It's up to the Plugin Processor to swap current and pending as soon as it has mapped a parameter change.
        On each process cycle, the process callback ask for current(),
        what induces a caching of the current ports values, avoiding locking for too long.
    */
    template<typename Type>
    class DoubleBuffer
    {
    public:
        DoubleBuffer()
            : mCurrent(&mA)
            , mPending(&mB)
        {}

    public:
        inline Type cacheCurrent()
        {
            juce::SpinLock::ScopedLockType lock(mMutex);
            return *mCurrent;
        }
        inline Type* pending()
        {
            return mPending;
        }

    public:
        inline void swapAndCopy()
        {
            juce::SpinLock::ScopedLockType lock(mMutex);
            *mCurrent = *mPending;
            std::swap(mCurrent, mPending);
        }

    private:
        Type mA;
        Type mB;
        juce::SpinLock mMutex;

    private:
        Type* mCurrent;
        Type* mPending;

    private:
        DoubleBuffer(const DoubleBuffer&);
        DoubleBuffer& operator=(const DoubleBuffer&);
    };

    // -------------------------------------------------------------------------

    struct StateBase
    {
        double mSamplerate;
    };

    // -------------------------------------------------------------------------

    /*!
        The Context structure contains Ports and State.
        Each port describes one algorithm operation.
        Ports are updated on Parameter value changed, using the dedicated Mapper Functions.
        The State describes the current state of the Processor's Algorithm.
        Ports are configuration data, State is persistent process data,
        such as filters memories, gains, delay lines, samplerate, temp. buffers etc.
    */
    template<class PortsType, class StateType>
    struct Context
    {
        plugin::DoubleBuffer<PortsType> mPorts;
        StateType mState;
    };
}

#define REGISTER_PLUGIN_FILTER(ProcessorClass)              \
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()    \
{                                                           \
    return new ProcessorClass;                              \
}                                                           \

