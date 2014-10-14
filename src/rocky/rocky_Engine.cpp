/*!
 * \file       rocky_Engine.cpp
 * Copyright   Eiosis 2014
 */

#include "rocky/rocky_Engine.h"

namespace rocky
{
    RockyEngineBase::RockyEngineBase(RockyProcessor& inProcessor)
        : juce::Thread("Rocky Engine")
        , mProcessor(inProcessor)
    {
        initializeEngine();
    }

    RockyEngineBase::~RockyEngineBase()
    {

    }

    // -------------------------------------------------------------------------

    void RockyEngineBase::run()
    {
        execute();
        triggerAsyncUpdate();
    }

    // -------------------------------------------------------------------------

    void RockyEngineBase::handleAsyncUpdate()
    {
        if (!mResult.mStatus)
        {
            juce::AlertWindow::showNativeDialogBox("Oops...", mResult.mStatusString, false);
        }
    }

    // -------------------------------------------------------------------------

    void RockyEngineBase::initializeEngine()
    {
        mResult.mStatus         = false;
        mResult.mStatusString   = juce::String::empty;
    }

    // -------------------------------------------------------------------------

    void RockyEngineBase::operator()()
    {
		 startThread();
		 run();
		 stopThread(-1);
    }

    // -------------------------------------------------------------------------

    RockyPresetSaver::RockyPresetSaver(RockyProcessor& inProcessor, const juce::File& inFile)
        : RockyEngineBase(inProcessor)
        , mFile(inFile)
    {
		initializeEngine();
    }

    RockyPresetSaver::~RockyPresetSaver()
    {

    }

    // -------------------------------------------------------------------------

    void RockyPresetSaver::execute()
    {
        if (mState.getSize() == 0)
        {
            mResult.mStatus         = false;
            mResult.mStatusString   = "Can't retrieve Plugin State.";
            return;
        }

        juce::FileOutputStream stream(mFile);
        if (stream.failedToOpen())
        {
            mResult.mStatus         = false;
            mResult.mStatusString   = "Can't open Preset File.";
            return;
        }

        // Update state structure in case it has changed since we last asked for it...
        mProcessor.getStateInformation(mState);

        stream << mState;

        if (!stream.getStatus())
        {
            mResult.mStatus         = false;
            mResult.mStatusString   = "Can't save Preset.";
        }
        else
        {
            mResult.mStatus         = true;
            mResult.mStatusString   = juce::String::empty;
        }
    }

    // -------------------------------------------------------------------------

    void RockyPresetSaver::initializeEngine()
    {
        mProcessor.getStateInformation(mState);
        RockyEngineBase::initializeEngine();
    }

    // -------------------------------------------------------------------------

    RockyPresetLoader::RockyPresetLoader(RockyProcessor& inProcessor, const juce::File& inFile)
        : RockyEngineBase(inProcessor)
        , mFile(inFile)
    {
		initializeEngine();
    }

    RockyPresetLoader::~RockyPresetLoader()
    {

    }

    // -------------------------------------------------------------------------

    void RockyPresetLoader::execute()
    {
        juce::FileInputStream stream(mFile);
        if (stream.failedToOpen())
        {
            mResult.mStatus         = false;
            mResult.mStatusString   = "Can't open Preset File.";
            return;
        }

        juce::MemoryBlock state;
        stream.readIntoMemoryBlock(state);
        if (!stream.getStatus())
        {
            mResult.mStatus         = false;
            mResult.mStatusString   = "Can't load Preset.";
            return;
        }

        mProcessor.setStateInformation(state.getData(), state.getSize());

        mResult.mStatus         = true;
        mResult.mStatusString   = juce::String::empty;
    }
}
