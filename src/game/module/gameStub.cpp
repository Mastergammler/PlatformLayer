#include "../internal.h"
#include <atomic>

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
static BitmapFont Font = {};
static Audio audio;
static Audio fx;
static Audio laserSound;
static Playback pb = {&audio};
static Playback fxpb = {&fx};
static Playback laser = {&laserSound};
static Clock SongClock = {};

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
static float Elapsed = 0;
static int GroundIdx = 0;
static float GroundFrameTime = 1;

// player movement
static f2 PlayerPosition = {0, 0};
static v2 PlayerCenter = {};
static int PlayerWalkAnimIdx = 0;
// Offbeat seems just strange
static int PlayerIdleAnimIdx = 1;
static float PlayerIdleElapsed = 0;
// frame time should be in relation to player speed
static float PlayerSpeed = 0.4;
static float PlayerWalkingBeatTime = 1;
static float PlayerIdleBeatTime = 1;
static bool FacingForward = true;

static bool Started = false;
static bool MusicStarted = false;

// ms in which the flip for the previous frame is accepted
// -> should be roughly synchronized with fps
static float PreBeatThreshold = 0.005;
// if the animation flip happend already
static bool PreBeatChange = false;
static bool IdlePreBeatChange = false;
static int BeatCounter = 0;
static float WalkingElapsed = 0;

static int FrameCounter = 0;

// TODO: MOVETO parsing
string TrimToVariableName(string s)
{
    size_t pos = s.find_last_of('.');
    return pos == string::npos ? s : s.substr(pos + 1);
}

float bpm_to_beat_duration_s(float bpm)
{
    float minute_to_s = 60;
    return minute_to_s / bpm;
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
    GameInputs.NudgeLeft.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.NudgeLeft));
    GameInputs.NudgeRight.identifier = TrimToVariableName(
                                            NAMEOF(GameInputs.NudgeRight));

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
    Font = BitmapFont{-48, -55, -61, &FontSprites};

    PlayerCenter = PlayerWalking.tile_size / 2;

    audio_load_sound(audio, "res/audio/Test2_112BPM_16B.wav");
    audio_load_sound(fx, "res/audio/FxTest_16B.wav");
    audio_load_sound(laserSound, "res/audio/LaserFx_16B.wav");
    laser.volume = 2.5;
    float bpm = 112;
    GroundFrameTime = bpm_to_beat_duration_s(bpm) * 2;
    PlayerWalkingBeatTime = bpm_to_beat_duration_s(bpm) / 8;
    PlayerIdleBeatTime = bpm_to_beat_duration_s(bpm) / 2;
    logf("Player idle beat time %.3f", PlayerIdleBeatTime);

    float elapsed = time_since_start(timer);
    logf("| %.1f ms | Game initialization", elapsed);
}

static int IdleBeatsPlayed = 0;
static float Offset_s = 0;
#define NUDGE_STEPS 0.01;

