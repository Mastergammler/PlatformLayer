#include "../internal.h"

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

    player.current_sprite = player.is_walking ? &player.walking_sprites->tiles[player.walking_idx]
                                              : &player.idle_sprites->tiles[player.idle_idx];
}
