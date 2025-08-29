#include "../beat.h"
#include "../internal.h"

void player_init(Player& player,
                 BeatCounter counter,
                 SpriteSheet* idle,
                 SpriteSheet* walking,
                 v2 gridSize)
{
    player.idle_sprites = idle;
    player.walking_sprites = walking;

    player.sprite_center = player.walking_sprites->tile_size / 2;
    // TODO: hardcoded grid
    // -2 for ground floor, -2 for player size
    player.position = f2{(float)gridSize.x / 2 * 16,
                         (float)(gridSize.y - 2 - 2) * 16};

    player.walking_idx = 5;

    player.movement_speed = 100;
    player.idle_counter = beat_find_division(counter, 2.);
    player.walking_counter = beat_find_division(counter, 8.);
}
