#include "internal.h"

#include "beat.h"
#include "collision.h"
#include "draw.h"
#include "player.h"
#include "player/types.h"
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

    // TODO: run idle animation first
    //-> level starts as soon as the player presses left or right
    beat_update(SongClock);
    player_update(Ninja);
    world_update(World);

    collision_box_player(Ninja);

    draw_world();
    draw_player();
    draw_ui();

    // start audio
    if (GameInputs.Right.pressed && !Started)
    {
        Ninja.state = WALKING;
        audio_start_playback(Audio.songPb);
        Started = true;
        logf("Player started music");
        beat_start(SongClock);
        GroundIdx = (++GroundIdx % 2) + GroundOffset;
        MusicStarted = true;
    }

    // sync audio
    if (!MusicStarted && AudioEvent.load() == AUDIO_START)
    {
        audio_start_playback(Audio.songPb);
        logf("Music start signal received");
        beat_start(SongClock);
        GroundIdx = (++GroundIdx % 2) + GroundOffset;
        MusicStarted = true;
    }
    // hot reload functionality
    if (GameInputs.ReloadConfig.released)
    {
        input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);
        log("Keybindings reloaded!");
    }
}
