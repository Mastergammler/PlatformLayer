#include "../engine.h"
#include "internal.h"

Win32Params Win32;

int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    Win32 = {hInstance, hPrevInstance, lpCmdLine, nShowCmd};

    engine_start();

    return 0;
}
