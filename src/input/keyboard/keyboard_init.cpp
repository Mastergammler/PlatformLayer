#include "../internal.h"

KeyInput* FindInput(InputState* input, string name)
{
    for (int i = 0; i < input->count; i++)
    {
        if (input->key_states[i].identifier == name)
            return &input->key_states[i];
    }

    return NULL;
}

// TODO: move to parsing utils
bool IsChar(const string& s)
{
    return s.length() == 1;
}

size_t GetSpecialKeyCode(unordered_map<string, uint32_t>* keycodes,
                         const string& keyId)
{
    if (keycodes->count(keyId) > 0)
    {
        return keycodes->find(keyId)->second;
    }
    else
    {
        logf("No special key with the name '%s' defined!", keyId.c_str());
        return 0;
    }
}

void input_init_keyboard(InputState* inputs,
                         const string mappingFile,
                         const string keycodeFile)
{
    InputStates = inputs;

    unordered_map<string, string> keybindings;
    unordered_map<string, uint32_t> keycodes;

    parse_kvp_file(&keybindings, mappingFile);
    parse_kvp_file(&keycodes, keycodeFile);

    for (const auto& [key, value] : keybindings)
    {
        KeyInput* input = FindInput(inputs, key);
        if (!input)
        {
            logf("Unused key mapping: %s", key.c_str());
            continue;
        }

        // TODO: OEM characters -> these would need to be handle special
        //  -> but could currently be input in the wrong format!
        if (IsChar(value))
        {
            // VK_KEYCODES always use the upper case value!
            input->keycode = toupper(value[0]);
            input->key_name = value;
        }
        else
        {
            input->keycode = GetSpecialKeyCode(&keycodes, value);
            input->key_name = value;
        }
    }
}
