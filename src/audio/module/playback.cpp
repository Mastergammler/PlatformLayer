#include "../internal.h"

#define MAX_CONCURRENT_SOUNDS 4
PlaybackPool Playbacks = {new Playback[MAX_CONCURRENT_SOUNDS](),
                          MAX_CONCURRENT_SOUNDS};

void audio_start_playback(Playback pb)
{
    for (int i = 0; i < Playbacks.max_size; i++)
    {
        if (!Playbacks.data[i].is_playing)
        {
            // TODO: not thread safe, but just a bool so it's probably fine?
            //  -> but i'm also updating the whole object here?
            Playbacks.data[i] = pb;
            Playbacks.data[i].is_playing = true;
            return;
        }
    }

    logf("Unable to play audio '%s' because no open slots found",
         pb.data->file.c_str());
}

// TODO: implement properly
void audio_stop_playback(Playback pb)
{
    ma_result result;
    result = ma_device_stop(&Device);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Device could not be stopped for playback: %i", result);
    }
}
