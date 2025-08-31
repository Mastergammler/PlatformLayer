#pragma once

#include "../audio/types.h"
#include "../input/types.h"
#include "../rendering/types.h"

// TODO: NVIM - create a generator for this?
struct GameInputState : InputState
{
    GameInputState(KeyInput* inputs, int count)
        : InputState{inputs, count}, Exit(key_states[0]), Action(key_states[1]),
          Jump(key_states[2]), Help(key_states[3]), ReloadConfig(key_states[4]),
          Up(key_states[5]), Down(key_states[6]), Left(key_states[7]),
          Right(key_states[8]), NudgeLeft(key_states[9]),
          NudgeRight(key_states[10]), Restart(key_states[11])
    {
    }

    KeyInput& Exit;
    KeyInput& Action;
    KeyInput& Jump;
    KeyInput& Help;
    KeyInput& ReloadConfig;
    KeyInput& Up;
    KeyInput& Down;
    KeyInput& Left;
    KeyInput& Right;
    KeyInput& NudgeLeft;
    KeyInput& NudgeRight;
    KeyInput& Restart;
};

struct SpriteShelf
{
    SpriteSheet PlayerWalking = {};
    SpriteSheet PlayerIdle = {};
    SpriteSheet PlayerHit = {};
    SpriteSheet PlayerJump = {};
    SpriteSheet FontSprites = {};
    SpriteSheet GroundSprites = {};
    BitmapFont Font = {};
};

struct AudioShelf
{
    Audio pcm_music;
    Audio pcm_hit;
    Audio pcm_jump;
    Audio pcm_land;
    Playback song = {&pcm_music, 1};
    Playback hit = {&pcm_hit, 2};
    Playback jump = {&pcm_jump, 4};
    Playback land = {&pcm_land, 5};
};
