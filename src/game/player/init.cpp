#include "../beat.h"
#include "../internal.h"
#include "types.h"

void player_init(Player& player,
                 BeatCounter counter,
                 SpriteShelf& shelf,
                 v2 gridSize)
{

    player.states = new PlayerState[PlayerStateIdCount];

    player.states[IDLE] = {IDLE,
                           0,
                           0,
                           &shelf.PlayerIdle,
                           beat_find_division(counter, 2.)};
    player.states[WALKING] = {WALKING,
                              0,
                              5,
                              &shelf.PlayerWalking,
                              beat_find_division(counter, 8.)};
    player.states[JUMPING] = {JUMPING,
                              0,
                              0,
                              &shelf.PlayerJump,
                              beat_find_division(counter, 2.)};
    player.states[COLLIDING] = {COLLIDING,
                                0,
                                0,
                                &shelf.PlayerCollision,
                                beat_find_division(counter, 2.)};

    // we draw from upper left corner, so offset for x would be negative for
    v2 tileOffset = v2{-(WORLD_TILE_SIZE.x / 2), WORLD_TILE_SIZE.y};
    // FIXME: this handling is super strange, this only works, because the
    // sprite is bigger than the world size, else this would fail also
    //-> I shouldn't do it like this this is a bad way!
    player.center_point = player.states[WALKING].sprites->tile_size / 2 +
                          tileOffset;

    // TODO: hardcoded grid position
    // -2 for ground floor
    player.screen_position = f2{(float)gridSize.x / 2 * WORLD_TILE_SIZE.x,
                                (float)(gridSize.y - 2) * WORLD_TILE_SIZE.y};

    // it feels pretty bad if you can have a short jump
    // that lands you in a box, that sucks
    // -> So we do a minimum of box width here
    player.jump_max = 4;
    player.jump_min = 2;
    player.movement_speed = 100;
    player.collider = {};
    player.current_state = IDLE;
    player.current_sprite = &player.states[IDLE].sprites->tiles[0];
}

void player_reset(Player& player, v2 gridSize)
{
    Ninja.current_state = WALKING;
    player.screen_position = f2{(float)gridSize.x / 2 * WORLD_TILE_SIZE.x,
                                (float)(gridSize.y - 2) * WORLD_TILE_SIZE.y};

    player.collider = {};
}
