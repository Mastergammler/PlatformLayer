#pragma once

#include "../define.h"
#include "colors.h"
#include "types.h"

void rendering_init_buffer(DrawBuffer& buffer, int width, int height);
void rendering_clear_screen(DrawBuffer& buffer, u32 bgColor);

void rendering_draw_sprite(DrawBuffer& buffer,
                           SpriteBuffer& sprite,
                           v2 position);
