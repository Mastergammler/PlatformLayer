#pragma once

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

extern DrawBuffer Buffer;

#define NAMEOF(x) #x

static const string WIN_KEYCODE_FILE = "config/windows.conf";
static const string KEYMAPPING_FILE = "config/keyboard.conf";
