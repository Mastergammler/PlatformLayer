#pragma once

// TODO: If there is only a internal file, how would the engine
//  tell the platform layer what kind of window to open?
//  How to input the window options???

#include "../input/keyboard.h"
#include "../logging/module.h"
#include "../rendering/module.h"
#include "imports.h"

struct Win32Params
{
    HINSTANCE hInstance;
    HINSTANCE prevInstance;
    LPSTR lpCmdLine;
    int nShowCmd;
};

extern Win32Params Win32;
extern HDC WindowContext;

/**
 * Reads the cmd arguments that where put forth to the window
 */
void win32_read_cmd_args(std::vector<std::string>& argStore);

void win32_open_window(std::string name,
                       HINSTANCE instance,
                       WindowInfo& winin,
                       std::function<void()> onExit);

void win32_handle_messages();
void win32_set_window_title(std::string title);

LRESULT CALLBACK WindowEvents(HWND hwnd,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam);
