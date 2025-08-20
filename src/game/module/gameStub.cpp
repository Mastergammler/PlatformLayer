#include "../internal.h"

const int KEYBOARD_INPUTS = 9;

static int helpcounter = 0;
static bool showBlue = true;
// 0 initialize the values to not have leftovers
static GameInputState GameInputs = {new KeyInput[KEYBOARD_INPUTS](),
                                    KEYBOARD_INPUTS};

// images
static PixelBuffer Grass = {};
static PixelBuffer Plate = {};
static PixelBuffer Border = {};
static SpriteSheet SheetTest = {};
static SpriteSheet AnimTest = {};

// anim
static float Elapsed = 0;
static int ImgIndx = 0;

// player movement
static f2 PlayerPosition = {0, 0};
static v2 PlayerCenter = {};
static int PlayerAnimIdx = 0;
static float PlayerAnimElapsed = 0;
// frame time should be in relation to player speed
static float PlayerSpeed = 0.4;
static float FrameTime = 0.05f;
static bool FacingForward = true;

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
    load_sheet(SheetTest, "res/img/s64x64-test.png", v2{32, 32});
    // i want order 0-2-3-1
    PixelBuffer swap = SheetTest.tiles[1];
    SheetTest.tiles[1] = SheetTest.tiles[2];
    SheetTest.tiles[2] = SheetTest.tiles[3];
    SheetTest.tiles[3] = swap;

    load_sheet(AnimTest, "res/img/Anim.png", v2{32, 32});

    PlayerCenter = AnimTest.tile_size / 2;

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

    Elapsed += GameClock.sim_time;
    if (Elapsed > 1)
    {
        Elapsed -= 1;
        ImgIndx = ++ImgIndx % SheetTest.tile_count;
    }

    bool playAnim = false;
    bool leftToRight = true;

    // TODO: diagonal speed not normalized
    if (GameInputs.Up.is_down)
    {
        PlayerPosition.y -= PlayerSpeed;
        playAnim = true;
    }
    else if (GameInputs.Down.is_down)
    {
        PlayerPosition.y += PlayerSpeed;
        playAnim = true;
    }
    if (GameInputs.Left.is_down)
    {
        PlayerPosition.x -= PlayerSpeed;
        playAnim = true;
        FacingForward = false;
    }
    else if (GameInputs.Right.is_down)
    {
        PlayerPosition.x += PlayerSpeed;
        playAnim = true;
        FacingForward = true;
    }

    // playing exit frame until player start
    if (playAnim)
    {
        PlayerAnimElapsed += GameClock.sim_time;
        if (PlayerAnimElapsed > FrameTime)
        {
            PlayerAnimIdx = ++PlayerAnimIdx % AnimTest.tile_count;
            PlayerAnimElapsed -= FrameTime;
        }
    }
    else if (PlayerAnimIdx > 0)
    {
        // reset to start frame
        // no exit animation time, becaues it looks akward
        PlayerAnimIdx = 0;
    }

    if (PlayerPosition.x < -PlayerCenter.x) PlayerPosition.x = -PlayerCenter.x;
    if (PlayerPosition.y < -PlayerCenter.y) PlayerPosition.y = -PlayerCenter.y;
    if (PlayerPosition.x > Buffer.width - PlayerCenter.x)
        PlayerPosition.x = Buffer.width - 1 - PlayerCenter.x;
    if (PlayerPosition.y > Buffer.height - PlayerCenter.y)
        PlayerPosition.y = Buffer.height - 1 - PlayerCenter.y;

    rendering_clear_screen(Buffer, BG_BLUE);

    PixelBuffer& bgSprite = showBlue ? Grass : Plate;
    PixelBuffer& playerSprite = showBlue ? Plate : Grass;

    rendering_fill_screen(Buffer, SheetTest.tiles[ImgIndx]);
    rendering_draw_sprite(Buffer,
                          AnimTest.tiles[PlayerAnimIdx],
                          PlayerPosition,
                          FacingForward);

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
