#include "internal.h"

#include "beat.h"
#include "player.h"
#include "world.h"

NamedValue find_config_value(ConfigStore configValues, string name)
{
    for (int i = 0; i < configValues.capacity; i++)
    {
        if (name == configValues.items[i].id)
        {
            return configValues.items[i];
        }
    }

    logf("Value '%s' was not present in the configuration file.", name.c_str());
    return {.empty = true};
}

template <typename T>
void override_default(T& defaultValue,
                      const string name,
                      ConfigStore configValues,
                      ValueType expectedType)
{
    NamedValue nv = find_config_value(configValues, name);
    if (nv.empty) return;

    // FIXME: c++17 extensions
    if (nv.type == expectedType)
    {
        if constexpr (is_same_v<T, bool>)
        {
            defaultValue = nv.value.b;
        }
        else if constexpr (is_same_v<T, int>)
        {
            defaultValue = nv.value.i;
        }
        else if constexpr (is_same_v<T, float>)
        {
            defaultValue = nv.value.f;
        }
        else if constexpr (is_same_v<T, string>)
        {
            defaultValue = *nv.value.str;
        }
    }
    else
    {
        logf("Expecte type %i for config value '%s' but found type %i",
             expectedType,
             name.c_str(),
             nv.type);
    }
}

void game_init()
{
    Clock timer = {};
    timer_start(timer);

    ConfigStore configValues = {.capacity = CONFIG_STORE_SIZE};
    configValues.items = new NamedValue[CONFIG_STORE_SIZE];
    memset((void*)configValues.items,
           0,
           sizeof(NamedValue) * CONFIG_STORE_SIZE);
    parse_config_file(GAME_CONFIG_FILE, configValues);

    override_default(Config.UseAsio, ID(Config.UseAsio), configValues, T_BOOL);
    override_default(Config.TargetFps,
                     ID(Config.TargetFps),
                     configValues,
                     T_INT);
    override_default(Config.AsioDriverName,
                     ID(Config.AsioDriverName),
                     configValues,
                     T_STRING);
    override_default(Config.AudioBufferSize,
                     ID(Config.AudioBufferSize),
                     configValues,
                     T_INT);
    override_default(Config.MasterVolume,
                     ID(Config.MasterVolume),
                     configValues,
                     T_FLOAT);

    logf("User asio driver name is '%s'", Config.AsioDriverName.c_str());
    logf("User fps: %i, user audio buffer: %i",
         Config.TargetFps,
         Config.AudioBufferSize);

    BgCache = {};
    BgCache.size = Buffer.size;
    BgCache.height = Buffer.height;
    BgCache.width = Buffer.width;
    BgCache.pixel_count = Buffer.pixel_count;
    BgCache.memory = new u32[Buffer.pixel_count];

    INPUT_LIST(SET_INPUT_NAME);

    GridSize16x16 = v2{Buffer.width / WORLD_TILE_SIZE.x,
                       Buffer.height / WORLD_TILE_SIZE.y};

    input_init_keyboard(&GameInputs, KEYMAPPING_FILE, WIN_KEYCODE_FILE);

    load_sheet(Sprites.PlayerWalking, "res/img/Walk.png", v2{32, 32});
    load_sheet(Sprites.PlayerIdle, "res/img/Idle.png", v2{32, 32});
    load_sheet(Sprites.PlayerHit, "res/img/Hit.png", v2{32, 32});
    load_sheet(Sprites.PlayerJump, "res/img/NinjaJump.png", v2{32, 32});
    load_sheet(Sprites.FontSprites, "res/img/Medodica_7x10.png", v2{7, 10});
    load_sheet(Sprites.GroundSprites,
               "res/img/GroundTiles_16x16.png",
               v2{16, 16});
    Sprites.Font = BitmapFont{-48, -55, -61, &Sprites.FontSprites};

    audio_load_sound(Audio.pcm_music, "res/audio/BeatNinjaJamBeat_16B_441.wav");
    audio_load_sound(Audio.pcm_hit, "res/audio/fx_hit.wav");
    audio_load_sound(Audio.pcm_jump, "res/audio/fx_jump.wav");
    audio_load_sound(Audio.pcm_land, "res/audio/fx_land.wav");
    Audio.song.loop = false;
    Audio.jump.volume = 1;
    Audio.land.volume = 1.4;
    Audio.song.volume = 0.7;

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
    vector<int> boxes;
    parse_number_file(boxes, LEVEL_FILE);
    int max = playerPos.x;
    if (!boxes.empty()) max += *max_element(boxes.begin(), boxes.end());
    // TODO: handle visible count better, this is strange
    //-> If i implement fluid movement, then i need a extra visible tile (or a
    // extra column rather) But this can then mess up the world movement &
    // index!
    world_init(World, max, 0, GridSize16x16.x);
    world_init_tile(BoxTile, SongClock, &Sprites.GroundSprites, 10, 2, 0.5);
    for (int i = 0; i < boxes.size(); i++)
    {
        // converting beat value to index
        // because starting beat is 1
        world_add_tile(World, BoxTile, boxes[i] - 1 + playerPos.x);
    }

    GroundDivision = beat_find_division(SongClock, 1);
    BgDivision = beat_find_division(SongClock, 0.5);
    BeatDivision = beat_find_division(SongClock, 1);
    MeasureDivision = beat_find_division(SongClock, 0.25);

    GroundIdx = GroundOffset;
    beat_start(SongClock);

    float elapsed = time_since_start(timer);
    logf("| %.1f ms | Game initialization", elapsed);
}

void game_dispose()
{
    log("Game exit");
}
