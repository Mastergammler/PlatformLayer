#pragma once

#include "imports.h"
#include "module.h"
#include "types.h"

#include "../logging/module.h"
#include "../timing/module.h"

using std::atomic;
using std::memory_order_acq_rel;

extern ma_device_config DeviceConfig;
extern ma_device Device;
extern ma_context Context;
extern ma_audio_buffer* Buffers;
extern int BufferCount;
extern int BufferIndex;
