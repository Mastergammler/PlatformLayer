#pragma once

#include "../module.h"

#include "../../rendering/module.h"
#include "../beat/types.h"

struct Player
{
    f2 position;
    v2 sprite_center;

    /** = right */
    bool facing_forward;
    bool is_walking;

    // TODO: do i need this? is this not to be done through subdivisions?
    // -> because it should be in sync as well
    // => Or maybe it's done through the camera later anyway?
    float movement_speed;

    PixelBuffer* current_sprite;
    SpriteSheet* idle_sprites;
    DivisionCounter* idle_counter;
    int idle_idx;
    SpriteSheet* walking_sprites;
    DivisionCounter* walking_counter;

    // TODO: do i need the index?
    int walking_idx;
};
