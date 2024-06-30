#pragma once

#include "gcem.hpp"
#include <array>
#include <cmath>
#include <cstdint>

constexpr auto PwmResolutionInBit = 10;

constexpr auto StartOffset = 0.0f;
constexpr auto GammaFactor = 4.5f;
constexpr auto MaximumIn = (1 << PwmResolutionInBit) - 1;
constexpr auto MaximumOut = (1 << PwmResolutionInBit) - 1;

using GammaTable = std::array<size_t, MaximumIn + 1>;

constexpr GammaTable createGammaTable()
{
    GammaTable gammaTable{};

    for (auto i = 1; i <= MaximumIn; i++)
    {
        const auto Logarithm =
            gcem::pow(static_cast<float>(i) / static_cast<float>(MaximumIn), GammaFactor);

        gammaTable[i] = StartOffset + gcem::round(Logarithm * (MaximumOut - StartOffset));
    }

    return gammaTable;
}

constexpr GammaTable GammaCorrectionLUT = createGammaTable();

constexpr auto SineTableSize = static_cast<size_t>(std::numbers::pi * 2 * 1000);
using SineTable = std::array<int, SineTableSize>;

constexpr SineTable createSineTable()
{
    SineTable sineTable{};

    for (size_t i = 0; i < SineTableSize; i++)
    {
        sineTable[i] = static_cast<int>(gcem::sin(i / 1000.0) * MaximumOut);
    }

    return sineTable;
}

constexpr SineTable SineLUT = createSineTable();
