#include "Leds.hpp"
#include "GammaCorrection.hpp"
#include "helpers/freertos.hpp"

void Leds::taskMain(void *)
{
    initTimers();

    while (true)
    {
        fillSineArray();
        circlePointer += 16;
        circlePointer %= SineTableSize;

        for (size_t i = 0; i < sineBrightness.size(); i++)
            setLedBrightness(i, sineBrightness[i]);

        vTaskDelay(toOsTicks(10.0_ms));
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Leds::fillSineArray()
{
    constexpr auto DiffBetweenLeds =
        static_cast<size_t>(std::numbers::pi * 2 * 1000 / (NumberOfLeds));

    for (size_t i = 0; i < NumberOfLeds; i++)
    {
        const auto LedPosition =
            ((NumberOfLeds - i) * DiffBetweenLeds + circlePointer) % SineTableSize;
        const size_t Brightness = SineLUT[LedPosition] < 0 ? 0 : SineLUT[LedPosition];

        sineBrightness[i] = Brightness;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void Leds::initTimers()
{
    for (const auto &timerAndChannel : LedTimersAndChannels)
    {
        HAL_TIM_PWM_Start(timerAndChannel.first, timerAndChannel.second);
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void Leds::setLedBrightness(uint8_t ledIndex, size_t newBrightness)
{
    if (ledIndex >= LedTimersAndChannels.size())
        return;

    auto &timerAndChannel = LedTimersAndChannels[ledIndex];
    __HAL_TIM_SET_COMPARE(timerAndChannel.first, timerAndChannel.second,
                          GammaCorrectionLUT[newBrightness]);
}