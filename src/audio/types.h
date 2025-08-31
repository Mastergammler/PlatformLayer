#pragma once

#include "../define.h"

/* Loaded audio file */
struct Audio
{
    bool loaded;

    int channels;
    int sample_rate;
    u16* pcm_data;
    u64 total_samples;
    u64 samples_per_channel;
    float length_s;

    int buffer_index;

    std::string file;
};

/* Audio playback options */
struct Playback
{
    Audio* data;
    int playback_id;
    bool is_playing;
    bool loop;
    int cursor_position;

    /* Between 0 and 1? */
    float volume = 1;
    /* do this in here? */
    float pan;
};

struct PlaybackPool
{
    Playback* data;
    int max_size;
};
