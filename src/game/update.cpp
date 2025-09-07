#include "internal.h"

#include "beat.h"
#include "collision.h"
#include "draw.h"
#include "player.h"
#include "types.h"
#include "world.h"

void game_update()
{
    if (GameInputs.Exit.released) engine_stop();

    if (GameInputs.NudgeLeft.pressed)
    {
        SongClock.offset -= NUDGE_STEPS;
    }
    else if (GameInputs.NudgeRight.pressed)
    {
        SongClock.offset += NUDGE_STEPS;
    }

    if (GameInputs.Help.pressed)
    {
        if (Game.current_state == LEVEL_STARTED)
        {
            SongClock.timer.time_scale = 0;
            timer_update(SongClock.timer);
            audio_stop_playback(&Audio.song);
            Game.current_state = LEVEL_PAUSED;
        }
        else if (Game.current_state == LEVEL_PAUSED)
        {
            audio_start_playback(Audio.song, false);
            SongClock.timer.time_scale = 1;
            timer_update(SongClock.timer);
            Game.current_state = LEVEL_STARTED;
        }
    }

    // log previous 16 entries
    if (GameInputs.Action.pressed)
    {
        int readCount = 16;
        int writerIndex = PerformanceInfo.writer_index.load();
        int startIdx = writerIndex - readCount;
        if (startIdx < 0) startIdx = 0;

        for (int i = startIdx; i < writerIndex; i++)
        {
            AtPerformanceInfo cur = PerformanceInfo.info_buffer[i];
            logf("AT Perf: %i, %.3f ms (cb time), %.3f ms (dsp time)",
                 cur.iteration,
                 cur.time_since_last_callback * 1000,
                 cur.dsp_time);
        }
    }

    if (Game.current_state == LEVEL_STARTED || Game.current_state == PLAYER_WON)
    {
        beat_update(SongClock);
        world_update(World);

        collision_box_player(Ninja);
        player_update(Ninja);
    }

    draw_background();
    draw_foreground();
    draw_player();
    draw_ui();

    // start/reset audio
    if (GameInputs.Restart.pressed && Game.current_state != LEVEL_PAUSED)
    {
        World.start_index = 0;
        audio_stop_playback(&Audio.song);
        player_reset(Ninja, GridSize16x16);
        beat_reset(SongClock);
        audio_start_playback(Audio.song);
        GroundIdx = (++GroundIdx % 2) + GroundOffset;
        Game.current_state = LEVEL_STARTED;
        logf("Level was reset");
    }

    // hot reload functionality
    if (GameInputs.ReloadConfig.released)
    {
        input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);
        log("Keybindings reloaded!");
    }
}
