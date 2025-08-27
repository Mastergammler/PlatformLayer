#include "../internal.h"
#include <asio.h>
#include <cassert>

static ASIOCallbacks asioCb;
ASIOBufferInfo* ChannelBuffers;
long OutputChannels;
long InputChannels;
int BufferSize;

int asio_init()
{
    Clock timer = {};
    timer_start(timer);

    asioDrivers = new AsioDrivers();

    int numDrivers = asioDrivers->asioGetNumDev();
    if (numDrivers < 1)
    {
        logf("[ASIO] No ASIO Drivers found");
        return -1;
    }

    // Choose driver by index or name
    char driverName[32];
    int result;
    if ((result = asioDrivers->asioGetDriverName(0,
                                                 driverName,
                                                 sizeof(driverName))))
    {
        logf("[ASIO] Failed to get driver name: err %i", result);
        return -1;
    }
    logf("[ASIO] Loading ASIO Driver: %s", driverName);

    if (!asioDrivers->loadDriver(driverName))
    {
        logf("[ASIO] Failed to load ASIO driver");
        return -1;
    }

    // ASIODriverInfo to store driver info
    ASIODriverInfo driverInfo;
    if (ASIOInit(&driverInfo) != ASE_OK)
    {
        logf("[ASIO] Failed to initialize ASIO driver");
        return -1;
    }

    logf("[ASIO] Driver initialized: %s", driverInfo.name);

    // Example: get channels
    if (ASIOGetChannels(&InputChannels, &OutputChannels) == ASE_OK)
    {
        logf("[ASIO] Inputs: %i, Outputs: %i", InputChannels, OutputChannels);
    }

    long minSize = 0, maxSize = 0, preferredSize = 0, granularity = 0;
    ASIOError err = ASIOGetBufferSize(&minSize,
                                      &maxSize,
                                      &preferredSize,
                                      &granularity);
    BufferSize = preferredSize;
    if (err == ASE_OK)
    {
        logf("[ASIO] Buffer size (%ld - %ld) pref: %ld granualirty %ld",
             minSize,
             maxSize,
             preferredSize,
             granularity);
    }
    else
    {
        logf("[ASIO] Error during queurying buffer size for device %s",
             driverInfo.name);
    }

    ASIOSampleRate sampleRate;
    err = ASIOGetSampleRate(&sampleRate);
    if (err == ASE_OK)
    {
        logf("[ASIO] Device sample rate: %.f Hz", sampleRate);
    }
    else
    {
        logf("[ASIO] Unable to query sample rate for device %s",
             driverInfo.name);
    }

    /*err =
ASIOSetSampleRate(sampleRate); if
(err == ASE_OK)
{
    logf("[ASIO] Set asio sample
rate to: %.f Hz", sampleRate);
}
else
{
    logf("[ASIO] Unable to set
sample rate for device to %.f",
sampleRate);
}*/

    // NOTE: it seems most asio drivers
    // will only use bufferswitch
    //-> buffer time info is newer and
    // is often not implemented
    asioCb.bufferSwitch = &bufferSwitch;
    asioCb.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;
    asioCb.sampleRateDidChange = &sampleRateDidChange;
    asioCb.asioMessage = &asioMessage;

    int totalChannels = InputChannels + OutputChannels;
    ChannelBuffers = new ASIOBufferInfo[totalChannels];
    for (int i = 0; i < InputChannels; i++)
    {
        ChannelBuffers[i].isInput = true;
        ChannelBuffers[i].channelNum = i;
        ChannelBuffers[i].buffers[0] = nullptr;
        ChannelBuffers[i].buffers[1] = nullptr;
    }
    for (int i = 0; i < OutputChannels; i++)
    {
        ChannelBuffers[InputChannels + i].isInput = false;
        ChannelBuffers[InputChannels + i].channelNum = i;
        ChannelBuffers[InputChannels + i].buffers[0] = nullptr;
        ChannelBuffers[InputChannels + i].buffers[1] = nullptr;
    }

    assert(OutputChannels > 1);

    err = ASIOCreateBuffers(ChannelBuffers, totalChannels, BufferSize, &asioCb);
    if (err != ASE_OK)
    {
        logf("[ASIO] Device buffer creation failed: %ld", err);
        return -1;
    }
    else
    {
        ASIOChannelInfo info;
        info.channel = 0;
        info.isInput = false;
        err = ASIOGetChannelInfo(&info);
        if (err == ASE_OK)
        {
            logf("[ASIO] Device bit depthts : %d ", info.type);
        }
        else
        {
            logf("[ASIO] Could not determine bit depth.");
            return -1;
        }

        long inputLatency, outputLatency;
        err = ASIOGetLatencies(&inputLatency, &outputLatency);
        if (err == ASE_OK)
        {
            logf("[ASIO] Reported driver latency (in/out):  %ld spls/%ld spls",
                 inputLatency,
                 outputLatency);
        }
    }

    float elapsed = time_since_start(timer);
    logf("| %.1f ms | [ASIO] Device initalization", elapsed);

    return 0;
}

void asio_start()
{
    logf("[ASIO] Start");
    MixBuffer = new u16[BufferSize * OutputChannels];
    logf("[ASIO] Output channels: %i", OutputChannels);
    logf("[ASIO] Buffer size %i", BufferSize);
    logf("[ASIO] Initialized mix buffer of size %i",
         BufferSize * OutputChannels);
    ASIOStart();
}

int asio_dispose()
{
    // Clean up
    ASIOStop();
    ASIODisposeBuffers();
    delete ChannelBuffers;
    asioDrivers->removeCurrentDriver();
    delete asioDrivers;

    logf("[ASIO] Disposed");

    return 0;
}
