#include "../internal.h"

/*
 * Using the cross platform api, i have to see how the precision is
 * Because the other way (QueryPerformanceCounter/Frequency, LARGE_INTEGER)
 * is windows specific
 */

void timer_start(Clock& clock)
{
    timepoint now = precision_clock::now();

    clock.start_time = now;
    clock.last_time = now;
    clock.time_counter = 0;

    if (clock.target_fps > 0)
        clock.delta_time_real = 1.f / clock.target_fps;
    else
        clock.delta_time_real = 1. / 60;
}

void timer_update(Clock& timer)
{
    timepoint now = precision_clock::now();

    timer.delta_time_real = duration_s(now - timer.last_time).count();
    timer.frame_time = timer.delta_time_real * 1000;
    timer.last_time = now;
    timer.fps = 1 / timer.delta_time_real;
    timer.time_counter += timer.delta_time_real;
    timer.sim_time = timer.time_scale * timer.delta_time_real;
}

/**
 * time since the last update call
 * Unit: ms
 */
float time_since_update(Clock& timer)
{
    timepoint now = precision_clock::now();
    return duration_ms(now - timer.last_time).count();
}

/**
 * time since the start of the timer
 * Unit: ms
 */
float time_since_start(Clock& timer)
{
    timepoint now = precision_clock::now();
    return duration_ms(now - timer.start_time).count();
}
