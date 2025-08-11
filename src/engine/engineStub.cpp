#include "internal.h"

static bool Running = true;
static Clock MainClock = {};

using namespace std;

void set_fps_text()
{
    if (time_to_update(MainClock, 0.25))
    {
        string fpsTitle = format("%i FPS - %.1f ms/f",
                                 MainClock.fps,
                                 MainClock.frame_time);
        platform_window_title(fpsTitle);
    }
}

void engine_start()
{
    logger_initialize({});
    platform_init();
    platform_open_window();
    game_init();
    timer_start(MainClock);

    MainClock.target_fps = 144;

    while (Running)
    {
        timer_update(MainClock);
        input_reset_all_keys();
        platform_handle_messages();
        game_update();
        // TODO: render step
        set_fps_text();
        timer_wait_till_next_frame(MainClock);
    }

    game_dispose();
    platform_dispose();

    log("Engine shutdown");
    logger_dispose();
}

void engine_stop()
{
    Running = false;
}
