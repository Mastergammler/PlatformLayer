#include "internal.h"

static bool Running = true;
Clock GameClock = {};
DrawBuffer Buffer = {};
static v2 TileSize = {32, 32};
static v2 ScreenTiles = {10, 6};
static WindowInfo WinIn = {3};

using namespace std;

void init_win_size()
{
    WinIn.draw_size = (TileSize * ScreenTiles);
    WinIn.display_size = WinIn.draw_size * WinIn.scale;
    logf("Drawsize %s, DisplaySize %s",
         str(WinIn.draw_size).c_str(),
         str(WinIn.display_size).c_str());
}

void set_fps_text()
{
    if (time_to_update(GameClock, 0.25))
    {
        string fpsTitle = format("%i FPS - %.1f ms/f",
                                 GameClock.fps,
                                 GameClock.frame_time);
        platform_window_title(fpsTitle);
    }
}

void engine_start()
{
    GameClock.target_fps = 1000;

    logger_initialize({});
    init_win_size();
    platform_init();
    platform_open_window(WinIn);
    timer_start(GameClock);
    audio_init();
    rendering_init_buffer(Buffer,
                          WinIn.draw_size.width,
                          WinIn.draw_size.height);

    game_init();

    while (Running)
    {
        timer_update(GameClock);
        input_reset_all_keys();
        platform_handle_messages();
        game_update();
        platform_draw(Buffer, WinIn);
        set_fps_text();
        timer_wait_till_next_frame(GameClock);
    }

    game_dispose();
    audio_dispose();
    platform_dispose();

    log("Engine shutdown");
    logger_dispose();
}

void engine_stop()
{
    Running = false;
}
