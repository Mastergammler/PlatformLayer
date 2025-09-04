#include "../internal.h"

#define DEFAULT_DIVISION_COUNT 2
static float DEFAULT_DIVISIONS[] = {BEAT_DIVISIONS, MEASURE_DIVISIONS};

void beat_init(BeatCounter& clock,
               float bpm,
               int beatsPerMeasure,
               float* divisions,
               int divisionCount)
{
    clock.bpm = bpm;
    clock.beats_per_measure = beatsPerMeasure;
    // clock.current_beat = 1;
    // clock.current_measure = 1;
    clock.time_per_beat = 60. / bpm;
    clock.division_count = divisionCount + DEFAULT_DIVISION_COUNT;
    clock.divisions = new DivisionCounter[clock.division_count]();

    for (int i = 0; i < clock.division_count; i++)
    {
        // user defined devisions
        if (i < divisionCount)
        {
            clock.divisions[i].divisions_per_beat = divisions[i];
        }
        else // default divisions
        {
            clock.divisions[i].divisions_per_beat = DEFAULT_DIVISIONS
                                                    [i - divisionCount];
        }
        clock.divisions[i].current_division = 1;
        clock.divisions[i].time_per_division = clock.time_per_beat /
                                               clock.divisions[i].divisions_per_beat;
    }
}

void beat_start(BeatCounter& clock)
{
    timer_start(clock.timer);

    // init first beat changes
    for (int i = 0; i < clock.division_count; i++)
    {
        clock.divisions[i].division_changed_this_frame = true;
    }
}

void beat_reset(BeatCounter& clock)
{
    // clock.current_beat = 1;
    // clock.current_measure = 1;

    // init first beat changes
    for (int i = 0; i < clock.division_count; i++)
    {
        clock.divisions[i].current_division = 1;
        clock.divisions[i].division_changed_this_frame = true;
    }

    timer_start(clock.timer);
    clock.elapsed = 0;
}
