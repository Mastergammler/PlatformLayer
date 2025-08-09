#include "imports.h"
#include "input.cpp"
#include "module.h"

// TODO: handle x-box-controller input

// NOTE: code from CaseyMuratori, not 100% sure whats going on here, or haw this
// works, just looks very complicated
#define X_INPUT_GET_STATE(name)                                                \
    DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
static x_input_get_state* XInputGetState_ = XInputGetStateStub;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
void HandleController()
{
    // NOTE: for controller stuff use xinput

    // TODO: should we poll this more frequently
    for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT;
         ++controllerIndex)
    {
        XINPUT_STATE controllerState;
        if (XInputGetState_(controllerIndex, &controllerState) == ERROR_SUCCESS)
        {
            // NOTE: controller is plugged in
            // TODO: See if controllerState.dwPacketNumber
            // increments too rapidly
            XINPUT_GAMEPAD* pad = &controllerState.Gamepad;

            bool up = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool down = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool left = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool right = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            bool start = (pad->wButtons & XINPUT_GAMEPAD_START);
            bool back = (pad->wButtons & XINPUT_GAMEPAD_BACK);
            bool lb = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
            bool rb = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
            bool aBtn = (pad->wButtons & XINPUT_GAMEPAD_A);
            bool bBtn = (pad->wButtons & XINPUT_GAMEPAD_B);
            bool xBtn = (pad->wButtons & XINPUT_GAMEPAD_X);
            bool yBtn = (pad->wButtons & XINPUT_GAMEPAD_Y);

            int16_t stickX = pad->sThumbLX;
            int16_t stickY = pad->sThumbLY;

            if (aBtn)
            {
                // TODO: action
            }
        }
        else
        {
            // NOTE: controller is not available
        }
    }
}
#pragma clang diagnostic pop
