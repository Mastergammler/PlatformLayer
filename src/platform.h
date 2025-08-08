#pragma once

#include <string>

/**
 * Code the platform layer has to implement in order for the engine to use it
 */
void platform_window_title(std::string title);

void platform_open_window();
void platform_init();
void platform_handle_messages();
void platform_dispose();
