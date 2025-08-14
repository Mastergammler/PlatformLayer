#pragma once

#include "../define.h"
#include <string>

using std::string;

struct DrawBuffer
{
    void* memory;
    int width;
    int height;
    int pixel_count;
    size_t size;
};

struct SpriteBuffer
{
    bool loaded;
    string file;

    int width;
    int height;
    int channels;
    // TODO: should store the data as array if possible
    //  that we can control allocation
    u8* pixels;
};
