#include "../internal.h"

void timer_wait_till_next_frame(Clock& timer)
{
    float targetFrameTime = 1000. / timer.target_fps;
    if (timer.frame_time > targetFrameTime)
    {
        float timeToRest = targetFrameTime - timer.frame_time;

        // intentionally undersleeping to have higher precision
        // we start the busy wait loop already, when ttr > 1 (sleep 0)
        // instead of sleep(1), because shedulers guarantee to sleep
        // AT LEAST this amount of time, but might overshoot
        if (timeToRest > 1)
        {
            sleep_for(ms((int)timeToRest - 1));
        }
    }

    // busy wait
    while (time_since_update(timer) < targetFrameTime)
    {
        yield();
    }
}

bool time_to_update(Clock& timer, float updateIntervalS)
{
    if (timer.time_counter > updateIntervalS)
    {
        timer.time_counter -= updateIntervalS;
        return true;
    }
    return false;
}
