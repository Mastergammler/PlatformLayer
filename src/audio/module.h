#pragma once

#include "../define.h"
#include "events.h"
#include <atomic>
#include <string>

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
    // channels etc
    // volume
    // loop info
};

void audio_init();
void audio_update();
void audio_dispose();

void audio_load_sound(Audio& sound, std::string file);
void audio_start_playback(Playback pb);
void audio_stop_playback(Playback pb);

extern std::atomic<int> AudioEvent;
extern std::atomic<int> FramesPassed;

// TODO: loudness etc info
