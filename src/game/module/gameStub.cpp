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
static SpriteSheet FontSprites = {};
static BitmapFont Font = {};

// anim
static float Elapsed = 0;
static int ImgIndx = 0;
static float GroundFrameTime = 1;

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

float bpm_to_beat_duration(float bpm)
{
    float minute_to_ms = 60;
    return minute_to_ms / bpm;
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
    load_sheet(FontSprites, "res/img/Medodica_7x10.png", v2{7, 10});
    Font = BitmapFont{-48, -55, -61, &FontSprites};

    PlayerCenter = AnimTest.tile_size / 2;

    Audio audio;
    audio_load_sound(audio, "res/audio/TestBeat_100Bpm_16M.wav");
    GroundFrameTime = bpm_to_beat_duration(100.);

    float elapsed = time_since_start(timer);
    logf("| %.1f ms | Game initialization", elapsed);
}

void game_update()
{
    if (GameInputs.Exit.released) engine_stop();
    if (GameInputs.Help.pressed)
    {
        helpcounter++;
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
    if (Elapsed > GroundFrameTime)
    {
        Elapsed -= GroundFrameTime;
        ImgIndx = ++ImgIndx % SheetTest.tile_count;
    }

    bool playAnim = false;

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
    rendering_fill_screen(Buffer, SheetTest.tiles[ImgIndx]);
    rendering_draw_sprite(Buffer,
                          AnimTest.tiles[PlayerAnimIdx],
                          PlayerPosition,
                          FacingForward);

    rendering_draw_text(Buffer,
                        Font,
                        format("Player has HELPED: %i", helpcounter),
                        v2{Buffer.width, 10},
                        false);

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
