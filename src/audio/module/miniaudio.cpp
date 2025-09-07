#include "../internal.h"

// 0 = choose default
#define SAMPLE_RATE 44100;
#define BUFFER_SIZE 0 // 128;

void init_via_miniaudio()
{
    Clock timer = {};
    timer_start(timer);
    Buffers = new ma_audio_buffer[BufferCount];

    ma_result result;
    result = ma_context_init(NULL, 0, NULL, &Context);
    if (result != MA_SUCCESS)
    {
        logf("[miniaudio] Unable to retrieve audio context %i", result);
        return;
    }

    ma_device_info* deviceInfos;
    ma_uint32 deviceCount;
    result = ma_context_get_devices(&Context,
                                    &deviceInfos,
                                    &deviceCount,
                                    NULL,
                                    NULL);
    if (result != MA_SUCCESS)
    {
        logf("[miniaudio] Unable to read hardware devices: %i", result);
        audio_dispose();
        return;
    }

    if (deviceCount == 0)
    {
        logf("[miniaudio] No output devices found.");
        audio_dispose();
        return;
    }

    int defaultDevice = 0;
    for (ma_uint32 i = 0; i < deviceCount; i++)
    {
        logf("[miniaudio] Device %u: %s", i, deviceInfos[i].name);
        if (deviceInfos[i].isDefault) defaultDevice = i;
    }

    DeviceConfig = ma_device_config_init(ma_device_type_playback);
    DeviceConfig.playback.pDeviceID = &deviceInfos[defaultDevice].id;
    // DeviceConfig.playback.pDeviceID = &deviceInfos[2].id;
    //  use default values
    DeviceConfig.playback.format = ma_format_unknown;

    // TODO: needs to be set before the device is initialized
    DeviceConfig.playback.format = ma_format_s16;
    DeviceConfig.playback.channels = 0;
    // TODO: set sample rate?!
    DeviceConfig.sampleRate = 0;
    DeviceConfig.dataCallback = data_callback;

    // For simple tasks even 32 is feasable
    // default seems to be 480 = 10ms for 48000 Hz
    // DeviceConfig.periodSizeInFrames = 32;
    DeviceConfig.periodSizeInFrames = BUFFER_SIZE;

    result = ma_device_init(&Context, &DeviceConfig, &Device);
    if (result != MA_SUCCESS)
    {
        logf("[miniaudio] Device '%s' could not be initalized: %i",
             deviceInfos[defaultDevice].name,
             result);
        audio_dispose();
        return;
    }

    Device.pUserData = &Playbacks;
    result = ma_device_start(&Device);
    if (result != MA_SUCCESS)
    {
        logf("[miniaudio] Device could not be started for playback: %i",
             result);
        audio_dispose();
        return;
    }

    SampleRate = Device.sampleRate;
    float initTime = time_since_start(timer);

    logf("[miniaudio] | %.1f ms | DefaultDevice '%s' with %u channels at "
         "%u hz "
         "and "
         "buffer size %i was initialized",
         initTime,
         deviceInfos[defaultDevice].name,
         Device.playback.channels,
         Device.sampleRate,
         DeviceConfig.periodSizeInFrames);
}
