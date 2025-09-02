#pragma once

#include "../define.h"
#include "events.h"
#include "types.h"
#include <atomic>
#include <string>

extern PlaybackPool Playbacks;

void audio_init();
void audio_update();
void audio_dispose();

void audio_load_sound(Audio& sound, std::string file);
void audio_start_playback(Playback pb);
void audio_stop_playback(Playback pb);

extern std::atomic<int> AudioEvent;
extern std::atomic<int> FramesPassed;

/*
 * Master volume factor logarithmically scaled
 * To match listener perception (linear loudness increase)
 */
extern float MasterVolume;

u16 clip(int32_t sample);
u16 mix_and_clip(u16 a, u16 b);
u16 adjust_volume(u16 sample, float factor);

float volume_to_gain(float sliderValue);

// TODO: loudness etc info
