#include "internal.h"

void player_init(Player& player,
                 BeatCounter counter,
                 SpriteSheet* idle,
                 SpriteSheet* walking,
                 v2 gridSize);

void player_update(Player& player);
void player_move(Player& player, bool forward);
