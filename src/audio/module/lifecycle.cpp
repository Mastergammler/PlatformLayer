#include "../internal.h"

ma_context Context;
ma_device_config DeviceConfig;
ma_device Device;

// 16 loaded audio clips
int BufferCount = 16;
int BufferIndex = 0;
ma_audio_buffer* Buffers;

static bool AudioDisposed = false;
atomic<int> AudioEvent(0);

// 0 = choose default
#define SAMPLE_RATE 44100;

void data_callback(ma_device* device,
                   void* output,
                   const void* input,
                   ma_uint32 frameCount)
{
    // TODO: remove this in audio thread
    // -> Play a FAKE audio (everything 0 or something) for the first time just
    // to query this
    // => Then ignore it for subsequent calls
    static int firstcall = 1;
    if (firstcall)
    {
        logf("Buffer size in frames (%i)", frameCount);
        firstcall = 0;
    }

    if (AudioEvent.load() == 0)
    {
        AudioEvent.fetch_or(AUDIO_START, memory_order_acq_rel);
        AudioEvent.fetch_or(AUDIO_START);
    }

    ma_audio_buffer* buffer = (ma_audio_buffer*)device->pUserData;
    if (buffer)
    {
        ma_audio_buffer_read_pcm_frames(buffer, output, frameCount, true);
    }
}

void audio_init()
{
    Clock timer = {};
    timer_start(timer);
    AudioDisposed = false;

    Buffers = new ma_audio_buffer[BufferCount];

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
    DeviceConfig.playback.format = ma_format_s16;
    DeviceConfig.playback.channels = 0;
    DeviceConfig.sampleRate = SAMPLE_RATE;
    DeviceConfig.dataCallback = data_callback;

    // For simple tasks even 32 is feasable
    // default seems to be 480 = 10ms for 48000 Hz
    // DeviceConfig.periodSizeInFrames = 32;

    result = ma_device_init(&Context, &DeviceConfig, &Device);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Device '%s' could not be initalized: %i",
             deviceInfos[defaultDevice].name,
             result);
        audio_dispose();
        return;
    }

    float initTime = time_since_start(timer);

    logf("[Audio] | %.1f ms | DefaultDevice '%s' with %u channels at %u hz and "
         "buffer size %i was initialized",
         initTime,
         deviceInfos[defaultDevice].name,
         Device.playback.channels,
         Device.sampleRate,
         DeviceConfig.periodSizeInFrames);
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
    delete Buffers;
    log("[Audio] Disposed");
}
