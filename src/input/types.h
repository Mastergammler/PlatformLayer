#pragma once

#include <cstdint>
#include <string>

struct KeyData
{
    bool was_down;
    bool is_down;
    uint32_t key_code;
};

struct KeyInput
{
    /**
     * Name of the key used, that is user understandable Name that can be used
     * in text, etc
     */
    std::string key_name = "init";

    /**
     * Key identifier, that corresponds to the mapping file
     */
    std::string identifier = "init";

    /**
     * TODO: Win32 specific?! ...
     * Keycode used by the Win32 Event system to identify keys
     */
    uint32_t keycode;

    /**
     * If the button is down right now (this frame)
     */
    bool is_down;

    /**
     * Single frame state, when the button just got pressed (this frame)
     */
    bool pressed;

    /**
     * Single frame state, when the button just got released (this frame)
     */
    bool released;
};

struct InputState
{
    KeyInput* key_states;
    int count;
};
