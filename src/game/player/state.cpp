#include "../internal.h"
#include "types.h"

void player_jump_enter(Player& player);
void player_jump_exit(Player& player);
void player_jump_update(Player& player);

void player_idle_update(Player& player);

void player_colliding_update(Player& player);
void player_colliding_enter(Player& player);

void player_walking_update(Player& player);

void noop(Player& player)
{
}

StateBehaviour PLAYER_TRANSITIONS[PlayerStateIdCount] = {
                                        [IDLE] = {noop,
                                                  player_idle_update,
                                                  noop},
                                        [WALKING] = {noop,
                                                     player_walking_update,
                                                     noop},
                                        [JUMPING] = {player_jump_enter,
                                                     player_jump_update,
                                                     player_jump_exit},
                                        [COLLIDING] = {player_colliding_enter,
                                                       player_colliding_update,
                                                       noop}};

void player_transition_to(Player& player, PlayerStateId newState)
{
    PlayerStateId oldState = player.current_state;

    PLAYER_TRANSITIONS[oldState].exit(player);
    player.current_state = newState;
    PLAYER_TRANSITIONS[newState].enter(player);
}

void player_jump_enter(Player& player)
{
    player.states[JUMPING].sprite_idx = player.states[JUMPING].counter->current_division %
                                        player.states[JUMPING].sprites->tile_count;

    player.jump_divs_elapsed = 0;
    player.screen_position.y -= WORLD_TILE_SIZE.y;
    audio_start_playback(Audio.jump);
}

void player_jump_exit(Player& player)
{
    player.screen_position.y += WORLD_TILE_SIZE.y;
    audio_start_playback(Audio.land);
}

void player_jump_update(Player& player)
{
    PlayerState* state = &player.states[player.current_state];
    if (state->counter->division_changed_this_frame)
    {
        if ((GameInputs.Jump.is_down &&
             player.jump_divs_elapsed < player.jump_max) ||
            player.jump_divs_elapsed < player.jump_min)
        {
            player.jump_divs_elapsed++;
            state->sprite_idx = state->counter->current_division %
                                state->sprites->tile_count;
        }
        else
        {
            player_transition_to(player, WALKING);
        }
    }
}

void player_walking_update(Player& player)
{
    if (player.collider.collision_enter_frame)
    {
        player_transition_to(player, COLLIDING);
    }
    else if (GameInputs.Jump.pressed)
    {
        player_transition_to(player, JUMPING);
    }
    else
    {
        PlayerState* state = &player.states[player.current_state];
        if (state->counter->division_changed_this_frame)
        {
            // TEST: not 100% sure if that is right, it seems to
            // start on idx 6 when logging
            // -> but it looks right, and the first beat is also
            // right!
            state->sprite_idx = (state->counter->current_division +
                                 state->start_offset - 1) %
                                state->sprites->tile_count;
            if (GameInputs.Action.is_down)
            {
                logf("Next walking idx: %i", state->sprite_idx);
            }
        }
    }
}

void player_idle_update(Player& player)
{
    PlayerState* state = &player.states[player.current_state];
    if (state->counter->division_changed_this_frame)
    {
        state->sprite_idx = state->counter->current_division %
                            state->sprites->tile_count;
    }
}

void player_colliding_enter(Player& player)
{
    audio_start_playback(Audio.hit);
    audio_start_playback(Audio.box);
    Game.current_state = PLAYER_LOST;
    audio_stop_playback(&Audio.song);
}

void player_colliding_update(Player& player)
{
    if (player.collider.collision_exit_frame)
    {
        player_transition_to(player, WALKING);
    }
}
