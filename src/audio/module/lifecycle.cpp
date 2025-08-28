#include "../internal.h"
#include <winbase.h>

ma_context Context;
ma_device_config DeviceConfig;
ma_device Device;

// 0 = choose default
#define SAMPLE_RATE 44100;
#define BUFFER_SIZE 0 // 128;

static bool UsingMiniAudio = false;
u16* MixBuffer;

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
    DeviceConfig.sampleRate = SAMPLE_RATE;
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

    float initTime = time_since_start(timer);

    logf("[miniaudio] | %.1f ms | DefaultDevice '%s' with %u channels at %u hz "
         "and "
         "buffer size %i was initialized",
         initTime,
         deviceInfos[defaultDevice].name,
         Device.playback.channels,
         Device.sampleRate,
         DeviceConfig.periodSizeInFrames);
}

void audio_init()
{
    Clock timer = {};
    timer_start(timer);

    LogAsioDrivers();
    int succes = asio_init();
    if (succes != 0)
    {
        init_via_miniaudio();
        UsingMiniAudio = true;
    }
    else
    {
        asio_start();
    }

    float initTime = time_since_start(timer);
    logf("| %.1f ms | [Audio] init", initTime);
}

void audio_update()
{
    // TODO: TESTING ONLY
    // asio_start();
    // for each channel -> call channel.update()
}

void audio_dispose()
{
    // if the audio was already disposed this runs into a endless loop
    // TODO: check device and context individually
    if (UsingMiniAudio)
    {
        ma_device_uninit(&Device);
        ma_context_uninit(&Context);
        UsingMiniAudio = false;
        delete Buffers;
        log("[miniaudio] Disposed");
    }
    else
    {
        asio_dispose();
    }
}
