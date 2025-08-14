#include "../../engine.h"
#include "../internal.h"

void platform_init()
{
    vector<string> cmdArgs;
    win32_read_cmd_args(cmdArgs);

    for (int i = 0; i < cmdArgs.size(); i++)
        logf("[ARG] %s", cmdArgs[i].c_str());
}

void platform_dispose()
{
}

void platform_open_window()
{
    win32_open_window("Loading ...", Window.hInstance, engine_stop);
}
void platform_window_title(string title)
{
    win32_set_window_title(title);
}

// TODO: needs to return key states?
// -> does this differ from controller input?
void platform_handle_messages()
{
    win32_handle_messages();
}

void* platform_alloc(size_t size)
{
    return VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}
void platform_free(void* mem, size_t size)
{
    VirtualFree(mem, size, MEM_RELEASE);
}
