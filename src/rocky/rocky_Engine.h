/*!
 * \file       rocky_Engine.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "rocky/rocky_Plugin.h"

namespace rocky
{
    class RockyEngineBase
        : public juce::Thread
        , public juce::AsyncUpdater
    {
    public:
        explicit RockyEngineBase(RockyProcessor& inProcessor);
        virtual ~RockyEngineBase();

    public: // juce::Thread
        virtual void run();

    public: // juce::AsyncUpdater
        virtual void handleAsyncUpdate();

    protected: // RockyEngineBase
        virtual void initializeEngine();
        virtual void execute() = 0;

    public:
        void operator()();

    protected:
        struct Result
        {
            bool mStatus;
            juce::String mStatusString;
        };

    protected:
        RockyProcessor& mProcessor;
        Result mResult;

    private:
        JUCE_DECLARE_NON_COPYABLE(RockyEngineBase);
    };

    // -------------------------------------------------------------------------

    class RockyPresetSaver : public RockyEngineBase
    {
    public:
        RockyPresetSaver(RockyProcessor& inProcessor, const juce::File& inFile);
        virtual ~RockyPresetSaver();

    protected: // RockyEngineBase
        virtual void initializeEngine();
        virtual void execute();

    private:
        const juce::File mFile;
        juce::MemoryBlock mState;

    private:
        JUCE_DECLARE_NON_COPYABLE(RockyPresetSaver);
    };

    // -------------------------------------------------------------------------

    class RockyPresetLoader : public RockyEngineBase
    {
    public:
        RockyPresetLoader(RockyProcessor& inProcessor, const juce::File& inFile);
        virtual ~RockyPresetLoader();

    protected: // RockyEngineBase
        virtual void execute();

    private:
        const juce::File mFile;
        juce::MemoryBlock mState;

    private:
        JUCE_DECLARE_NON_COPYABLE(RockyPresetLoader);
    };
}
