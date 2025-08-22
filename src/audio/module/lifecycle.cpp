#include "../internal.h"

ma_context Context;
ma_device_config DeviceConfig;
ma_device Device;

static bool AudioDisposed = false;

void data_callback(ma_device* device,
                   void* output,
                   const void* input,
                   ma_uint32 frameCount)
{
    ma_decoder* decoder = (ma_decoder*)device->pUserData;
    if (decoder)
    {
        ma_result res = ma_decoder_read_pcm_frames(decoder,
                                                   output,
                                                   frameCount,
                                                   NULL);
        if (res != MA_SUCCESS)
        {
            // Will be called after the audio ends
            // logf("[Audio] Playback error: %i", res);
        }
    }
    /*else
    {
        log("Decoder not present!!");
    }*/
}

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

    // TODO: needs to be set before the device is initialized
    DeviceConfig.playback.format = ma_format_s24;
    DeviceConfig.playback.channels = 0;
    DeviceConfig.sampleRate = 0;
    DeviceConfig.dataCallback = data_callback;

    result = ma_device_init(&Context, &DeviceConfig, &Device);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Device '%s' could not be initalized: %i",
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
    // TODO: check device and context individually
    if (AudioDisposed) return;

    ma_device_uninit(&Device);
    ma_context_uninit(&Context);
    AudioDisposed = true;
    log("[Audio] Disposed");
}
