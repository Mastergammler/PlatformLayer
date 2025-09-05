#pragma once

#include "../audio/module.h"
#include "../engine.h"
#include "../input/keyboard.h"
#include "../io/module.h"
#include "../logging/module.h"
#include "../platform.h"
#include "../rendering/module.h"
#include "../timing/module.h"
#include "../util/module.h"
#include "imports.h"
#include "module.h"
#include "types.h"

#include "beat/types.h"
#include "collision/types.h"
#include "player/types.h"
#include "world/types.h"

#define KEYBOARD_INPUTS 12
#define NUDGE_STEPS 0.01
#define CONFIG_STORE_SIZE 8
#define WIN_KEYCODE_FILE "configs/windows.conf"
#define KEYMAPPING_FILE "configs/keyboard.conf"
#define GAME_CONFIG_FILE "configs/settings.conf"
// TODO: For later build
#define LEVEL_FILE "res/level/actual.lvl"
#define LEVEL_FILE_JAM "res/level/jam.lvl"

#define ID(x) trim_to_var_name(#x)
#define SET_INPUT_NAME(name) GameInputs.name.identifier = ID(GameInputs.name);

extern DrawBuffer Buffer;
extern Clock GameClock;
extern StateBehaviour PLAYER_TRANSITIONS[PlayerStateIdCount];

static BeatCounter SongClock;
static GameInputState GameInputs = {new KeyInput[KEYBOARD_INPUTS](),
                                    KEYBOARD_INPUTS};

// Global state
static DrawBuffer BgCache;
static Player Ninja;
// world stuff
static WorldGrid World;
static WorldTile BoxTile;
static v2 WorldOffset;
static v2 ParalaxOffset;

static SpriteShelf Sprites;
static AudioShelf Audio;
static GameConfig Config;
static GameState Game;

/*static bool Started = false;
static bool MusicStarted = false;
static bool Freeze = false;
static bool PlayerWon = false;*/

// timings
static DivisionCounter* GroundDivision;
static DivisionCounter* BgDivision;
static DivisionCounter* BeatDivision;
static DivisionCounter* MeasureDivision;
static DivisionCounter* PixelDivision;
static DivisionCounter* BgParalaxDivision;

// TODO: move to game state general
static int GroundIdx = 0;
static int GroundOffset = 8;
static v2 GridSize16x16 = {};
static bool BgChanged = true;

static const v2 WORLD_TILE_SIZE = v2{16, 16};
