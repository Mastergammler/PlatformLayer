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
    bool loop;

    /* will not be canceled */
    bool keep_alive;
    int cursor_position;

    /* Between 0 and 1? */
    float volume = 1;
    /* do this in here? */
    float pan;
    bool is_playing;
};

struct PlaybackPool
{
    Playback* data;
    int max_size;
};

struct AtPerformanceInfo
{
    int iteration;
    /* unit s */
    float time_since_last_callback;
    /* unit ms */
    float dsp_time;

    // check in case didn't finish (XRun)
    bool measure_finished;
};

struct PerformanceInfoBuffer
{
    /*
     * usually audio thread
     * do not modify from game thread
     * audio thread modifies atomic
     * audio thread does not need to read atomic
     */
    std::atomic<int> writer_index;

    /*
     * usually game thread
     * audio thread doesn't touch this one
     */
    int reader_index;

    AtPerformanceInfo* info_buffer;
    int buffer_size;
};
