#pragma once

#include "../module.h"

#include "../../rendering/module.h"
#include "../beat/types.h"
#include "../collision/types.h"

struct Player;
typedef void (*PlayerStateEnter)(Player& p);
typedef void (*PlayerStateUpdate)(Player& p);
typedef void (*PlayerStateExit)(Player& p);

enum PlayerStateId
{
    IDLE,
    WALKING,
    JUMPING,
    COLLIDING,
    ATTACKING,
    PlayerStateIdCount
};

// vtable
struct StateBehaviour
{
    PlayerStateEnter enter;
    PlayerStateUpdate update;
    PlayerStateExit exit;
};

struct PlayerState
{
    PlayerStateId id;
    int sprite_idx;
    int start_offset;
    SpriteSheet* sprites;
    SubbeatCounter* counter;
    int elapsed_subbs;
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

    // TODO: do i need this? is this not to be done through subdivisions?
    // -> because it should be in sync as well
    // => Or maybe it's done through the camera later anyway?
    float movement_speed;

    PixelBuffer* current_sprite;
    Collider collider;
    int jump_max;
    int jump_min;
};
