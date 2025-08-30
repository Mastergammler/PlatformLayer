#include "../internal.h"

void draw_player()
{
    rendering_draw_sprite(Buffer,
                          *Ninja.current_sprite,
                          Ninja.screen_position - Ninja.center_point,
                          {Ninja.facing_forward});
}
