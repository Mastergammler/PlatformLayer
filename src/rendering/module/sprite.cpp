#include "../internal.h"

void rendering_fill_screen(DrawBuffer buffer, PixelBuffer sprite)
{
    int columns = buffer.width / sprite.size.width;
    int rows = buffer.height / sprite.size.height;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            rendering_draw_sprite(buffer,
                                  sprite,
                                  v2{x * sprite.size.width,
                                     y * sprite.size.height});
        }
    }
}

void debug_unsafe_draw(DrawBuffer buffer, PixelBuffer sprite, v2 pos)
{
    int startIdx = pos.x + pos.y * buffer.width;
    u32* bufferStart = (u32*)buffer.memory + startIdx;
    int spritePixelCount = sprite.size.x * sprite.size.y;

    u32* bufferPixel = bufferStart;
    u32* spritePixel = (u32*)sprite.pixels;
    for (int i = 0; i < spritePixelCount; i++)
    {
        if (i % sprite.size.x == 0 && i > 0)
        {
            // goto next row
            bufferPixel = bufferPixel + buffer.width - sprite.size.x;
        }

        *bufferPixel = *spritePixel;
        bufferPixel++;
        spritePixel++;
    }
}

// TODO: handle negative valuse for clipping correctly
void rendering_draw_sprite(DrawBuffer buffer,
                           PixelBuffer sprite,
                           v2 pos,
                           bool leftToRight)
{
    if (pos.x >= buffer.width || pos.x < 0 - sprite.size.width ||
        pos.y >= buffer.height || pos.y < 0 - sprite.size.height)
        return;

    // clip checking
    int xStart = pos.x < 0 ? 0 : pos.x;
    int xEnd = pos.x + sprite.size.width;
    int xBound = xEnd >= buffer.width ? buffer.width : xEnd;
    int yStart = pos.y < 0 ? 0 : pos.y;
    int yEnd = pos.y + sprite.size.height;
    int yBound = yEnd >= buffer.height ? buffer.height : yEnd;
    int xVisible = xBound - xStart;
    int yVisible = yBound - yStart;

    int startIdx = yStart * buffer.width + xStart;

    // NOTE:
    // because we loaded the pixels as individual bytes,
    // and we're on little endian the byte order is reveresd!
    // ARGB!
    u32* bufferStart = (u32*)buffer.memory + startIdx;
    u32* bitmapStart = (u32*)sprite.pixels;

    u32* bufferPixel = bufferStart;
    u32* bitmapPixel = bitmapStart;

    for (int y = 0; y < yVisible; y++)
    {
        bufferPixel = bufferStart + y * buffer.width;
        bitmapPixel = bitmapStart + y * sprite.size.width;

        if (!leftToRight) bitmapPixel = bitmapPixel + sprite.size.width - 1;

        for (int x = 0; x < xVisible; x++)
        {
            // NOTE: since Win GDI doesn't handle transparency
            // we need to implement it ourselfes here
            // we just ignore pixels who are fully transparent
            u32 bitmapValue = *bitmapPixel;
            u8 alpha = (bitmapValue >> 24) & 0xFF;
            if (alpha != 0)
            {
                *bufferPixel = *bitmapPixel;
            }

            bufferPixel++;
            if (leftToRight)
                bitmapPixel++;
            else
                bitmapPixel--;
        }
    }
}
