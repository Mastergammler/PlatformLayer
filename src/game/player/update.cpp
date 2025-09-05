#include "../internal.h"
#include "types.h"

void player_update(Player& player)
{
    if (Game.current_state == PLAYER_WON)
    {
        player.current_state = IDLE;
    }
    else
    {
        PLAYER_TRANSITIONS[player.current_state].update(player);
    }

    PlayerState newState = player.states[player.current_state];
    player.current_sprite = &newState.sprites->tiles[newState.sprite_idx];
}
