#include "../internal.h"

void world_update(WorldGrid& grid)
{
    if (MusicStarted)
    {
        if (BeatDivision->division_changed_this_frame)
        {
            // FIXME: it seems like here can be a off by one issue at the end
            //-> not 100% sure how this happens, but it looks like this, but
            // only happens sometimes
            //=> Maybe because of the beat change?
            if (grid.start_index + grid.visibile_tiles - 1 <
                grid.tile_count + 1)
            {
                grid.start_index++;
            }
            else
            {
                // end reached
                PlayerWon = true;
            }
        }
    }

    for (int i = 0; i < grid.visibile_tiles; i++)
    {
        int idx = i + grid.start_index;
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
