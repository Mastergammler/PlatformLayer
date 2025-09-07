#pragma once

#include "../../timing/module.h"
#include "../module.h"

#define BEAT_SUBBS 1
#define MEASURE_SUBBS 0.25

struct SubbeatCounter
{
    bool subb_changed_this_frame;
    u32 current_subb;

    /** unit: s */
    float time_per_subb;
    float subbs_per_beat;
};

struct BeatCounter
{
    float bpm;
    /** unit: s */
    double elapsed;
    /** unit: s */
    float offset;
    /** unit: s */
    float time_per_beat;
    // u32 current_beat;
    // u32 current_measure;
    int beats_per_measure;

    int subb_count;
    SubbeatCounter* subbs;
    Clock timer;

    // TODO: add pointer to audio file?
};
