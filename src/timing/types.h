#pragma once

#include <chrono>

using timepoint = std::chrono::steady_clock::time_point;

struct Clock
{
    int target_fps;

    int fps;
    int fps_min = 10000;
    int fps_max = 0;

    /**
     * frame time (since the last frame)
     * Unit: ms
     */
    float frame_time;

    /**
      Time since the last update call
     * When update is called every frame = frame time
     * Delta time is in seconds?
     * This is the actual passed time, so all IO system based things
     * should use this time
     *
     * Unit: s
     */
    float delta_time_real;

    float time_scale = 1;

    /**
     * this is the scaled delta time (simulation time)
     * it depends on time scale
     * So all entities and game systems should use this time
     *
     * Unit: s
     */
    float sim_time;

    float delta_min = 10000;
    float delta_max = 0;

    /**
     * For counting the currently elapsed time
     * for threshold updates
     *
     * NOTE: not sure if this is the correct solution
     * since we need to modify this value in order to
     * update use it, so it can only be used once at a time
     */
    float time_counter;
    timepoint last_time;
    timepoint start_time;
};
