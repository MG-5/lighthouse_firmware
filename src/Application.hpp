#pragma once

#include "tim.h"

#include "Leds.hpp"
#include "helpers/freertos.hpp"

/// The entry point of users C++ firmware. This comes after CubeHAL and FreeRTOS initialization.
/// All needed classes and objects have the root here.
class Application
{
public:
    static constexpr auto LedTimerPrime = &htim2;
    static constexpr auto LedTimerSecundus = &htim3;

    Application();
    void run();

    static Application &getApplicationInstance();

private:
    static inline Application *instance{nullptr};

    Leds leds{LedTimerPrime, LedTimerSecundus};
};
