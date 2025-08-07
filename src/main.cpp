#include "imports.h"
#include "logging/module.h"
#include <windows.h>

// TODO: main should not be part of the source? Because this would be for the
// lib only?

int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nShowCmd)
{
    Logger_Init({});
    Log("Hello from win32");
    Logf("This is formatted %i %s", 15, "another");
    Logger_Dispose();

    return 0;
}
