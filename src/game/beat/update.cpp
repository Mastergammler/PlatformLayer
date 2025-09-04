#include "../internal.h"

void beat_update(BeatCounter& clock)
{
    timer_update(clock.timer);
    clock.elapsed += clock.timer.sim_time;

    // TODO: i can mess up beat counting with the offset quite easily
    // -> especially for subdivisions
    assert(abs(clock.offset) < clock.time_per_beat);
    for (int i = 0; i < clock.division_count; i++)
    {
        DivisionCounter* div = &clock.divisions[i];
        float divDec = (clock.elapsed + clock.offset) / div->time_per_division;
        // NOTE: divDec is 0 based, so we don't need the +1 here!
        if (divDec >= div->current_division)
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

DivisionCounter* beat_find_division(BeatCounter& clock, float divisionsPerBeat)
{
    for (int i = 0; i < clock.division_count; i++)
    {
        if (clock.divisions[i].divisions_per_beat == divisionsPerBeat)
            return &clock.divisions[i];
    }

    ASSERT(false,
           format("Subdivision for %.2f is not defined", divisionsPerBeat)
                                                   .c_str());
    return {};
}
