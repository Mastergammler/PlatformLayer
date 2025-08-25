#include "../internal.h"
#include <cstdint>

static int switchCounter = 0;
ASIOTime* bufferSwitchTimeInfo(ASIOTime* params,
                               long doubleBufferIndex,
                               ASIOBool directProcess)
{
    logf("switchCounter called %i", ++switchCounter);
    uint64_t samplePos = ((uint64_t)params->timeInfo.samplePosition.hi << 32) |
                         params->timeInfo.samplePosition.lo;
    // precise time the first sample of the current buffer will be played
    // -> the scheduled time for that playback
    uint64_t sysTimeNs = ((uint64_t)params->timeInfo.systemTime.hi << 32) |
                         params->timeInfo.systemTime.lo;
    double sampleRate = *reinterpret_cast<double*>(
                                            params->timeInfo.sampleRate.ieee);

    double positionSeconds = samplePos / sampleRate;

    static int measureCounter = 0;

    if (measureCounter++ < 5)
    {
        logf("SamplePos: %.d, Time: %.d ns, Uptime .5s",
             samplePos,
             sysTimeNs,
             positionSeconds);
    }
    return params;
}

void bufferSwitch(long index, ASIOBool processNow)
{
    // logf("[AudioThread] Processing Buffers for index %i", index);

    for (int i = 0; i < OutputChannels; i++)
    {
        float* curOut = (float*)ChannelBuffers[i + InputChannels]
                                                                .buffers[index];
        for (long s = 0; s < BufferSize; s++)
        {
            curOut[s] = 0;
        }
    }

    // logf("[AudioThread] Processing Buffers for index %i Finished", index);
}

void sampleRateDidChange(ASIOSampleRate sRate)
{
    logf("Spmale rate did change?");
}
long asioMessage(long selector, long value, void* message, double* opt)
{
    logf("[ASIO] Message received %ld - %ld", selector, value);
    return 0;
}
