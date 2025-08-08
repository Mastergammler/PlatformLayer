#include "../internal.h"

static function<void()> ExitProgram;
static HWND ActiveWindow;

/**
 *  Requires open window to be called before !
 */
void win32_set_window_title(string title)
{
    SetWindowText(ActiveWindow, title.c_str());
}

/**
 *  Requires open window to be called before !
 */
void win32_handle_messages()
{
    // TODO: TO-ENGINE: this should be part of the engine layer?!
    // we need to reset the single frame states before handling new messages
    // else these states could be persisted multiple rounds
    // Input_ResetKeyStates();

    MSG message;
    while (PeekMessage(&message, ActiveWindow, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void win32_open_window(string name, HINSTANCE instance, function<void()> onExit)
{
    WNDCLASS winClass = {};
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpszClassName = "dangine-proto";
    winClass.lpfnWndProc = WindowEvents;
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    int winWidth = 800;
    int winHeight = 600;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHegiht = GetSystemMetrics(SM_CYSCREEN);

    int centerX = (screenWidth - winWidth) / 2;
    // TODO: - taskbar height etc
    int centerY = (screenHegiht - winHeight) / 2;

    RegisterClass(&winClass);
    ActiveWindow = CreateWindow(winClass.lpszClassName,
                                name.c_str(),
                                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                centerX,
                                centerY,
                                winWidth,
                                winHeight,
                                0,
                                0,
                                instance,
                                0);
    ExitProgram = onExit;
}

LRESULT CALLBACK WindowEvents(HWND hwnd,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam)
{
    LRESULT result = 0;
    switch (uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            ExitProgram();
        }
        break;
        case WM_GETMINMAXINFO:
        {
            // supposedly setting size constraints
            // requires WS_THICKFRAME
            // doesn't seem to work with wine
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            mmi->ptMinTrackSize.x = 400;
            mmi->ptMinTrackSize.y = 300;
            mmi->ptMaxTrackSize.x = 1024;
            mmi->ptMaxTrackSize.y = 768;
        }
        break;
        case WM_SETFOCUS:
        {
            HCURSOR hCursor = LoadCursor(NULL, IDC_IBEAM);
            SetCursor(hCursor);
        }
        break;
        case WM_SIZE:
        {
            // int newWidth = LOWORD(lParam);
            // int newHeight = HIWORD(lParam);
        }
        break;
        case WM_ACTIVATEAPP: OutputDebugStringA("WM_ACTIVEAPP\n"); break;
        // sys keydown is for things like Alt+F4
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        {
            // handle as default right now
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        break;
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
            // TODO: input handling
            // HandleKeyboardInput(lParam, wParam);
        }
        break;
        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}
