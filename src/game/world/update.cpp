#include "../internal.h"
#include "../world.h"

#define NUM_FLOOR_TILES 2

void world_animate(WorldGrid& grid)
{
    if (GroundSubb->subb_changed_this_frame)
    {
        GroundIdx = (++GroundIdx % NUM_FLOOR_TILES) + GroundOffset;
    }
    if (BackgroundSubb->subb_changed_this_frame)
    {
        BgChanged = true;
    }

    for (int i = 0; i < grid.visibile_tiles; i++)
    {
        int idx = i + grid.start_index;
        if (idx >= World.tile_count)
        {
            logf("WARN: Detected invalid tile idx: %i, start idx %i, tileCount "
                 "was %i, "
                 "visible tiles %i",
                 idx,
                 grid.start_index,
                 World.tile_count,
                 World.visibile_tiles);
            continue;
        }

        // NOTE: if the tile count is not correct, we have a sporadic issue with
        // crashing, because it is uninitialized memory, and if it is is_visible
        // and one of the pointers is null, then it'll crash
        // either here, or either when trying to draw
        WorldTile* tile = &grid.tiles[idx];
        if (!tile->is_visible) continue;

        if (tile->counter->subb_changed_this_frame)
        {
            tile->sprite_index = tile->counter->current_subb % tile->animation_frames +
                                 tile->sheet_min_index;
            tile->current_sprite = &tile->sheet->tiles[tile->sprite_index];
        }
    }
}

void world_update(WorldGrid& grid)
{
    if (Game.current_state == LEVEL_STARTED)
    {
        world_move(grid);
    }
    world_animate(grid);
}
