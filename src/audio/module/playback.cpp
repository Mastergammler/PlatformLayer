#include "../internal.h"

#define MAX_CONCURRENT_SOUNDS 6
#define INDEX_TRACK_INIT -1
PlaybackPool Playbacks = {new Playback[MAX_CONCURRENT_SOUNDS](),
                          MAX_CONCURRENT_SOUNDS};

/**
 * Schedules the next audio in an open slot
 * If no open slots if found, an old sound will be aborted
 * UNLESS it's marked with the 'keep_alive' flag
 *
 * TEST: does this work well with other sounds, are there any longer fx?
 *  -> would these than just need to be 'keep_alive' ?
 * => First impression is, that it works very nicely
 */
void audio_start_playback(Playback newPlayback, bool resetToStart)
{
    int longestPlayingIdx = INDEX_TRACK_INIT;
    int furthestCursor = 0;

    for (int i = 0; i < Playbacks.max_size; i++)
    {
        if (!Playbacks.data[i].is_playing)
        {
            // TODO: not thread safe, but just a bool so it's probably fine?
            //  -> but i'm also updating the whole object here?
            Playbacks.data[i] = newPlayback;
            if (resetToStart)
            {
                Playbacks.data[i].cursor_position = 0;
            }
            Playbacks.data[i].is_playing = true;
            return;
        }
        else if (!Playbacks.data[i].keep_alive)
        {
            int currentCursor = Playbacks.data[i].cursor_position;
            if (longestPlayingIdx == INDEX_TRACK_INIT ||
                currentCursor > furthestCursor)
            {
                longestPlayingIdx = i;
                furthestCursor = currentCursor;
            }
        }
    }

    if (longestPlayingIdx != INDEX_TRACK_INIT)
    {
        Playbacks.data[longestPlayingIdx] = newPlayback;
        if (resetToStart)
        {
            Playbacks.data[longestPlayingIdx].cursor_position = 0;
        }
        Playbacks.data[longestPlayingIdx].is_playing = true;
    }
    else
    {
        logf("Unable to play audio '%s' because no open or cancelable slots "
             "found",
             newPlayback.data->file.c_str());
    }
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
