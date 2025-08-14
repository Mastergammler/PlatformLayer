#include "../internal.h"

void platform_draw(DrawBuffer& buffer)
{
    // TODO: should probably not be created every single time?
    BITMAPINFO info = {};
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = buffer.width;
    // negative height = drawing top to bottom
    info.bmiHeader.biHeight = -buffer.height;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;

    StretchDIBits(WindowContext,
                  0,
                  0,
                  // these are screen dimensions actually
                  // TODO: input screen dimenions as well
                  buffer.width,
                  buffer.height,
                  0,
                  0,
                  buffer.width,
                  buffer.height,
                  buffer.memory,
                  &info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}
