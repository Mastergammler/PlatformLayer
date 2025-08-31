#include "internal.h"

void player_init(Player& player,
                 BeatCounter counter,
                 SpriteShelf& shelf,
                 v2 gridSize);
void player_reset(Player& player, v2 gridSize);
void player_update(Player& player);
void player_move(Player& player, bool forward);
