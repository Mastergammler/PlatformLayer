#include "internal.h"

#include "beat.h"
#include "collision.h"
#include "draw.h"
#include "player.h"
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

    if (!Stop)
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
    if (GameInputs.Restart.pressed)
    {
        World.start_index = 0;
        audio_stop_playback(Audio.song);
        player_reset(Ninja, GridSize16x16);
        beat_reset(SongClock);
        audio_start_playback(Audio.song);
        GroundIdx = (++GroundIdx % 2) + GroundOffset;
        MusicStarted = true;
        Stop = false;
        PlayerWon = false;
        logf("Level was reset");
    }

    // hot reload functionality
    if (GameInputs.ReloadConfig.released)
    {
        input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);
        log("Keybindings reloaded!");
    }
}
