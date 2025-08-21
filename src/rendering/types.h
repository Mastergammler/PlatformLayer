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

template <typename T> struct Matrix
{
    int rows;
    int columns;
    int cell_count;

    T* data;

    T& operator()(int row, int col)
    {
        return data[row * columns + col];
    }
};

/**
 * Bitmap font refers to a font that is loaded based on a sprite sheet
 * So bascially a pre-rasterised font that is just displayed like any
 * other image
 */
struct BitmapFont
{
    /**
     * ASCII offset for the number indices of the sprite
     * E.g.: 0 = ASCII 48  SpriteIdx = 26
     *       Offset = -22 => ASCII - 22 = 26
     */
    int digit_offset;

    /**
     *  Upper letter ASCII offset of the sprite indices
     */
    int upper_offset;

    /**
     * Lower letter ASCII offset for the sprite indices
     */
    int lower_offset;
    SpriteSheet* sprites;
};
