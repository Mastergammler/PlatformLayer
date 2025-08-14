#pragma once

// TODO: is this good separation?
#include "rendering/types.h"
#include <cstddef>
#include <string>

void platform_init();
void platform_dispose();

// Window
void platform_window_title(std::string title);
void platform_open_window();
void platform_handle_messages();
void platform_draw(DrawBuffer& buffer);

// Allocation
void* platform_alloc(size_t size);
void platform_free(void* mem, size_t size);
