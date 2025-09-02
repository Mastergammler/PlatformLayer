#include "../internal.h"

#define GAIN_MIN -60.
#define GAIN_MAX -3.
#define LOG_BASE 10.
#define DECIBEL_FACTOR 20.

float volume_to_gain(float sliderValue)
{
    // clamp
    if (sliderValue > 1) sliderValue = 1.;
    if (sliderValue < 0) sliderValue = 0.;

    float db = GAIN_MIN + sliderValue * (GAIN_MAX - GAIN_MIN);

    return pow(LOG_BASE, db / DECIBEL_FACTOR);
}
