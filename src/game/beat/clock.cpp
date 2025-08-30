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
    clock.current_beat = 1;
    clock.current_measure = 1;
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

DivisionCounter* beat_find_division(BeatCounter& clock, float divisionsPerBeat)
{
    for (int i = 0; i < clock.division_count; i++)
    {
        if (clock.divisions[i].divisions_per_beat == divisionsPerBeat)
            return &clock.divisions[i];
    }

    assert(false);
    return {};
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

void beat_update(BeatCounter& clock)
{
    if (!MusicStarted) return;

    timer_update(clock.timer);
    clock.elapsed += clock.timer.sim_time;

    // TODO: i can mess up beat counting with the offset quite easily
    // -> especially for subdivisions
    assert(abs(clock.offset) < clock.time_per_beat);
    for (int i = 0; i < clock.division_count; i++)
    {
        DivisionCounter* div = &clock.divisions[i];
        float divDec = (clock.elapsed + clock.offset) / div->time_per_division;

        // FIXME: there seems to be a off-by-one for the masures, but the rest
        // seems fine
        if (divDec >= div->current_division + 1)
        {
            div->current_division++;
            div->division_changed_this_frame = true;
        }
        else
        {
            div->division_changed_this_frame = false;
        }
    }
}
