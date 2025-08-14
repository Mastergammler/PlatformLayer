#include "../internal.h"

void rendering_init_buffer(DrawBuffer& buffer, int width, int height)
{
    buffer.width = width;
    buffer.height = height;
    buffer.pixel_count = buffer.height * buffer.width;
    buffer.size = buffer.pixel_count * sizeof(u32);
    buffer.memory = platform_alloc(buffer.size);

    rendering_clear_screen(buffer, BG_BLUE);
}

void rendering_clear_screen(DrawBuffer& buffer, u32 bgColor)
{
    u32* pixel = (u32*)buffer.memory;
    for (int i = 0; i < buffer.pixel_count; i++)
    {
        *pixel++ = bgColor;
    }
}
