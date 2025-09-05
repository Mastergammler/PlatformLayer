#include "../internal.h"

void world_move(WorldGrid& grid)
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
            Game.current_state = PLAYER_WON;
        }
    }

    // we don't want to offset this on the last change
    if (Game.current_state != PLAYER_WON)
    {
        if (PixelDivision->division_changed_this_frame)
        {
            // we're starting at beat 1, but the offset should start at 0
            WorldOffset.x = -((PixelDivision->current_division - 1) %
                              World.tile_size.x);
        }
        if (BgParalaxDivision->division_changed_this_frame)
        {
            // TODO: dunno quite why but for some reason this has be
            // positive to seem to moving backwards?
            //-> Don't quite understand why yet
            ParalaxOffset.x = (BgParalaxDivision->current_division - 1) %
                              World.tile_size.x;
        }
    }
}
