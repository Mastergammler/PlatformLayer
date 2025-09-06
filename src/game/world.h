#include "internal.h"

void world_init(WorldGrid& grid, int count, int startIdx, int endIdx);
void world_update(WorldGrid& grid);
void world_add_tile(WorldGrid grid, WorldTile& tile, int xPos);
void world_init_tile(WorldTile& tile,
                     BeatCounter counter,
                     SpriteSheet* sheet,
                     int minIdx,
                     int maxIdx,
                     float subsubb);
void world_move(WorldGrid& grid);
