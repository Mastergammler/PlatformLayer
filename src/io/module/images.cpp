#include "../internal.h"

// TODO: the datastructure is still kind of messy, between where the info for
//  channels / size / files etc should live
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

        for (int i = 0; i < sheet.tile_count; i++)
        {
            PixelBuffer* cur = &sheet.tiles[i];

            cur->channels = sourceImage.channels;
            cur->size = tileSize;
            cur->file = sourceImage.file;
            cur->loaded = sourceImage.loaded;

            int tilePixels = tileSize.x * tileSize.y;

            // NOTE: We need to multiply by the channels since there is one byte
            //  per channel for each pixel, and we want the pixel offset
            cur->pixels = sourceImage.pixels +
                          (i * tilePixels * sourceImage.channels);
        }

        for (int strip = 0; strip < sheet.grid_size.y; strip++)
        {
            PixelBuffer firstStripTile = sheet.tiles[strip * sheet.grid_size.x];
            u32* stripStart = (u32*)firstStripTile.pixels;
            int tileRowLength = firstStripTile.size.x;
            int tileRowCount = firstStripTile.size.y;
            int stripTiles = sheet.grid_size.x;

            // 1 colum per tile, but 1 row per pixel
            Matrix<u32*> matrix = {};
            matrix.columns = stripTiles;
            matrix.rows = tileRowCount;
            matrix.cell_count = matrix.columns * matrix.rows;
            matrix.data = new u32*[matrix.cell_count];

            for (int cellIdx = 0; cellIdx < matrix.cell_count; cellIdx++)
            {
                u32* cellStart = stripStart + (cellIdx * tileRowLength);
                matrix.data[cellIdx] = cellStart;
            }

            SwapFunction<u32*> swapRow = [](u32*& source,
                                            u32*& target,
                                            int length) {
                for (int i = 0; i < length; i++)
                {
                    u32 swapStore = target[i];
                    target[i] = source[i];
                    source[i] = swapStore;
                }
            };

            matrix_row_swap(matrix, swapRow, tileRowLength);
        }
    }

    float elapsed = time_since_start(timer);
    logf("| %.3f ms | Creating sprite sheet for file '%s' (%ix%i tiles a "
         "%ix%i)",
         elapsed,
         imgPath.c_str(),
         sheet.grid_size.x,
         sheet.grid_size.y,
         sheet.tile_size.x,
         sheet.tile_size.y);
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
        logf("| %.3f ms | Loading file '%s' (%ix%i)",
             elapsed,
             path.c_str(),
             buffer.size.width,
             buffer.size.height);
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
