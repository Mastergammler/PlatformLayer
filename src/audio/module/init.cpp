#include "../internal.h"

ma_context Context;
ma_device_config DeviceConfig;
ma_device Device;
ma_resource_manager_config RmConfig;
ma_resource_manager Rm;
int SampleRate;
static bool UsingMiniAudio = false;

u16* MixBuffer;

Clock AT_Timer;
PerformanceInfoBuffer PerformanceInfo;

void audio_init()
{
    Clock timer = {};
    timer_start(timer);

    PerformanceInfo.buffer_size = 1024;
    PerformanceInfo.info_buffer = new AtPerformanceInfo
                                            [PerformanceInfo.buffer_size]();

    LogAsioDrivers();
    int succes = asio_init();
    if (succes != 0)
    {
        init_via_miniaudio();
        UsingMiniAudio = true;
    }
    else
    {
        timer_start(AT_Timer);
        asio_start();
        SampleRate = AsioSampleRate;
    }

    RmConfig = ma_resource_manager_config_init();
    RmConfig.decodedChannels = 2;
    RmConfig.decodedFormat = ma_format_s16; // PCM16
    RmConfig.decodedSampleRate = SampleRate;

    ma_result result = ma_resource_manager_init(&RmConfig, &Rm);
    if (result != MA_SUCCESS)
    {
        logf("Unable to initialize resource manager: %i", result);
    }

    float initTime = time_since_start(timer);
    logf("| %.1f ms | [Audio] init", initTime);
}

void audio_update()
{
    // TODO: TESTING ONLY
    // asio_start();
    // for each channel -> call channel.update()
    // -> Does this make sense? Would this not happend per audio callback?
    // => But this would mean more resource use there?
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
