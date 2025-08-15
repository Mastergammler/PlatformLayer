#include "../internal.h"

void load_sprite(SpriteBuffer& buffer, string path)
{
    Clock timer = {};
    timer_start(timer);

    buffer.file = path;
    // TODO: stbi does not allow to control the memory allocation of the image
    // this is a serious drawback and i'll probably not use it in the future
    // then

    // NOTE: loads the pixels as RGBA
    buffer.pixels = stbi_load(path.c_str(),
                              &buffer.width,
                              &buffer.height,
                              &buffer.channels,
                              4);

    // NOTE: since Windows GDI expects pixels in BGRA we need to
    // swap the red and blue channel here
    // TODO: check how this is done on Linux / MacOs
    int pixelCount = buffer.width * buffer.height;
    for (int i = 0; i < pixelCount; i++)
    {
        u8* pixel = buffer.pixels + i * 4;
        u8 red = pixel[0];
        u8 blue = pixel[2];

        pixel[0] = blue;
        pixel[2] = red;
    }

    if (buffer.pixels != NULL)
    {
        buffer.loaded = true;
        float elapsed = time_since_start(timer);
        logf("Loading file '%s' (%ix%i) within %.3f ms",
             path.c_str(),
             buffer.width,
             buffer.height,
             elapsed);
    }
    else
    {
        logf("Error attempting to load file '%s' (%ix%i %ic)",
             path.c_str(),
             buffer.width,
             buffer.height,
             buffer.channels);
    }
}
