#pragma once

#include "../../timing/module.h"
#include "../module.h"

#define BEAT_DIVISIONS 1
#define MEASURE_DIVISIONS 0.25

struct DivisionCounter
{
    bool division_changed_this_frame;
    u32 current_division;

    /** unit: s */
    float time_per_division;
    float divisions_per_beat;
};

struct BeatCounter
{
    float bpm;
    /** unit: s */
    float elapsed;
    /** unit: s */
    float offset;
    /** unit: s */
    float time_per_beat;
    // u32 current_beat;
    // u32 current_measure;
    int beats_per_measure;

    int division_count;
    DivisionCounter* divisions;
    Clock timer;

    // TODO: add pointer to audio file?
};
