#include "../internal.h"
#include "types.h"

void player_jump_enter(Player& player);
void player_jump_exit(Player& player);
void player_jump_update(Player& player);

void player_idle_update(Player& player);

void player_colliding_update(Player& player);
void player_colliding_enter(Player& player);

void player_walking_update(Player& player);

void player_attack_enter(Player& player);
void player_attack_update(Player& player);

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
                                                       noop},
                                        [ATTACKING] = {player_attack_enter,
                                                       player_attack_update,
                                                       noop}};

void set_animation_index(PlayerState* state)
{
    // -1 because beats start at 1
    state->sprite_idx = (state->counter->current_subb - 1 +
                         state->start_offset) %
                        state->sprites->tile_count;
}

void player_transition_to(Player& player, PlayerStateId newState)
{
    PlayerStateId oldState = player.current_state;

    PLAYER_TRANSITIONS[oldState].exit(player);
    player.current_state = newState;
    PLAYER_TRANSITIONS[newState].enter(player);
}

void player_jump_enter(Player& player)
{
    player.states[JUMPING].sprite_idx = player.states[JUMPING].counter->current_subb %
                                        player.states[JUMPING].sprites->tile_count;

    player.states[JUMPING].elapsed_subbs = 0;
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
    if (player.collider.collision_enter_frame)
    {
        // we don't collide while in air
        // TODO: this is a bit strange, now we're bascially just battling the
        // collision system
        // -> But generally the collision system has to decide based on
        // something
        // => And this might be the player position, or state, so this is not
        // incorrectly set
        player.collider.collision_enter_frame = false;
        player.collider.collision_active = false;
    }

    PlayerState* state = &player.states[player.current_state];
    if (state->counter->subb_changed_this_frame)
    {
        state->elapsed_subbs++;
        if ((GameInputs.Jump.is_down &&
             state->elapsed_subbs <= player.jump_max) ||
            state->elapsed_subbs <= player.jump_min)
        {
            set_animation_index(state);
        }
        else
        {
            player_transition_to(player, WALKING);
        }
    }
}

void player_attack_enter(Player& player)
{
    player.states[ATTACKING].elapsed_subbs = 0;
    player.states[ATTACKING].sprite_idx = 0;
    audio_start_playback(Audio.sword);
}

void player_attack_update(Player& player)
{
    // TODO: collision handling should be maybe global? General????
    // -> handled in the collision system?
    if (player.collider.collision_enter_frame)
    {
        player_transition_to(player, COLLIDING);
        return;
    }

    PlayerState* state = &player.states[player.current_state];
    // animation cancel logic - restrict attack to minimum?
    if (GameInputs.Right.pressed && state->elapsed_subbs >= 2)
    {
        player_transition_to(player, ATTACKING);
    }
    else if (GameInputs.Jump.pressed)
    {
        player_transition_to(player, JUMPING);
    }
    else if (state->counter->subb_changed_this_frame)
    {
        if (++state->elapsed_subbs < 4)
        {
            state->sprite_idx = ++state->sprite_idx %
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
    else if (GameInputs.Right.pressed)
    {
        player_transition_to(player, ATTACKING);
    }
    else
    {
        PlayerState* state = &player.states[player.current_state];
        if (state->counter->subb_changed_this_frame)
        {
            // TEST: not 100% sure if that is right, it seems to
            // start on idx 6 when logging -> need better debug tools
            set_animation_index(state);
        }
    }
}

void player_idle_update(Player& player)
{
    PlayerState* state = &player.states[player.current_state];
    if (state->counter->subb_changed_this_frame)
    {
        set_animation_index(state);
    }
}

void player_colliding_enter(Player& player)
{
    audio_stop_all();
    audio_start_playback(Audio.hit);
    audio_start_playback(Audio.box);
    set_animation_index(&player.states[COLLIDING]);
    Game.current_state = PLAYER_LOST;
}

void player_colliding_update(Player& player)
{
    if (player.collider.collision_exit_frame)
    {
        player_transition_to(player, WALKING);
    }
}
