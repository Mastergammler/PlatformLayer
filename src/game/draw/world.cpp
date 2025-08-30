#include "../internal.h"

void draw_world()
{
    bool bgChanged = false;
    if (MusicStarted)
    {
        // TODO: handle this properly with the world
        if (GroundDivision->division_changed_this_frame)
        {
            GroundIdx = (++GroundIdx % 2) + GroundOffset;
            // GroundIdx = ++GroundIdx % Sprites.SheetTest.tile_count;
        }
        if (BgDivision->division_changed_this_frame)
        {
            bgChanged = true;
        }
    }

    // TODO: cache the current one, and only update on frame changes etc
    if (bgChanged)
    {
        rendering_fill_screen_rng(BgCache,
                                  Sprites.GroundSprites,
                                  0,
                                  GroundOffset - 1);
    }
    memcpy(Buffer.memory, BgCache.memory, Buffer.size);

    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[GroundIdx],
                             // FIXME: something here doesn't make any sense
                             // but ok
                             v2{0, GridSize16x16.y - 2},
                             v2{GridSize16x16.x, GridSize16x16.y - 2});
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[12],
                             v2{0, 0},
                             v2{GridSize16x16.x, 0});
    rendering_fill_grid_area(Buffer,
                             Sprites.GroundSprites.tiles[12],
                             v2{0, GridSize16x16.y - 1},
                             v2{GridSize16x16.x, GridSize16x16.y - 1});
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

    for (int i = 0; i < World.visibile_tiles; i++)
    {
        int tileIdx = i + World.start_index;

        // TODO: grid space to pixel space conversion
        if (World.tiles[tileIdx].is_visible)
        {
            rendering_draw_sprite(Buffer,
                                  *World.tiles[tileIdx].current_sprite,
                                  v2{i * 16, (GridSize16x16.y - 3) * 16});
        }
    }
}
