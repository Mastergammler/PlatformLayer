#include "../internal.h"

#include "../beat.h"

#define NUDGE_STEPS 0.01;

const int KEYBOARD_INPUTS = 11;

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
static SpriteSheet PlayerWalking = {};
static SpriteSheet PlayerIdle = {};
static SpriteSheet FontSprites = {};
static SpriteSheet GroundSprites = {};
static BitmapFont Font = {};
static Audio audio;
static Audio fx;
static Audio laserSound;
static Playback songPb = {&audio};
static Playback fxpb = {&fx};
static Playback laser = {&laserSound};
static BeatClock SongClock;
static DrawBuffer BgCache;

// TODO: this way of syncing using a clock doesn't seem to be working
//  I probably need to send the frame position (cursor position) from
//  the audio directly and calculate the beat based on that
//  else i'll never have a proper sync
//  This would also compensate for sound glitches (lags) as i encountered
// TEST: input lag from keyboard with just then logging out the BEAT on which it
// was pressed & how close it was to the beat (ms diff etc) to see how bad it
// is, if it's playable etc -> because that's the other issue
// - Syncing player input & syncing to audio in general

// anim
static int GroundIdx = 0;
static int GroundOffset = 8;

// player movement
static f2 PlayerPosition = {0, 0};
static v2 PlayerCenter = {};
static v2 GridSize16x16 = {};
static int PlayerWalkAnimIdx = 0;
// Offbeat seems just strange
static int PlayerIdleAnimIdx = 1;
static float PlayerIdleElapsed = 0;
// frame time should be in relation to player speed
static float PlayerSpeed = 40;
static bool FacingForward = true;

static bool Started = false;
static bool MusicStarted = false;

// timings
static DivisionCounter* GroundDivision;
static DivisionCounter* BgDivision;
static DivisionCounter* IdleDivision;
static DivisionCounter* WalkingDivision;
static DivisionCounter* BeatDivision;
static DivisionCounter* MeasureDivision;

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

    BgCache = {};
    BgCache.size = Buffer.size;
    BgCache.height = Buffer.height;
    BgCache.width = Buffer.width;
    BgCache.pixel_count = Buffer.pixel_count;
    BgCache.memory = new u32[Buffer.pixel_count];

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
    GameInputs.NudgeLeft.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.NudgeLeft));
    GameInputs.NudgeRight.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.NudgeRight));

    GridSize16x16 = v2{Buffer.width / 16, Buffer.height / 16};

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

    load_sheet(PlayerWalking, "res/img/Anim.png", v2{32, 32});
    load_sheet(PlayerIdle, "res/img/Idle.png", v2{32, 32});
    load_sheet(FontSprites, "res/img/Medodica_7x10.png", v2{7, 10});
    load_sheet(GroundSprites, "res/img/tiles_16x16.png", v2{16, 16});
    Font = BitmapFont{-48, -55, -61, &FontSprites};

    PlayerCenter = PlayerWalking.tile_size / 2;
    // 2 for ground, 2 for player size
    PlayerPosition = f2{0, (float)(GridSize16x16.y - 2 - 2) * 16};

    audio_load_sound(audio, "res/audio/Test2_112BPM_16B.wav");
    audio_load_sound(fx, "res/audio/FxTest_16B.wav");
    audio_load_sound(laserSound, "res/audio/LaserFx_16B.wav");
    laser.volume = 2.5;
    songPb.loop = true;
    float bpm = 112;
    float divisions[] = {.5, 2, 8};
    beat_init(SongClock, bpm, 4, divisions, 3);

    GroundDivision = beat_find_division(SongClock, 1);
    BgDivision = beat_find_division(SongClock, 0.5);
    IdleDivision = beat_find_division(SongClock, 2.);
    WalkingDivision = beat_find_division(SongClock, 8.);
    BeatDivision = beat_find_division(SongClock, 1);
    MeasureDivision = beat_find_division(SongClock, 0.25);

    float elapsed = time_since_start(timer);
    logf("| %.1f ms | Game initialization", elapsed);
}

