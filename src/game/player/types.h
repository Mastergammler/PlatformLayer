#pragma once

#include "../module.h"

#include "../../rendering/module.h"
#include "../beat/types.h"
#include "../collision/types.h"

enum PlayerStateId
{
    IDLE,
    WALKING,
    JUMPING,
    COLLIDING
};

struct PlayerState
{
    PlayerStateId id;

    int sprite_idx;
    int start_offset;
    SpriteSheet* sprites;
    DivisionCounter* counter;
};

struct Player
{
    f2 screen_position;
    /** relative to the sprite position */
    v2 center_point;

    /** = right */
    bool facing_forward;

    PlayerStateId current_state;
    PlayerState* states;
    int state_count;

    // TODO: do i need this? is this not to be done through subdivisions?
    // -> because it should be in sync as well
    // => Or maybe it's done through the camera later anyway?
    float movement_speed;

    /*SpriteSheet* idle_sprites;
    DivisionCounter* idle_counter;
    SpriteSheet* hit_sprites;
    SpriteSheet* walking_sprites;
    DivisionCounter* walking_counter;
    SpriteSheet* jump_sprites;
    DivisionCounter* jump_counter;

    // TODO: do i need the index?
    int walking_idx;
    int jump_idx;
    int idle_idx;*/

    PixelBuffer* current_sprite;
    Collider collider;
    int jump_elapsed;
    int jump_max;
};