void game_update()
{
    if (GameInputs.Exit.released) engine_stop();

    bool walkingBeatChangeThisFrame = false;
    bool nextIdleFrame = false;
    int framesRead = FramesPassed.load(std::memory_order_acquire);
    FramesPassed.fetch_sub(framesRead, std::memory_order_acquire);
    FrameCounter += framesRead;
    float playbackDuration = (float)44100 / FrameCounter;
    int beatsPlayed = (playbackDuration - Offset_s) / PlayerIdleBeatTime;
    if (beatsPlayed > IdleBeatsPlayed)
    {
        IdleBeatsPlayed = beatsPlayed;
        nextIdleFrame = true;
    }

    if (GameInputs.NudgeLeft.pressed)
    {
        Offset_s -= NUDGE_STEPS;
    }
    else if (GameInputs.NudgeRight.pressed)
    {
        Offset_s += NUDGE_STEPS;
    }

    if (GameInputs.Help.pressed)
    {
        audio_update();
        helpcounter++;
        audio_start_playback(fxpb);
    }
    else if (GameInputs.Action.is_down)
    {
        logf("Audio did play %i frames this frame", framesRead);
    }
    else if (GameInputs.Jump.pressed)
    {
        // audio_start_playback(fxpb);
        audio_start_playback(laser);
    }

    // start audio
    if (!Started)
    {
        audio_start_playback(pb);
        Started = true;
    }

    // sync audio
    if (!MusicStarted && AudioEvent.load() == AUDIO_START)
    {
        GroundIdx = ++GroundIdx % SheetTest.tile_count;
        MusicStarted = true;
        timer_start(SongClock);
    }

    bool walkingAnim = false;

    // TODO: diagonal speed not normalized
    if (GameInputs.Up.is_down)
    {
        PlayerPosition.y -= PlayerSpeed;
        walkingAnim = true;
    }
    else if (GameInputs.Down.is_down)
    {
        PlayerPosition.y += PlayerSpeed;
        walkingAnim = true;
    }
    if (GameInputs.Left.is_down)
    {
        PlayerPosition.x -= PlayerSpeed;
        walkingAnim = true;
        FacingForward = false;
    }
    else if (GameInputs.Right.is_down)
    {
        PlayerPosition.x += PlayerSpeed;
        walkingAnim = true;
        FacingForward = true;
    }

    if (MusicStarted)
    {
        timer_update(SongClock);
        Elapsed += SongClock.sim_time;
        if (Elapsed > GroundFrameTime)
        {
            Elapsed -= GroundFrameTime;
            GroundIdx = ++GroundIdx % SheetTest.tile_count;
        }
    }

    if (MusicStarted)
    {
        WalkingElapsed += SongClock.sim_time;
        if (WalkingElapsed >= PlayerWalkingBeatTime)
        {
            WalkingElapsed -= PlayerWalkingBeatTime;
            BeatCounter++;
            walkingBeatChangeThisFrame = true;
        }

        PlayerIdleElapsed += SongClock.sim_time;
        if (PlayerIdleElapsed >= PlayerIdleBeatTime)
        {
            PlayerIdleElapsed -= PlayerIdleBeatTime;
            nextIdleFrame = true;
        }
    }

    if (!walkingAnim && nextIdleFrame)
    {
        PlayerIdleAnimIdx = ++PlayerIdleAnimIdx % PlayerIdle.tile_count;
    }

    // playing exit frame until player start
    if (walkingAnim)
    {
        // this beat vs last beat calculation etc
        float beatsPartial = time_since_start(SongClock) * 1000 /
                             PlayerWalkingBeatTime;

        float fullBeatsTime = (int)beatsPartial * PlayerWalkingBeatTime;
        float timeInCurBeat = beatsPartial - fullBeatsTime;
        // Pre beat condition
        if (timeInCurBeat > (PlayerWalkingBeatTime - PreBeatThreshold) &&
            !PreBeatChange)
        {
            PreBeatChange = true;
            PlayerWalkAnimIdx = ++PlayerWalkAnimIdx % PlayerWalking.tile_count;
        }
        else if (walkingBeatChangeThisFrame)
        {
            if (!PreBeatChange)
            {
                PlayerWalkAnimIdx = ++PlayerWalkAnimIdx %
                                    PlayerWalking.tile_count;
            }
            PreBeatChange = false;
        }
    }
    else
    {
        // TODO: this isn't really synced, this doesn't work yet
        //->  i guess it's too much of a mess anyway to do this properly
        // currently
        //=> I need to clean this up accordingly
        //  this beat vs last beat calculation etc
        /*float beatsPartial = time_since_start(SongClock) / 1000 /
                             PlayerIdleBeatTime;

        float fullBeatsTime = (int)beatsPartial * PlayerIdleBeatTime;
        float timeInCurBeat = beatsPartial - fullBeatsTime;
        // Pre beat condition
        if (timeInCurBeat > (PlayerIdleBeatTime - PreBeatThreshold) &&
            !IdlePreBeatChange)
        {
            IdlePreBeatChange = true;
            PlayerIdleAnimIdx = ++PlayerIdleAnimIdx % PlayerIdle.tile_count;
            logf("Anim for Beat %i animIdx %i groundIdx %i",
                 (int)beatsPartial + 1,
                 PlayerIdleAnimIdx,
                 GroundIdx);
    }
    else if (idleBeatChangeThisFrame)
    {
        if (!IdlePreBeatChange)
        {
            PlayerIdleAnimIdx = ++PlayerIdleAnimIdx % PlayerIdle.tile_count;
            logf("Beat %i animIdx %i groundIdx %i",
                 (int)beatsPartial,
                 PlayerIdleAnimIdx,
                 GroundIdx);
        }
        IdlePreBeatChange = false;
    }
    */
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

    rendering_clear_screen(Buffer, BG_BLUE);
    rendering_fill_screen(Buffer, SheetTest.tiles[GroundIdx]);
    rendering_draw_sprite(Buffer, playerTile, PlayerPosition, FacingForward);

    rendering_draw_text(Buffer,
                        Font,
                        format("Nudge offset: %.f ms", Offset_s * 1000),
                        v2{Buffer.width - 14, 14},
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
