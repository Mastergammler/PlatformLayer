#include "../internal.h"

const int KEYBOARD_INPUTS = 9;

static int helpcounter = 0;
static bool showBlue = true;
static GameInputState GameInputs = {new KeyInput[KEYBOARD_INPUTS],
                                    KEYBOARD_INPUTS};

static PixelBuffer Grass = {};
static PixelBuffer Plate = {};
static PixelBuffer Border = {};
static PixelBuffer TwoX = {};
static v2 DrawPosition = {0, 0};

// TODO: MOVETO parsing
string TrimToVariableName(string s)
{
    size_t pos = s.find_last_of('.');
    return pos == string::npos ? s : s.substr(pos + 1);
}

void game_init()
{
    Clock timer = {};
    timer_start(timer);

    GameInputs.Exit.identifier = TrimToVariableName(NAMEOF(GameInputs.Exit));
    GameInputs.Action.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.Action));
    GameInputs.Help.identifier = TrimToVariableName(NAMEOF(GameInputs.Help));
    GameInputs.Jump.identifier = TrimToVariableName(NAMEOF(GameInputs.Jump));
    GameInputs.ReloadConfig.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.ReloadConfig));

    GameInputs.Up.identifier = TrimToVariableName(NAMEOF(GameInputs.Up));
    GameInputs.Down.identifier = TrimToVariableName(NAMEOF(GameInputs.Down));
    GameInputs.Left.identifier = TrimToVariableName(NAMEOF(GameInputs.Left));
    GameInputs.Right.identifier = TrimToVariableName(NAMEOF(GameInputs.Right));

    input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);

    load_sprite(Grass, "res/img/tile-grass.png");
    load_sprite(Plate, "res/img/tile-plate.bmp");
    load_sprite(Border, "res/img/tile-border.png");
    load_sprite(TwoX, "res/img/s64x64-test.png");

    float elapsed = time_since_start(timer);
    logf("Game initalized within %.1f ms", elapsed);
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
        showBlue = !showBlue;
    }

    if (GameInputs.Up.pressed) DrawPosition.y -= Grass.size.height;
    if (GameInputs.Down.pressed) DrawPosition.y += Grass.size.height;
    if (GameInputs.Left.pressed) DrawPosition.x -= Grass.size.width;
    if (GameInputs.Right.pressed) DrawPosition.x += Grass.size.width;

    rendering_clear_screen(Buffer, BG_BLUE);

    PixelBuffer& bgSprite = showBlue ? Grass : Plate;
    PixelBuffer& playerSprite = showBlue ? Plate : Grass;

    rendering_fill_screen(Buffer, Border);
    rendering_draw_sprite(Buffer, playerSprite, DrawPosition);

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
