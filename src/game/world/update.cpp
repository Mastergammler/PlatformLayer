#include "../internal.h"

#define NUM_FLOOR_TILES 2

void world_update(WorldGrid& grid)
{
    if (MusicStarted)
    {
        if (BeatDivision->division_changed_this_frame)
        {
            // TODO: this condition is kinda bad! This way of world movement
            // works poorly
            // -> just because visible tiles is always 1 greater than what is
            // displayed
            // => Which means the movement stops before the last tile is shown
            if (grid.start_index + grid.visibile_tiles < grid.tile_count)
            {
                grid.start_index++;
            }
            else
            {
                // end reached
                PlayerWon = true;
            }
        }
        if (GroundDivision->division_changed_this_frame)
        {
            GroundIdx = (++GroundIdx % NUM_FLOOR_TILES) + GroundOffset;
        }
        if (BgDivision->division_changed_this_frame)
        {
            BgChanged = true;
        }
        if (PixelDivision->division_changed_this_frame && !PlayerWon)
        {
            // we're starting at beat 1, but the offset should start at 0
            WorldOffset.x = -((PixelDivision->current_division - 1) %
                              World.tile_size.x);
        }
        if (BgParalaxDivision->division_changed_this_frame && !PlayerWon)
        {
            // TODO: dunno quite why but for some reason this has be positive to
            // seem to moving backwards?
            //-> Don't quite understand why yet
            ParalaxOffset.x = (BgParalaxDivision->current_division - 1) %
                              World.tile_size.x;
        }
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

        if (tile->counter->division_changed_this_frame)
        {
            tile->sprite_index = tile->counter->current_division % tile->animation_frames +
                                 tile->sheet_min_index;
            tile->current_sprite = &tile->sheet->tiles[tile->sprite_index];
        }
    }
}
