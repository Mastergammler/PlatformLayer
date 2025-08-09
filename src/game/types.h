#pragma once

#include "internal.h"

// TODO: NVIM - create a generator for this?
struct GameInputState : InputState
{
    GameInputState(KeyInput* inputs, int count)
        : InputState{inputs, count}, Exit(key_states[0]), Action(key_states[1]),
          Jump(key_states[2]), Help(key_states[3]), ReloadConfig(key_states[4])
    {
    }

    KeyInput& Exit;
    KeyInput& Action;
    KeyInput& Jump;
    KeyInput& Help;
    KeyInput& ReloadConfig;
};
