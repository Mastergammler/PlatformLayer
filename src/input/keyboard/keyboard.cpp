#include "../internal.h"

void input_reset_all_keys()
{
    for (int i = 0; i < InputStates->count; i++)
    {
        KeyInput* currentKey = &InputStates->key_states[i];

        currentKey->released = false;
        currentKey->pressed = false;
    }
}

void SetKeyState(KeyInput& key, bool wasDown, bool isDown)
{
    assert(wasDown != isDown);

    if (!wasDown && isDown)
    {
        key.pressed = true;
        key.is_down = true;
    }
    else
    {
        key.released = true;
        key.is_down = false;
    }
}

void input_notify_key(KeyData keyData)
{
    for (int i = 0; i < InputStates->count; i++)
    {
        KeyInput* input = &InputStates->key_states[i];
        if (input->keycode == keyData.key_code)
        {
            SetKeyState(*input, keyData.was_down, keyData.is_down);
        }
    }
}
