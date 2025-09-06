#include "../internal.h"

#define DEFAULT_SUBB_COUNT 2
static float DEFAULT_SUBBS[] = {BEAT_SUBBS, MEASURE_SUBBS};

void beat_init(BeatCounter& clock,
               float bpm,
               int beatsPerMeasure,
               float* subbs,
               int subbCount)
{
    clock.bpm = bpm;
    clock.beats_per_measure = beatsPerMeasure;
    // clock.current_beat = 1;
    // clock.current_measure = 1;
    clock.time_per_beat = 60. / bpm;
    clock.subb_count = subbCount + DEFAULT_SUBB_COUNT;
    clock.subbs = new SubbeatCounter[clock.subb_count]();

    for (int i = 0; i < clock.subb_count; i++)
    {
        // user defined devisions
        if (i < subbCount)
        {
            clock.subbs[i].subbs_per_beat = subbs[i];
        }
        else // default divisions
        {
            clock.subbs[i].subbs_per_beat = DEFAULT_SUBBS
                                                    [i - subbCount];
        }
        clock.subbs[i].current_subb = 1;
        clock.subbs[i].time_per_subb = clock.time_per_beat /
                                               clock.subbs[i].subbs_per_beat;
    }
}

void beat_start(BeatCounter& clock)
{
    timer_start(clock.timer);

    // init first beat changes
    for (int i = 0; i < clock.subb_count; i++)
    {
        clock.subbs[i].subb_changed_this_frame = true;
    }
}

void beat_reset(BeatCounter& clock)
{
    // clock.current_beat = 1;
    // clock.current_measure = 1;

    // init first beat changes
    for (int i = 0; i < clock.subb_count; i++)
    {
        clock.subbs[i].current_subb = 1;
        clock.subbs[i].subb_changed_this_frame = true;
    }

    timer_start(clock.timer);
    clock.elapsed = 0;
}
