#include "../beat.h"
#include "../internal.h"
#include "types.h"

WorldGrid World;

void world_init(WorldGrid& grid, int count, int startIdx, int visible_count)
{
    grid.tile_count = count;
    grid.visibile_tiles = visible_count;
    // TODO: i got some off by one issue here sometimes? Dunno really
    grid.tiles = new WorldTile[grid.tile_count + 1];
    memset(grid.tiles, 0, (grid.tile_count + 1) * sizeof(WorldTile));
    grid.start_index = startIdx;
    grid.pixel_offset = v2{0, 0};
    grid.tile_size = WORLD_TILE_SIZE;
}

void world_add_tile(WorldGrid grid, WorldTile& tile, int xPos)
{
    // TODO: use v2 and calculate position
    grid.tiles[xPos] = tile;
    grid.tiles[xPos].is_visible = true;
    grid.tiles[xPos].word_position = v2{xPos, 0};
}

void world_init_tile(WorldTile& tile,
                     BeatCounter counter,
                     SpriteSheet* sheet,
                     int minIdx,
                     int count,
                     float subdivision)
{
    tile.sheet = sheet;
    tile.sheet_min_index = minIdx;
    tile.animation_frames = count;

    tile.counter = beat_find_division(counter, subdivision);
    tile.sprite_index = tile.sheet_min_index;
    tile.current_sprite = &sheet->tiles[tile.sprite_index];
}
