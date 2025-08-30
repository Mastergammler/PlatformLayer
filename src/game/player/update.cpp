#include "../internal.h"
#include "types.h"

void player_update(Player& player)
{
    if (player.walking_counter->division_changed_this_frame)
    {
        player.walking_idx = ++player.walking_idx %
                             player.walking_sprites->tile_count;
    }
    if (player.idle_counter->division_changed_this_frame)
    {
        player.idle_idx = player.idle_counter->current_division %
                          player.idle_sprites->tile_count;
    }

    if (player.jump_counter->division_changed_this_frame &&
        player.state == JUMPING)
    {
        // JUMP EXIT
        if (player.jump_elapsed >= player.jump_max)
        {
            player.state = WALKING;
            player.screen_position.y += WORLD_TILE_SIZE.y;
        }
        else
        {
            player.jump_elapsed++;
            player.jump_idx = player.jump_counter->current_division %
                              player.jump_sprites->tile_count;
        }
    }

    // JUMP ENTER
    if (GameInputs.Jump.pressed && player.state != JUMPING)
    {
        player.state = JUMPING;
        player.jump_idx = 0;
        player.jump_elapsed = 0;
        player.screen_position.y -= WORLD_TILE_SIZE.y;
        audio_start_playback(Audio.pb_jump);
    }
    // TODO: i need to move the player instead, else this is akward
    else if (player.collider.collision_enter_frame && player.state != JUMPING)
    {
        player.state = COLLIDING;
        audio_start_playback(Audio.fxpb);
    }
    else if (player.collider.collision_exit_frame)
    {
        player.state = WALKING;
    }

    switch (player.state)
    {
        case IDLE:
            player.current_sprite = &player.idle_sprites->tiles[player.idle_idx];
            break;
        case WALKING:
            player.current_sprite = &player.walking_sprites->tiles[player.walking_idx];
            break;
        case JUMPING:
            player.current_sprite = &player.jump_sprites->tiles[player.jump_idx];
            break;
        case COLLIDING:
            // TODO: kinda bad solution should change this
            //-> no guarantee that same sprite length!
            player.current_sprite = &player.hit_sprites->tiles[player.idle_idx];
            break;
    }
}
