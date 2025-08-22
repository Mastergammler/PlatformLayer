#include "../internal.h"

ma_context Context;
ma_device_config DeviceConfig;
ma_device Device;

static bool AudioDisposed = false;

void audio_init()
{
    AudioDisposed = false;
    ma_result result;
    result = ma_context_init(NULL, 0, NULL, &Context);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Unable to retrieve audio context %i", result);
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
        logf("[Audio] Unable to read hardware devices: %i", result);
        audio_dispose();
        return;
    }

    if (deviceCount == 0)
    {
        logf("[Audio] No output devices found.");
        audio_dispose();
        return;
    }

    int defaultDevice = 0;
    for (ma_uint32 i = 0; i < deviceCount; i++)
    {
        logf("[Audio] Device %u: %s", i, deviceInfos[i].name);
        if (deviceInfos[i].isDefault) defaultDevice = i;
    }

    DeviceConfig = ma_device_config_init(ma_device_type_playback);
    DeviceConfig.playback.pDeviceID = &deviceInfos[defaultDevice].id;
    // use default values
    DeviceConfig.playback.format = ma_format_unknown;
    DeviceConfig.playback.channels = 0;
    DeviceConfig.sampleRate = 0;

    result = ma_device_init(&Context, &DeviceConfig, &Device);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Device '%s' could not be initalized: %i",
             deviceInfos[defaultDevice].name,
             result);
        audio_dispose();
        return;
    }

    result = ma_device_start(&Device);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Device '%s' could not be started for playback: %i",
             deviceInfos[defaultDevice].name,
             result);
        audio_dispose();
        return;
    }

    logf("[Audio] DefaultDevice '%s' with %u channels at %u hz was initalized",
         deviceInfos[defaultDevice].name,
         Device.playback.channels,
         Device.sampleRate);
}

void audio_update()
{
}

void audio_dispose()
{
    // if the audio was already disposed this runs into a endless loop
    if (AudioDisposed) return;

    ma_device_uninit(&Device);
    ma_context_uninit(&Context);
    AudioDisposed = true;
    log("[Audio] Disposed");
}
