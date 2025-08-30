#include "internal.h"

#include "beat.h"
#include "player.h"
#include "world.h"

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

    GridSize16x16 = v2{Buffer.width / WORLD_TILE_SIZE.x,
                       Buffer.height / WORLD_TILE_SIZE.y};

    input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);

    load_sprite(Sprites.Grass, "res/img/tile-grass.png");
    load_sprite(Sprites.Plate, "res/img/tile-plate.bmp");
    load_sprite(Sprites.Border, "res/img/tile-border.png");
    load_sheet(Sprites.SheetTest, "res/img/s64x64-test.png", v2{32, 32});
    // i want order 0-2-3-1
    PixelBuffer swap = Sprites.SheetTest.tiles[1];
    Sprites.SheetTest.tiles[1] = Sprites.SheetTest.tiles[2];
    Sprites.SheetTest.tiles[2] = Sprites.SheetTest.tiles[3];
    Sprites.SheetTest.tiles[3] = swap;

    load_sheet(Sprites.PlayerWalking, "res/img/Anim.png", v2{32, 32});
    load_sheet(Sprites.PlayerIdle, "res/img/Idle.png", v2{32, 32});
    load_sheet(Sprites.PlayerHit, "res/img/Hit.png", v2{32, 32});
    load_sheet(Sprites.FontSprites, "res/img/Medodica_7x10.png", v2{7, 10});
    load_sheet(Sprites.GroundSprites, "res/img/tiles_16x16.png", v2{16, 16});
    Sprites.Font = BitmapFont{-48, -55, -61, &Sprites.FontSprites};

    audio_load_sound(Audio.audio, "res/audio/Test2_112BPM_16B.wav");
    audio_load_sound(Audio.fx, "res/audio/FxTest_16B.wav");
    audio_load_sound(Audio.laserSound, "res/audio/LaserFx_16B.wav");
    Audio.laser.volume = 2.5;
    Audio.songPb.loop = true;

    float bpm = 112;
    float divisions[] = {.5, 2, 8};
    beat_init(SongClock, bpm, 4, divisions, 3);
    player_init(Ninja, SongClock, Sprites, GridSize16x16);
    Ninja.facing_forward = true;
    v2 playerPos = (Ninja.screen_position + Ninja.center_point) /
                   WORLD_TILE_SIZE;
    logf("Player start position: %i,%i (screen: %i,%i)",
         playerPos.x,
         playerPos.y,
         (int)Ninja.screen_position.x / WORLD_TILE_SIZE.x,
         (int)Ninja.screen_position.y / WORLD_TILE_SIZE.y);

    // world init testing
    world_init(World, 128, 0, GridSize16x16.x + 2);
    world_init_tile(BoxTile, SongClock, &Sprites.GroundSprites, 10, 2, 0.5);
    world_add_tile(World, BoxTile, 14);
    world_add_tile(World, BoxTile, 18);
    world_add_tile(World, BoxTile, 22);
    world_add_tile(World, BoxTile, 28);
    world_add_tile(World, BoxTile, 40);
    world_add_tile(World, BoxTile, 53);
    world_add_tile(World, BoxTile, 88);
    world_add_tile(World, BoxTile, 89);
    world_add_tile(World, BoxTile, 100);
    world_add_tile(World, BoxTile, 102);
    world_add_tile(World, BoxTile, 110);
    world_add_tile(World, BoxTile, 125);
    world_add_tile(World, BoxTile, 126);
    world_add_tile(World, BoxTile, 127);

    GroundDivision = beat_find_division(SongClock, 1);
    BgDivision = beat_find_division(SongClock, 0.5);
    BeatDivision = beat_find_division(SongClock, 1);
    MeasureDivision = beat_find_division(SongClock, 0.25);

    float elapsed = time_since_start(timer);
    logf("| %.1f ms | Game initialization", elapsed);
}

void game_dispose()
{
    log("Game exit");
}
