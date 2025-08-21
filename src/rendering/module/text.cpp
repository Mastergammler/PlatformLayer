#include "../internal.h"

// NOTE: supporting uniform sprite sheets only (all letters same width!)
void rendering_draw_text(DrawBuffer buffer,
                         BitmapFont font,
                         string text,
                         v2 position,
                         bool anchorLeft)
{
    v2 curPos = position;
    if (!anchorLeft)
    {
        curPos.x = curPos.x - text.length() * font.sprites->tile_size.x;
    }

    for (int i = 0; i < text.length(); i++)
    {
        char ascii = text[i];
        int offset = 0;
        bool emptyField = false;

        if (isupper(ascii))
        {
            offset = font.upper_offset;
        }
        else if (islower(ascii))
        {
            offset = font.lower_offset;
        }
        else if (isdigit(ascii))
        {
            offset = font.digit_offset;
        }
        else
        {
            emptyField = true;
        }

        if (!emptyField)
        {
            int glyphIdx = ascii + offset;
            if (glyphIdx >= font.sprites->tile_count) glyphIdx = 0;

            PixelBuffer fontBitmap = font.sprites->tiles[glyphIdx];
            rendering_draw_sprite(buffer, fontBitmap, curPos);
        }

        int letterSpace = font.sprites->tile_size.x;
        curPos.x += letterSpace;
    }
}
