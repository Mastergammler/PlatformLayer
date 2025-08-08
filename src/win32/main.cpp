#include "../engine.h"
#include "internal.h"

WindowParams Window;

int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    Window = {hInstance, hPrevInstance, lpCmdLine, nShowCmd};

    engine_start();

    return 0;
}
