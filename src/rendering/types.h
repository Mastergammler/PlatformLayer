#pragma once

#include "../define.h"

struct DrawBuffer
{
    void* memory;
    int width;
    int height;
    int pixel_count;
    size_t size;
};
