#pragma once

#include "vector.h"
#include <cstdint>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct WindowInfo
{
    float scale = 1;
    bool allow_resize = false;

    /*
     * Size of the draw buffer
     * Actually drawn pixels
     */
    Dim draw_size;

    /*
     * Size in which the buffer is displayed
     * Scaled size of the drawings
     */
    Dim display_size;

    /*
     * Size of the window including border and taskbar
     * Needs to be determined by the platform
     */
    Dim window_size;
};
