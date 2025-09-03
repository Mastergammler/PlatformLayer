#include "../internal.h"

void draw_background()
{
    if (BgChanged)
    {
        rendering_fill_screen_rng(BgCache,
                                  Sprites.GroundSprites,
                                  0,
                                  GroundOffset - 1);
        BgChanged = false;
    }

    // Render with offset
    u32* bufferStart = (u32*)Buffer.memory;
    u32* sourceStart = (u32*)BgCache.memory;
    for (int y = 0; y < Buffer.height; y++)
    {
        u32* targetPixel = bufferStart + y * Buffer.width;
        u32* sourcePixel = sourceStart + y * BgCache.width;
        for (int x = 0; x < Buffer.width; x++)
        {
            u32* newPixelValue = sourcePixel + x + ParalaxOffset.x;
            //+ World.tile_size.x;
            *targetPixel++ = *newPixelValue;
        }
    }
    // memcpy(Buffer.memory, BgCache.memory, Buffer.size);

    // static ground
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[12],
                             v2{0, 0},
                             v2{GridSize16x16.x, 0});
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[12],
                             v2{0, GridSize16x16.y - 1},
                             v2{GridSize16x16.x, GridSize16x16.y - 1});

    // moving ground
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[GroundIdx],
                             // FIXME: something here doesn't make any sense
                             // but ok
                             v2{0, GridSize16x16.y - 2},
                             v2{GridSize16x16.x + 1, GridSize16x16.y - 2},
                             {.offset = WorldOffset + v2{8, 0}});
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[((GroundIdx + 1) % 2) +
                                                         GroundOffset],
                             // FIXME:
                             // something
                             // here
                             // doesn't
                             // make any
                             // sense
                             // but ok
                             v2{0, 1},
                             v2{GridSize16x16.x + 1, 1},
                             {true, false, WorldOffset + v2{8, 0}});
}

void draw_foreground()
{
    // TODO: should use some world position index
    //-> i have this in update world & draw world, but it should use the
    // same
    // logic for detecting the index
    for (int i = 0; i < World.visibile_tiles; i++)
    {
        int tileIdx = i + World.start_index;
        if (tileIdx >= World.tile_count)
        {
            logf("WARN: Detected invalid tile idx: %i, tileCount was %i, "
                 "visible tiles %i",
                 tileIdx,
                 World.tile_count,
                 World.visibile_tiles);
            continue;
        }

        // TODO: grid space to pixel space conversion
        if (World.tiles[tileIdx].is_visible)
        {
            // player is 3 above the ground layer
            v2 tilePos = v2{i, GridSize16x16.y - 3};
            v2 pixelPos = tilePos * World.tile_size;
            rendering_draw_sprite(Buffer,
                                  *World.tiles[tileIdx].current_sprite,
                                  pixelPos,
                                  // 1/2 offset, because else we're seeing
                                  // the box in the middle of the jump
                                  {.offset = WorldOffset + v2{8, 0}});
        }
    }
}
