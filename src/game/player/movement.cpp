#include "../internal.h"
#include "types.h"

void player_move(Player& player, bool forward)
{
    float distance = player.movement_speed * GameClock.sim_time;

    if (forward)
    {
        player.screen_position.x += distance;
    }
    else
    {
        player.screen_position.x -= distance;
    }

    player.state = WALKING;
    player.facing_forward = forward;
}
