#pragma once

#include "tim.h"
#include "wrappers/Task.hpp"

#include <array>
#include <cstdint>
#include <utility>

class Leds : public util::wrappers::TaskWithMemberFunctionBase
{
public:
    Leds(TIM_HandleTypeDef *ledTimerPrime, TIM_HandleTypeDef *ledTimerSecundus)
        : TaskWithMemberFunctionBase("ledTask", 1024, osPriorityNormal3), //
          ledTimerPrime(ledTimerPrime),                                   //
          ledTimerSecundus(ledTimerSecundus){};

    void setOverallBrightness(uint8_t newBrightness);
    void setLedBrightness(uint8_t ledIndex, size_t newBrightness);

protected:
    void taskMain(void *) override;

private:
    TIM_HandleTypeDef *ledTimerPrime = nullptr;
    TIM_HandleTypeDef *ledTimerSecundus = nullptr;

    using LedTimerAndChannel = std::pair<TIM_HandleTypeDef *, uint32_t>;

    static constexpr auto NumberOfLeds = 8;

    std::array<LedTimerAndChannel, NumberOfLeds> LedTimersAndChannels = {
        std::make_pair(ledTimerPrime, TIM_CHANNEL_1),    //
        std::make_pair(ledTimerPrime, TIM_CHANNEL_2),    //
        std::make_pair(ledTimerPrime, TIM_CHANNEL_3),    //
        std::make_pair(ledTimerPrime, TIM_CHANNEL_4),    //
        std::make_pair(ledTimerSecundus, TIM_CHANNEL_1), //
        std::make_pair(ledTimerSecundus, TIM_CHANNEL_2), //
        std::make_pair(ledTimerSecundus, TIM_CHANNEL_3), //
        std::make_pair(ledTimerSecundus, TIM_CHANNEL_4)};

    std::array<size_t, NumberOfLeds> sineBrightness;
    size_t circlePointer = 0;

    void initTimers();
    void fillSineArray();
};