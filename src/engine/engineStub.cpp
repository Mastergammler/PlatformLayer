#include "internal.h"

static bool running = true;

void engine_start()
{
    Logger_Init({});
    platform_init();
    platform_open_window();
    Log("Engine initialized - starting main loop");

    while (running)
    {
        platform_handle_messages();
        game_update();
    }

    platform_dispose();

    Log("Engine shutdown");
    Logger_Dispose();
}

void engine_stop()
{
    running = false;
}
