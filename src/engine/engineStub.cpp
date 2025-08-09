#include "internal.h"

static bool Running = true;

void engine_start()
{
    logger_initialize({});
    platform_init();
    platform_open_window();
    game_init();

    while (Running)
    {
        input_reset_all_keys();
        platform_handle_messages();
        game_update();
        // TODO: render step
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
