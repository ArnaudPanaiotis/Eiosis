/*!
 * \file       framework_DSP.h
 * Copyright   Eiosis 2014
 */

#pragma once

#include <cmath>

namespace dsp
{
    typedef float   float32;
    typedef double  float64;

    typedef float32 ProcessType;

    static struct Zero
    {
        inline operator float64() const { return 0.; }
        inline operator float32() const { return 0.f; }
    } zero;

    static struct One
    {
        inline operator float64() const { return 1.; }
        inline operator float32() const { return 1.f; }
    } one;

    static struct Pi
    {
        inline operator float64() const { return 3.1415926535897932384626433832796; }
        inline operator float32() const { return 3.14159265358979323846f; }
    } pi;

    static struct TwoPi
    {
        inline operator float64() const { return 6.283185307179586476925286766559; }
        inline operator float32() const { return 6.28318530717958647693f; }
    } twoPi;

    static struct HalfPi
    {
        inline operator float64() const { return 1.5707963267948966192313216916398; }
        inline operator float32() const { return 1.57079632679489661923f; }
    } halfPi;

    static struct Sqrt2
    {
        inline operator float64() const { return 1.4142135623730950488016887242093; }
        inline operator float32() const { return 1.41421356237309504880f; }
    } sqrt2;

    static struct E
    {
        inline operator float64() const { return 2.7182818284590452353602874713526; }
        inline operator float32() const { return 2.71828182845904523536f; }
    } e;

    static const float32 pi_32      = pi;
    static const float64 pi_64      = pi;

    static const float32 twoPi_32   = 2.f * pi_32;
    static const float64 twoPi_64   = 2. * pi_64;

    static const float32 halfPi_32  = .5f * pi_32;
    static const float64 halfPi_64  = .5 * pi_64;

    static const float32 sqrt2_32   = sqrt2;
    static const float64 sqrt2_64   = sqrt2;

    static const float32 e_32       = e;
    static const float64 e_64       = e;

    inline float32 dBToLinear(float32 inValue)
    {
        static const float32 factor = .05f * std::log(10.f);
        return std::exp(factor * inValue);
    }

    inline float64 dBToLinear(float64 inValue)
    {
        static const float64 factor = .05 * std::log(10.);
        return std::exp(factor * inValue);
    }

    inline float32 linearTodB(float32 inValue)
    {
        return 20.f * std::log10(inValue);
    }

    inline float64 linearTodB(float64 inValue)
    {
        return 20. * std::log10(inValue);
    }
}

#define dsp_denormalize_32(inValue) ((inValue) > -1e-37f && (inValue) < 1e-37f ? 0.f : (inValue))
#define dsp_denormalize_64(inValue) ((inValue) > -1e-307 && (inValue) < 1e-307 ? 0. : (inValue))

