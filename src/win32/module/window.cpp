#include "../internal.h"

HDC WindowContext;

static function<void()> ExitProgram;
static HWND ActiveWindow;
static v2 WindowSize;
static bool AllowWindowResize;

void win32_set_window_title(string title)
{
    assert(ActiveWindow);

    SetWindowText(ActiveWindow, title.c_str());
}

void win32_handle_messages()
{
    assert(ActiveWindow);

    MSG message;
    while (PeekMessage(&message, ActiveWindow, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void win32_open_window(string name,
                       HINSTANCE instance,
                       WindowInfo& winin,
                       function<void()> onExit)
{
    WNDCLASS winClass = {};
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpszClassName = "dangine-proto";
    winClass.lpfnWndProc = WindowEvents;
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHegiht = GetSystemMetrics(SM_CYSCREEN);

    int centerX = (screenWidth - winin.display_size.width) / 2;
    int centerY = (screenHegiht - winin.display_size.height) / 2;

    RegisterClass(&winClass);

    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    RECT rect = {0, 0, winin.display_size.width, winin.display_size.height};
    AdjustWindowRect(&rect, dwStyle, false);

    // set window size before the window is created
    // else the WindowEvents callback might start with 0,0 on creation
    winin.window_size = v2{rect.right - rect.left, rect.bottom - rect.top};
    WindowSize = winin.window_size;
    AllowWindowResize = winin.allow_resize;
    ActiveWindow = CreateWindow(winClass.lpszClassName,
                                name.c_str(),
                                dwStyle,
                                centerX,
                                centerY,
                                winin.window_size.width,
                                winin.window_size.height,
                                0,
                                0,
                                instance,
                                0);
    ExitProgram = onExit;
    WindowContext = GetDC(ActiveWindow);
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
            // NOTE: i3 is still able to override this settings
            //  since it hooks into the linux platform api directly
            //  and can therefore circumvent the windows based constraints
            if (!AllowWindowResize)
            {
                MINMAXINFO* mmi = (MINMAXINFO*)lParam;
                mmi->ptMinTrackSize.x = WindowSize.x;
                mmi->ptMinTrackSize.y = WindowSize.y;
                mmi->ptMaxTrackSize.x = WindowSize.x;
                mmi->ptMaxTrackSize.y = WindowSize.y;
            }
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
        // SYS Keydown will trigger when using special keys like ALT
        // But we want to handle them together with the normal key events
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
            /*
             * lParam is a bit set,
             * 29: down state alt key this frame (0: up, 1: down)
             * 30: key down previous event ( 0: up, 1: down)
             * 31: transition state (0: DOWN, 1: UP )
             *     the transition state is the current press but inverted
             */
            bool wasDown = ((lParam & (1 << 30)) != 0);
            bool isDown = ((lParam & (1 << 31)) == 0);
            uint32_t vkCode = wParam;

            // for alt key implementation we need to do a bit more
            // because we might need to check isDown vs wasDown differently
            // also keycode handling might be different (VK_MENU)
            // bool altKeyWasDown = (lParam & (1 << 29)) != 0;

            if (isDown == wasDown) return 0;

            input_notify_key({wasDown, isDown, vkCode});
        }
        break;
        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}
