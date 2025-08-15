#include "../internal.h"

void platform_draw(DrawBuffer buffer, WindowInfo winin)
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
                  winin.display_size.width,
                  winin.display_size.height,
                  0,
                  0,
                  buffer.width,
                  buffer.height,
                  buffer.memory,
                  &info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}
