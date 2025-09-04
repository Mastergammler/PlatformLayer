#include "../internal.h"
#include "types.h"

void player_update(Player& player)
{
    // walking is differente because it's continouosly offset? Does it need to
    // be????

    PlayerState* oldState = &player.states[player.current_state];
    if (PlayerWon)
    {
        player.current_state = IDLE;
    }
    else
    {
        switch (player.current_state)
        {
            case JUMPING:
                if (oldState->counter->division_changed_this_frame)
                {
                    // JUMP EXIT
                    if (player.jump_elapsed >= player.jump_max)
                    {
                        player.current_state = WALKING;
                        // transition code
                        player.screen_position.y += WORLD_TILE_SIZE.y;
                        audio_start_playback(Audio.land);
                    }
                    else
                    {
                        player.jump_elapsed++;
                        oldState->sprite_idx = oldState->counter->current_division %
                                               oldState->sprites->tile_count;
                    }
                }
                break;
            case COLLIDING:
                if (player.collider.collision_exit_frame)
                {
                    player.current_state = WALKING;
                }
                break;
            case WALKING:
                if (player.collider.collision_enter_frame)
                {
                    player.current_state = COLLIDING;
                    audio_start_playback(Audio.hit);
                    Freeze = true;
                    MusicStarted = false;
                    audio_stop_playback(&Audio.song);
                }
                else if (GameInputs.Jump.pressed)
                {
                    player.current_state = JUMPING;
                    player.states[JUMPING].sprite_idx = player.states[JUMPING].counter->current_division %
                                                        player.states[JUMPING].sprites->tile_count;

                    player.jump_elapsed = 0;
                    player.screen_position.y -= WORLD_TILE_SIZE.y;
                    audio_start_playback(Audio.jump);
                }
                else
                {
                    if (oldState->counter->division_changed_this_frame)
                    {
                        // TEST: not 100% sure if that is right, it seems to
                        // start on idx 6 when logging
                        // -> but it looks right, and the first beat is also
                        // right!
                        oldState->sprite_idx = (oldState->counter->current_division +
                                                oldState->start_offset - 1) %
                                               oldState->sprites->tile_count;
                        if (GameInputs.Action.is_down)
                        {
                            logf("Next walking idx: %i", oldState->sprite_idx);
                        }
                    }
                }
                break;
            case IDLE:
                if (oldState->counter->division_changed_this_frame)
                {
                    oldState->sprite_idx = oldState->counter->current_division %
                                           oldState->sprites->tile_count;
                }
                break;
        }
    }

    PlayerState currentState = player.states[player.current_state];
    player.current_sprite = &currentState.sprites->tiles[currentState.sprite_idx];
}
