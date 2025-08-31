#pragma once

#include "../audio/module.h"
#include "../engine.h"
#include "../input/keyboard.h"
#include "../io/module.h"
#include "../logging/module.h"
#include "../platform.h"
#include "../rendering/module.h"
#include "../timing/module.h"
#include "imports.h"
#include "module.h"
#include "types.h"

#include "beat/types.h"
#include "collision/types.h"
#include "player/types.h"
#include "world/types.h"

#define NAMEOF(x) #x
#define KEYBOARD_INPUTS 12
#define NUDGE_STEPS 0.01
#define WIN_KEYCODE_FILE "config/windows.conf"
#define KEYMAPPING_FILE "config/keyboard.conf"
#define LEVEL_FILE "res/level/jam.lvl"

// TODO: move to a parsing module
inline static string TrimToVariableName(string s)
{
    size_t pos = s.find_last_of('.');
    return pos == string::npos ? s : s.substr(pos + 1);
}

extern DrawBuffer Buffer;
extern Clock GameClock;
extern BeatCounter SongClock;

static GameInputState GameInputs = {new KeyInput[KEYBOARD_INPUTS](),
                                    KEYBOARD_INPUTS};

// Global state
static DrawBuffer BgCache;
// static BeatCounter SongClock;
static Player Ninja;
// world stuff
extern WorldGrid World;
static WorldTile BoxTile;

static SpriteShelf Sprites;
static AudioShelf Audio;

// timings
static DivisionCounter* GroundDivision;
static DivisionCounter* BgDivision;
static DivisionCounter* BeatDivision;
static DivisionCounter* MeasureDivision;

// TODO: move to game state general
static int GroundIdx = 0;
static int GroundOffset = 8;
static v2 GridSize16x16 = {};
static bool Started = false;
static bool MusicStarted = false;
static bool BgChanged = true;

static const v2 WORLD_TILE_SIZE = v2{16, 16};
