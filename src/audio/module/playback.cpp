#include "../internal.h"

void audio_start_playback(Playback pb)
{
    ma_result result;
    result = ma_device_start(&Device);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Device could not be started for playback: %i", result);
    }
}

void audio_stop_playback(Playback pb)
{
    ma_result result;
    result = ma_device_stop(&Device);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Device could not be stopped for playback: %i", result);
    }
}
