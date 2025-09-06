#include "../internal.h"
#include <climits>
#include <cstdint>

#define UNSIGNED_CONVERSION 32678
#define PCM16MAX UINT16_MAX
const int PCM16MIN = -(UINT16_MAX + 1);

u16 clip(int64_t sample)
{
    // FIXME: this looks very wrong
    //-> im going from MAX 65000 to min -32600
    if (sample > PCM16MAX)
        sample = PCM16MAX;
    else if (sample < PCM16MIN)
        sample = PCM16MIN;
    // TEST: this somehow fucks up the low end?
    //  -> but i did barely notice???
    //  -> So this is like, one sided clipping??? Interesting
    /*else if (sample < -UNSIGNED_CONVERSION / 16)
        sample = -UNSIGNED_CONVERSION / 16;*/

    return (u16)sample; //(sample + UNSIGNED_CONVERSION);
}

u16 mix_defensively(u16 a, u16 b)
{
    // convert to signed
    int16_t s1 = (int16_t)a;
    int16_t s2 = (int16_t)b;

    int32_t mixed = s1 / 2 + s2 / 2;

    // TODO: this should actually never clip?!
    // -> But i need the conversion i tink?
    // return clip(mixed);
    return /*(u16)*/ mixed;
}

u16 inverse_clip(int32_t sample)
{
    if (sample > PCM16MAX)
    {
        int32_t overflow = sample - PCM16MAX;
        sample = PCM16MAX - overflow;
    }
    else if (sample < PCM16MIN)
    {
        int32_t underflow = sample - PCM16MIN;
        sample = PCM16MIN - underflow;
    }

    return (u16)sample;
}

/**
 * INT16 MAX * 1.5
 * Dunno the lower value works better, or is this then just loudness scaling?
 * Not quite right? -> what is it exactly?
 */
#define Q15_x1_5 24567
// #define Q15_x1_5 49152
/**
 * INT16 MAX * 0.5
 */
#define Q15_x0_5 16384

#define Q15_x1 32678

/**
 * Threshold of 0.7
 */
#define Q15_THRESHOLD 22938

/**
 * NOTE: this reduced clipping artifacts but still clips
 * apparently this is more a wave-shaper than a soft clip
 * so this would need a clamped int16 input!
 */
int32_t waveshape_clip_q15(int32_t sample)
{
    if (sample <= Q15_THRESHOLD && sample >= -Q15_THRESHOLD) return sample;

    // TODO: i'm still not sure if this should be int64 instead
    //-> because else we would already clamp during the multiplication no?
    //=> And the output result would be already modified?
    int32_t x = sample;
    int32_t x2 = ((int64_t)x * x) >> 15;
    int32_t x3 = ((int64_t)x2 * x) >> 15;

    int32_t y = (((int64_t)Q15_x1_5 * sample) >> 15) -
                (((int64_t)Q15_x0_5 * x3) >> 15);

    return y;
    // return clip(y);
}

u16 soft_clip_q15(int32_t sample)
{
    /*int32_t x_q15 = sample >> 15;

    int32_t abs_x = (x_q15 < 0) ? -x_q15 : x_q15;
    int32_t denom = Q15_x1 + abs_x;
    int64_t num = x_q15;

    int64_t y = (num << 15) / denom;*/

    int32_t y = waveshape_clip_q15(sample);
    return inverse_clip(y);
}

u16 mix_q15_soft_clipping(u16 a, u16 b)
{
    int16_t s1 = (int16_t)a;
    int16_t s2 = (int16_t)b;

    int32_t mixed = s1 + s2;

    return soft_clip_q15(mixed);
}

u16 mix_inverse_clipping(u16 a, u16 b)
{
    int16_t s1 = (int16_t)a;
    int16_t s2 = (int16_t)b;

    int32_t mixed = s1 + s2;
    return inverse_clip(mixed);
}

u16 adjust_volume(u16 sample, float factor)
{
    int16_t value = (int16_t)sample;
    int32_t adjusted = value * factor;

    return clip(adjusted);
}
