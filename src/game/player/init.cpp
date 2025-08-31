#include "../beat.h"
#include "../internal.h"

void player_init(Player& player,
                 BeatCounter counter,
                 SpriteShelf& shelf,
                 v2 gridSize)
{
    player.idle_sprites = &shelf.PlayerIdle;
    player.walking_sprites = &shelf.PlayerWalking;
    player.hit_sprites = &shelf.PlayerHit;
    player.jump_sprites = &shelf.PlayerJump;

    // we draw from upper left corner, so offset for x would be negative for
    v2 tileOffset = v2{-(WORLD_TILE_SIZE.x / 2), WORLD_TILE_SIZE.y};
    // FIXME: this handling is super strange, this only works, because the
    // sprite is bigger than the world size, else this would fail also
    //-> I shouldn't do it like this this is a bad way!
    player.center_point = player.walking_sprites->tile_size / 2 + tileOffset;

    // TODO: hardcoded grid position
    // -2 for ground floor
    player.screen_position = f2{(float)gridSize.x / 2 * WORLD_TILE_SIZE.x,
                                (float)(gridSize.y - 2) * WORLD_TILE_SIZE.y};

    player.walking_idx = 5;
    player.jump_max = 4;

    player.movement_speed = 100;
    player.idle_counter = beat_find_division(counter, 2.);
    player.walking_counter = beat_find_division(counter, 8.);
    player.jump_counter = beat_find_division(counter, 2);

    player.collider = {};
    player.state = IDLE;
}

void player_reset(Player& player, v2 gridSize)
{
    Ninja.state = WALKING;
    player.screen_position = f2{(float)gridSize.x / 2 * WORLD_TILE_SIZE.x,
                                (float)(gridSize.y - 2) * WORLD_TILE_SIZE.y};

    player.walking_idx = 5;
    player.collider = {};
}
