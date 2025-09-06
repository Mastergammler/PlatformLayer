#include "../internal.h"

v2 screen_space_to_world_space(WorldGrid world, v2 position)
{
    // TODO: handle for more than 1 row
    // -> need to know height of the world
    int xoffset = World.start_index;
    int yoffset = 0;
    v2 playerWorldPosition = position + v2{xoffset, yoffset};
    return playerWorldPosition;
}

v2 pixel_space_to_grid_space(WorldGrid world, v2 position)
{
    return position / world.tile_size;
}

void collision_box_player(Player& player)
{
    v2 playerGridPosition = pixel_space_to_grid_space(
                                            World,
                                            player.screen_position + player.center_point);
    v2 playerWorldPosition = screen_space_to_world_space(World,
                                                         playerGridPosition);

    // TODO: actual collision handling based on height?
    if (World.tiles[playerWorldPosition.x].is_visible)
    {
        // ON COLLISION ENTER
        if (!player.collider.collision_active)
        {
            player.collider.collision_active = true;
            player.collider.collision_enter_frame = true;
        }
        else
        {
            player.collider.collision_enter_frame = false;
        }
    } // means WAS active till now
      // TODO: PROBLEM, 2 items back to back will fail ...
    else if (player.collider.collision_active)
    {
        player.collider.collision_exit_frame = true;
        player.collider.collision_active = false;
    }
    else if (player.collider.collision_exit_frame)
    {
        player.collider.collision_exit_frame = false;
    }
}
