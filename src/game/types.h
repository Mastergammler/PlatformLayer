#pragma once

#include "internal.h"

// TODO: NVIM - create a generator for this?
struct GameInputState : InputState
{
    GameInputState(KeyInput* inputs, int count)
        : InputState{inputs, count}, Exit(key_states[0]), Action(key_states[1]),
          Jump(key_states[2]), Help(key_states[3]), ReloadConfig(key_states[4]),
          Up(key_states[5]), Down(key_states[6]), Left(key_states[7]),
          Right(key_states[8])
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
};
