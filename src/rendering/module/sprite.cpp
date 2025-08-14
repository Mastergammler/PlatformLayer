#include "../internal.h"

void rendering_draw_sprite(DrawBuffer& buffer, SpriteBuffer& sprite, v2 pos)
{
    if (pos.x >= buffer.width || pos.x < 0 - sprite.width ||
        pos.y >= buffer.height || pos.y < 0 - sprite.height)
        return;

    int xStart = pos.x < 0 ? 0 : pos.x;
    int xEnd = pos.x + sprite.width;
    int xBound = xEnd >= buffer.width ? buffer.width : xEnd;
    int yStart = pos.y < 0 ? 0 : pos.y;
    int yEnd = pos.y + sprite.height;
    int yBound = yEnd >= buffer.height ? buffer.height : yEnd;
    int xVisible = xBound - xStart;
    int yVisible = yBound - yStart;

    int startIdx = yStart * buffer.width + xStart;

    u32* bufferStart = (u32*)buffer.memory + startIdx;
    u32* bitmapStart = (u32*)sprite.pixels;

    u32* bufferPixel = bufferStart;
    u32* bitmapPixel = bitmapStart;

    for (int y = 0; y < yVisible; y++)
    {
        bufferPixel = bufferStart + y * buffer.width;
        bitmapPixel = bitmapStart + y * sprite.width;
        for (int x = 0; x < xVisible; x++)
        {
            *bufferPixel = *bitmapPixel;
            bufferPixel++;
            bitmapPixel++;
        }
    }
}
