#pragma once

#include "types.h"

#include <string>

/**
 * Resets the pressed and released states for each key,
 * because these are one time events
 *
 * Needs to be called every time before messages are handled
 */
void input_reset_all_keys();
void input_notify_key(KeyData keyData);

/**
 * Loads the key configuration for input handling
 */
void input_init_keyboard(InputState* inputs,
                         const std::string mappingFile,
                         const std::string keycodeFile);
