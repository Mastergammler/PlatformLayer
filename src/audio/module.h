#pragma once

#include <string>

/* Loaded audio file */
struct Audio
{
    std::string file;

    bool loaded;
    // bytes etc
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

// TODO: loudness etc info
