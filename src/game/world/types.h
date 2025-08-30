#pragma once

#include "../../define.h"
#include "../../rendering/module.h"
#include "../beat/types.h"

struct WorldTile
{
    bool is_visible;
    /** world position in tiles */
    v2 word_position;

    SpriteSheet* sheet;
    int sheet_min_index;
    int animation_frames;

    // TODO: hmm maybe it should be it's own thing / animator?
    DivisionCounter* counter;
    PixelBuffer* current_sprite;
    int sprite_index;
};

// TODO: more than one row?
//  Here i would probably save them column by column, because it's moving
struct WorldGrid
{
    WorldTile* tiles;
    int tile_count;

    /* determines current view port */
    int start_index;
    int visibile_tiles;

    /**
     * "camera position kind of"
     * needs one extra tile then to draw etc
     * always one extra row
     * */
    v2 pixel_offset;

    /**
     * Size of the world tiles
     * The units that the world uses for them
     */
    v2 tile_size;
};
