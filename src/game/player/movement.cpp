#include "../internal.h"

void player_move(Player& player, bool forward)
{
    float distance = player.movement_speed * GameClock.sim_time;

    if (forward)
    {
        player.position.x += distance;
    }
    else
    {
        player.position.x -= distance;
    }

    player.is_walking = true;
    player.facing_forward = forward;
}
