#include "../internal.h"

void beat_update(BeatCounter& clock)
{
    timer_update(clock.timer);
    clock.elapsed += clock.timer.sim_time;

    // TODO: i can mess up beat counting with the offset quite easily
    // -> especially for subdivisions
    assert(abs(clock.offset) < clock.time_per_beat);
    for (int i = 0; i < clock.subb_count; i++)
    {
        SubbeatCounter* div = &clock.subbs[i];
        // FIXME: this seems to drift a bit towards the end of the song
        //-> we're loosing precision, with double it seems to work for now
        //=> But it's probably not a perfect solution
        double divDec = (clock.elapsed + clock.offset) / div->time_per_subb;
        // NOTE: divDec is 0 based, so we don't need the +1 here!
        if (divDec >= div->current_subb)
        {
            div->current_subb++;
            div->subb_changed_this_frame = true;
        }
        else
        {
            div->subb_changed_this_frame = false;
        }
    }
}

SubbeatCounter* beat_find_subb(BeatCounter& clock, float subbsPerBeat)
{
    for (int i = 0; i < clock.subb_count; i++)
    {
        if (clock.subbs[i].subbs_per_beat == subbsPerBeat)
            return &clock.subbs[i];
    }

    ASSERT(false,
           format("Subdivision for %.2f is not defined", subbsPerBeat).c_str());
    return {};
}
