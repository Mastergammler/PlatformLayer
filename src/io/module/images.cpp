#include "../internal.h"

void load_sprite(SpriteBuffer& buffer, string path)
{
    buffer.file = path;
    // TODO: stbi does not allow to control the memory allocation of the image
    // this is a serious drawback and i'll probably not use it in the future
    // then
    buffer.pixels = stbi_load(path.c_str(),
                              &buffer.width,
                              &buffer.height,
                              &buffer.channels,
                              4);

    if (buffer.pixels != NULL) buffer.loaded = true;
}
