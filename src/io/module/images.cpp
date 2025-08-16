#include "../internal.h"

void load_sheet(SpriteSheet& sheet, string imgPath, v2 tileSize, bool swapRB)
{
    PixelBuffer sourceImage = {};
    load_sprite(sourceImage, imgPath, swapRB);

    Clock timer = {};
    timer_start(timer);

    if (sourceImage.loaded)
    {
        sheet.tile_size = tileSize;
        sheet.channels = sourceImage.channels;
        sheet.source_image_size = sourceImage.size;
        // TODO: image cutoff / not fitting exactly
        sheet.grid_size = sourceImage.size / tileSize;
        sheet.tile_count = sheet.grid_size.x * sheet.grid_size.y;
        sheet.tiles = new PixelBuffer[sheet.tile_count];

        int tilePixelSize = tileSize.x * tileSize.y;

        // pixel swap the positions so the images align in memory correctly
        // and each image is continuous layed out
        u32* imgStart = (u32*)sourceImage.pixels;
        for (int i = 0; i < sheet.tile_count; i++)
        {
            int col = i / sheet.grid_size.x;
            int row = i % sheet.grid_size.x;

            // skip first row, because first row shourld be correct always
            u32* currentTilePixel = imgStart + (i * tilePixelSize) + tileSize.x;
            for (int p = 0; p < tilePixelSize - tileSize.x; p++)
            {
                // first swap all rows for the first tile r1-m1 => rx mx
                // then reverse for each one, but with one step less
                // one of those steps will be the identity (x=y position
                // unchanged)
                // => I'm getting strong matrix vibes here! It's kind of like
                // turning a matrix
                // => But not quite, or maybe it just works because i'm turning
                // an even matrix?
                // => Also not quite sure
            }
        }
    }

    float elapsed = time_since_start(timer);
    logf("Creating sprite sheet for file '%s' (%ix%i tiles a %ix%i) took %.3f "
         "ms",
         imgPath.c_str(),
         sheet.grid_size.x,
         sheet.grid_size.y,
         sheet.tile_size.x,
         sheet.tile_size.y,
         elapsed);
}

void load_sprite(PixelBuffer& buffer, string path, bool swapRB)
{
    Clock timer = {};
    timer_start(timer);

    buffer.file = path;
    // TODO: stbi does not allow to control the memory allocation of the image
    // this is a serious drawback and i'll probably not use it in the future
    // then

    // NOTE: loads the pixels as RGBA
    buffer.pixels = stbi_load(path.c_str(),
                              &buffer.size.width,
                              &buffer.size.height,
                              &buffer.channels,
                              4);

    if (swapRB)
    {
        int pixelCount = buffer.size.width * buffer.size.height;
        for (int i = 0; i < pixelCount; i++)
        {
            u8* pixel = buffer.pixels + i * 4;
            u8 red = pixel[0];
            u8 blue = pixel[2];

            pixel[0] = blue;
            pixel[2] = red;
        }
    }

    if (buffer.pixels != NULL)
    {
        buffer.loaded = true;
        float elapsed = time_since_start(timer);
        logf("Loading file '%s' (%ix%i) within %.3f ms",
             path.c_str(),
             buffer.size.width,
             buffer.size.height,
             elapsed);
    }
    else
    {
        logf("Error attempting to load file '%s' (%ix%i %ic)",
             path.c_str(),
             buffer.size.width,
             buffer.size.height,
             buffer.channels);
    }
}
