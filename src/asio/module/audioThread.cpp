#include "../internal.h"
#include <cassert>
#include <cstdint>

// TODO: is it initialized correctly?
// TODO: handle Interleaved vs output channel wise conversion
// -> during loading ...

float convert_PCM16_to_float(u16 sample)
{
    int16_t singed = sample;
    return (float)singed / (INT16_MAX + 1);
}

void bufferSwitch(long index, ASIOBool processNow)
{
    if (AudioEvent.load() == 0)
    {
        AudioEvent.fetch_or(AUDIO_START, memory_order_acq_rel);
        AudioEvent.fetch_or(AUDIO_START);
    }

    // zero out buffer, before new mixing
    // -> can be optimized?
    u16* out = MixBuffer;
    for (int i = 0; i < BufferSize * OutputChannels; i++)
    {
        out[i] = 0;
    }

    PlaybackPool* pool = &Playbacks;

    bool soundsAreActive = false;
    if (pool)
    {
        bool isFirstSound = true;
        for (int i = 0; i < pool->max_size; i++)
        {
            // interleaved data
            Playback* cur = &pool->data[i];
            if (cur->is_playing)
            {
                soundsAreActive = true;
                int cursorPositionBefore = cur->cursor_position;
                cur->cursor_position += BufferSize;

                // TEST: dunno if this works properly for solo audio,
                // probably not, because it needs to be mixed differently
                int sampleDataCount = cur->data->channels * BufferSize;
                // sound ends before buffer end
                int samplesReadBefore = cursorPositionBefore *
                                        cur->data->channels;

                if (cur->cursor_position >= cur->data->samples_per_channel)
                {
                    int framesToRead = cur->data->total_samples -
                                       samplesReadBefore;
                    sampleDataCount = framesToRead;
                }

                /*logf("Start mixing sound %i, starting at cursor pos: %i, %i "
                     "samples this buffer",
                     i,
                     cursorPositionBefore,
                     sampleDataCount);*/
                if (isFirstSound)
                {
                    // first sound should override old buffer data!
                    // (dunno if it is nulled already)
                    isFirstSound = false;
                    for (int i = 0; i < sampleDataCount; i++)
                    {
                        u16 sample = cur->data->pcm_data[i + samplesReadBefore];
                        u16 adjustedSample = adjust_volume(sample, cur->volume);
                        out[i] = adjustedSample;
                    }
                }
                else
                {
                    for (int i = 0; i < sampleDataCount; i++)
                    {
                        u16 sample = cur->data->pcm_data[i + samplesReadBefore];
                        u16 adjustedSample = adjust_volume(sample, cur->volume);
                        out[i] = mix_and_clip(out[i], adjustedSample);
                    }
                }

                if (cur->cursor_position >= cur->data->samples_per_channel)
                {
                    cur->is_playing = false;
                    cur->cursor_position = 0;
                }
            }
        }
    }

    if (soundsAreActive)
    {
        /*logf("Sound samples:");
        for (int i = 0; i < 64; i++)
        {
            logf("%i: %i", i, out[i]);
        }*/
    }

    for (int i = 0; i < OutputChannels; i++)
    {
        // logf("Writing output channel %i", i);
        float* curOut = (float*)ChannelBuffers[i + InputChannels]
                                                                .buffers[index];

        // input is interleaved 0 1 2 0 1 2 0 1 2 eg
        // -> we need to convert it for each buffer
        long outS = 0;
        for (long interS = i; interS < BufferSize * OutputChannels;
             interS += OutputChannels)
        {
            float outputSample = convert_PCM16_to_float(MixBuffer[interS]);
            curOut[outS++] = 0.5 * outputSample;
        }
    }
}

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

void sampleRateDidChange(ASIOSampleRate sRate)
{
    logf("Spmale rate did change?");
}
long asioMessage(long selector, long value, void* message, double* opt)
{
    logf("[ASIO] Message received %ld - %ld", selector, value);
    return 0;
}
