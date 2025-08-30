#pragma once

#include "../module.h"

#include "../../rendering/module.h"
#include "../beat/types.h"
#include "../collision/types.h"

enum PlayerState
{
    IDLE,
    WALKING,
    JUMPING,
    COLLIDING
};

struct Player
{
    f2 screen_position;
    /** relative to the sprite position */
    v2 center_point;

    /** = right */
    bool facing_forward;

    PlayerState state;

    // TODO: do i need this? is this not to be done through subdivisions?
    // -> because it should be in sync as well
    // => Or maybe it's done through the camera later anyway?
    float movement_speed;

    PixelBuffer* current_sprite;
    SpriteSheet* idle_sprites;
    DivisionCounter* idle_counter;
    SpriteSheet* hit_sprites;
    int idle_idx;
    SpriteSheet* walking_sprites;
    DivisionCounter* walking_counter;

    Collider collider;

    SpriteSheet* jump_sprites;
    DivisionCounter* jump_counter;

    // TODO: do i need the index?
    int walking_idx;

    int jump_idx;
    int jump_elapsed;
    int jump_max;
};
