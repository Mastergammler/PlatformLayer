#pragma once

#include "imports.h"
#include "module.h"
#include "types.h"

#include "../asio/module.h"
#include "../logging/module.h"
#include "../timing/module.h"

using std::atomic;
using std::memory_order_acq_rel;

extern ma_device_config DeviceConfig;
extern ma_device Device;
extern ma_context Context;
extern ma_audio_buffer* Buffers;
extern ma_resource_manager_config RmConfig;
extern ma_resource_manager Rm;
extern int BufferCount;
extern int BufferIndex;
extern int SampleRate;
// extern PlaybackPool Playbacks;

void data_callback(ma_device* device,
                   void* output,
                   const void* input,
                   ma_uint32 frameCount);
