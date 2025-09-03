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
    memcpy(Buffer.memory, BgCache.memory, Buffer.size);

    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[12],
                             v2{0, 0},
                             v2{GridSize16x16.x, 0});
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[12],
                             v2{0, GridSize16x16.y - 1},
                             v2{GridSize16x16.x, GridSize16x16.y - 1});
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[GroundIdx],
                             // FIXME: something here doesn't make any sense
                             // but ok
                             v2{0, GridSize16x16.y - 2},
                             v2{GridSize16x16.x, GridSize16x16.y - 2});
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
                             v2{GridSize16x16.x, 1},
                             {true, false});
}

void draw_foreground()
{
    // TODO: should use some world position index
    //-> i have this in update world & draw world, but it should use the same
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
            rendering_draw_sprite(Buffer,
                                  *World.tiles[tileIdx].current_sprite,
                                  v2{i * 16, (GridSize16x16.y - 3) * 16});
        }
    }
}
