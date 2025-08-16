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

struct PixelBuffer
{
    bool loaded;
    string file;
    Dim size;
    int channels;
    // TODO: should store the data as array if possible
    //  that we can control allocation
    u8* pixels;
};

struct SpriteSheet
{
    bool loaded;

    Dim tile_size;
    int channels;

    Dim source_image_size;
    v2 grid_size;

    PixelBuffer* tiles;
    int tile_count;
};

/*
 * Creates a logic unit of what should be drawn
 * A sprite can be multiple tiles wide or height
 */
struct Sprite
{
    /* size in tiles */
    v2 size;
    int sheet_start_index;

    SpriteSheet* sheet;
};
