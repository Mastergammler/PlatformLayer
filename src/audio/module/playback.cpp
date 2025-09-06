#include "../internal.h"

#define MAX_CONCURRENT_SOUNDS 6
PlaybackPool Playbacks = {new Playback[MAX_CONCURRENT_SOUNDS](),
                          MAX_CONCURRENT_SOUNDS};

void audio_start_playback(Playback pb, bool resetToStart)
{
    for (int i = 0; i < Playbacks.max_size; i++)
    {
        if (!Playbacks.data[i].is_playing)
        {
            // TODO: not thread safe, but just a bool so it's probably fine?
            //  -> but i'm also updating the whole object here?
            Playbacks.data[i] = pb;
            if (resetToStart)
            {
                Playbacks.data[i].cursor_position = 0;
            }
            Playbacks.data[i].is_playing = true;

            return;
        }
    }

    logf("Unable to play audio '%s' because no open slots found",
         pb.data->file.c_str());
}

void audio_stop_playback(Playback* pb)
{
    for (int i = 0; i < Playbacks.max_size; i++)
    {
        if (Playbacks.data[i].is_playing &&
            Playbacks.data[i].playback_id == pb->playback_id)
        {
            Playbacks.data[i].is_playing = false;
            pb->cursor_position = Playbacks.data[i].cursor_position;
            return;
        }
    }

    logf("Requested to stop playing '%s' but no ACTIVE playback with id %i was "
         "found",
         pb->data->file.c_str(),
         pb->playback_id);
}

/**
 * This will stop all audio, but will not reset playback states
 */
void audio_stop_all()
{
    for (int i = 0; i < Playbacks.max_size; i++)
    {
        if (Playbacks.data[i].is_playing)
        {
            Playbacks.data[i].is_playing = false;
        }
    }
}