void game_update()
{
    if (MusicStarted) beat_update(SongClock);

    if (GameInputs.Exit.released) engine_stop();

    if (GameInputs.NudgeLeft.pressed)
    {
        SongClock.offset -= NUDGE_STEPS;
    }
    else if (GameInputs.NudgeRight.pressed)
    {
        SongClock.offset += NUDGE_STEPS;
    }

    if (GameInputs.Help.pressed)
    {
        helpcounter++;
        audio_start_playback(fxpb);
    }
    else if (GameInputs.Action.is_down)
    {
    }
    else if (GameInputs.Jump.pressed)
    {
        // audio_start_playback(fxpb);
        audio_start_playback(laser);
    }

    // start audio
    if (!Started)
    {
        audio_start_playback(songPb);
        Started = true;
    }

    // sync audio
    if (!MusicStarted && AudioEvent.load() == AUDIO_START)
    {
        logf("Music start signal received");
        GroundIdx = (++GroundIdx % 2) + GroundOffset;
        MusicStarted = true;
        beat_start(SongClock);
    }

    bool walkingAnim = false;
    if (GameInputs.Left.is_down)
    {
        PlayerPosition.x -= PlayerSpeed * GameClock.sim_time;
        walkingAnim = true;
        FacingForward = false;
    }
    else if (GameInputs.Right.is_down)
    {
        PlayerPosition.x += PlayerSpeed * GameClock.sim_time;
        walkingAnim = true;
        FacingForward = true;
    }

    bool walkingBeatChangeThisFrame = false;
    bool nextIdleFrame = false;
    bool groundChanged = false;
    bool bgChanged = false;
    if (MusicStarted)
    {
        if (GroundDivision->division_changed_this_frame)
        {
            GroundIdx = (++GroundIdx % 2) + GroundOffset;
            // GroundIdx = ++GroundIdx % SheetTest.tile_count;
            groundChanged = true;
        }
        if (BgDivision->division_changed_this_frame)
        {
            bgChanged = true;
        }

        if (IdleDivision->division_changed_this_frame)
        {
            nextIdleFrame = true;
        }

        if (WalkingDivision->division_changed_this_frame)
        {
            walkingBeatChangeThisFrame = true;
        }
    }

    if (!walkingAnim && nextIdleFrame)
    {
        PlayerIdleAnimIdx = ++PlayerIdleAnimIdx % PlayerIdle.tile_count;
    }

    // playing exit frame until player start
    if (walkingAnim && walkingBeatChangeThisFrame)
    {
        PlayerWalkAnimIdx = ++PlayerWalkAnimIdx % PlayerWalking.tile_count;
    }

    PixelBuffer playerTile;
    if (walkingAnim)
        playerTile = PlayerWalking.tiles[PlayerWalkAnimIdx];
    else
        playerTile = PlayerIdle.tiles[PlayerIdleAnimIdx];

    if (PlayerPosition.x < -PlayerCenter.x) PlayerPosition.x = -PlayerCenter.x;
    if (PlayerPosition.y < -PlayerCenter.y) PlayerPosition.y = -PlayerCenter.y;
    if (PlayerPosition.x > Buffer.width - PlayerCenter.x)
        PlayerPosition.x = Buffer.width - 1 - PlayerCenter.x;
    if (PlayerPosition.y > Buffer.height - PlayerCenter.y)
        PlayerPosition.y = Buffer.height - 1 - PlayerCenter.y;

    // TODO: cache the current one, and only update on frame changes etc
    if (bgChanged)
    {
        rendering_fill_screen_rng(BgCache, GroundSprites, 0, GroundOffset - 1);
    }
    memcpy(Buffer.memory, BgCache.memory, Buffer.size);

    rendering_fill_grid_area(Buffer,
                             GroundSprites.tiles[GroundIdx],
                             // FIXME: something here doesn't make any sense
                             // but ok
                             v2{0, GridSize16x16.y - 2},
                             v2{GridSize16x16.x, GridSize16x16.y - 2});
    rendering_fill_grid_area(Buffer,
                             GroundSprites.tiles[12],
                             v2{0, 0},
                             v2{GridSize16x16.x, 0});
    rendering_fill_grid_area(Buffer,
                             GroundSprites.tiles[12],
                             v2{0, GridSize16x16.y - 1},
                             v2{GridSize16x16.x, GridSize16x16.y - 1});
    rendering_fill_grid_area(Buffer,
                             GroundSprites.tiles[((GroundIdx + 1) % 2) +
                                                 GroundOffset],
                             // FIXME:
                             // something
                             // here
                             // doesn't
                             // make any
                             // sense
                             // but ok
                             v2{0, 1},
                             v2{GridSize16x16.x, 1},
                             {true, false});

    rendering_draw_sprite(Buffer, playerTile, PlayerPosition, {FacingForward});
    rendering_draw_text(Buffer,
                        Font,
                        format("Nudge "
                               "offset:"
                               " %.f "
                               "ms",
                               SongClock.offset * 1000),
                        v2{8, 8},
                        true);
    rendering_draw_text(Buffer,
                        Font,
                        format("Beat: %i %i",
                               MeasureDivision->current_division,
                               BeatDivision->current_division % SongClock.beats_per_measure +
                                                                       1),
                        v2{Buffer.width - 8, 8},
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
