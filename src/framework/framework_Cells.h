/*!
 * \file       framework_Cells.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include "framework/framework_DSP.h"

namespace dsp
{
    struct Gain
    {
        struct Port
        {
            ProcessType mTargetGain;
        };

        struct State
        {
            ProcessType mCurrentGain;
        };

        static inline void reset(State& ioState)
        {
            ioState.mCurrentGain = 0.f;
        }
        static inline void process(const ProcessType* inSrc, ProcessType* outDest,
                                   int inNumSamples, const Port& inPort, State& ioState)
        {
            ProcessType gain            = ioState.mCurrentGain;
            const ProcessType target    = inPort.mTargetGain;
            const ProcessType epsilon   = 1e-9f;
            const ProcessType delta     = target - gain;
            const ProcessType deltaAbs  = std::abs(delta);

            const ProcessType* src      = inSrc;
            ProcessType* dest           = outDest;

            if (deltaAbs < epsilon)
            {
                gain = target;
                for (int i = 0; i < inNumSamples; ++i)
                {
                    *dest++ = *src++ * gain;
                }
            }
            else
            {
                const ProcessType slew = .0025f;
                for (int i = 0; i < inNumSamples; ++i)
                {
                    gain += slew * (target - gain);
                    *dest++ = *src++ * gain;
                }
            }
            ioState.mCurrentGain = gain;
        }
    };

    // -------------------------------------------------------------------------

    struct IIR
    {
        struct Port
        {
            float32 mCoefficients[6];
        };

        struct State
        {
            float32 mX;
            float32 mY;
        };

        static inline void reset(State& ioState)
        {
            ioState.mX = 0.f;
            ioState.mY = 0.f;
        }
        static inline void process(const ProcessType* inSrc, ProcessType* outDest,
                                   int inNumSamples, const Port& inPort, State& ioState)
        {
            const float32 a0 = inPort.mCoefficients[0];
            const float32 a1 = inPort.mCoefficients[1];
            const float32 a2 = inPort.mCoefficients[2];

            const float32 b1 = inPort.mCoefficients[4];
            const float32 b2 = inPort.mCoefficients[5];

            const ProcessType* src  = inSrc;
            ProcessType* dest       = outDest;

            float32 x = ioState.mX;
            float32 y = ioState.mY;

            for (int j = 0; j < inNumSamples; ++j)
            {
                const ProcessType in = *src++;
                const ProcessType out = a0 * in + x;
                *dest++ = out;

                x = a1 * in - b1 * out + y;
                y = a2 * in - b2 * out;
            }

            ioState.mX = dsp_denormalize_32(x);
            ioState.mY = dsp_denormalize_32(y);
        }
    };

}
