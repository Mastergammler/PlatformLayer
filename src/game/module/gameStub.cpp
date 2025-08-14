#include "../internal.h"

const int KEYBOARD_INPUTS = 5;

static int helpcounter = 0;
static bool showBlue = true;
static GameInputState GameInputs = {new KeyInput[KEYBOARD_INPUTS],
                                    KEYBOARD_INPUTS};

// TODO: MOVETO parsing
string TrimToVariableName(string s)
{
    size_t pos = s.find_last_of('.');
    return pos == string::npos ? s : s.substr(pos + 1);
}

void game_init()
{
    GameInputs.Exit.identifier = TrimToVariableName(NAMEOF(GameInputs.Exit));
    GameInputs.Action.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.Action));
    GameInputs.Help.identifier = TrimToVariableName(NAMEOF(GameInputs.Help));
    GameInputs.Jump.identifier = TrimToVariableName(NAMEOF(GameInputs.Jump));
    GameInputs.ReloadConfig.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.ReloadConfig));
    input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);

    log("Game initalized");
}

void game_update()
{
    if (GameInputs.Exit.released) engine_stop();
    if (GameInputs.Help.pressed)
    {
        string msg = format("I have helped %i times already", ++helpcounter);
        log(msg);
        platform_window_title(msg);
    }
    else if (GameInputs.Action.pressed)
    {
        log("Player triggered action");
        platform_window_title("STUFF IS HAPPENING");
    }
    else if (GameInputs.Jump.pressed)
    {
        if (showBlue)
        {
            rendering_clear_screen(Buffer, BG_BLUE);
        }
        else
        {
            rendering_clear_screen(Buffer, BG_COLOR);
        }

        showBlue = !showBlue;
    }

    // hot reload functionality
    if (GameInputs.ReloadConfig.released)
    {
        input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);
        log("Keybindings reloaded!");
    }
}

void game_dispose()
{
    log("Game exit");
}
