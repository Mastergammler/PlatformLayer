#include "../internal.h"
#include <cassert>
#include <cstdint>

// TODO: is it initialized correctly?
// TODO: handle Interleaved vs output channel wise conversion
// -> during loading ...

#define MASTER_LEVEL 0.35

float convert_PCM16_to_float(u16 sample)
{
    int16_t singed = sample;
    return (float)singed / (INT16_MAX + 1);
}

static int switchCounter = 0;

/*
 * According to common folk lore, this function is called when the
 * first sample of the previous buffer is getting playde
 * So theoretically there should be a delay of 1x buffer size
 * For some reason, the asio latency reported is double that,
 * not 100% why that is
 */
void bufferSwitch(long index, ASIOBool processNow)
{

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
                int cursorStart = cur->cursor_position;
                int nextCursorPosition = cur->cursor_position + BufferSize;
                int cursorEnd = cur->data->samples_per_channel;
                int totalBufferSize = BufferSize * cur->data->channels;

                // TEST: dunno if this works properly for solo audio,
                // probably not, because it needs to be mixed differently
                // at least for left and right
                int audioSamplesToPlay;
                // sound ends before buffer end
                int sampleStartPosition = cursorStart * cur->data->channels;

                // play partial buffer
                if (nextCursorPosition >= cursorEnd)
                {
                    int samplesLeftInAudio = cur->data->total_samples -
                                             sampleStartPosition;
                    audioSamplesToPlay = samplesLeftInAudio;
                }
                else // play whole buffer
                {
                    audioSamplesToPlay = totalBufferSize;
                }

                int samplesLeftInPlaybackBuffer = totalBufferSize -
                                                  audioSamplesToPlay;
                if (cur->loop)
                {
                    if (samplesLeftInPlaybackBuffer > 0 ||
                        nextCursorPosition >= cursorEnd)
                        // converting sample position back to cursor position
                        nextCursorPosition = samplesLeftInPlaybackBuffer /
                                             cur->data->channels;
                }

                if (isFirstSound)
                {
                    if (AudioEvent.load() == 0)
                    {
                        AudioEvent.fetch_or(AUDIO_START, memory_order_acq_rel);
                        AudioEvent.fetch_or(AUDIO_START);
                    }
                    // first sound should override old buffer data!
                    // (dunno if it is nulled already)
                    isFirstSound = false;
                    for (int i = 0; i < audioSamplesToPlay; i++)
                    {
                        u16 sample = cur->data->pcm_data[i +
                                                         sampleStartPosition];
                        u16 adjustedSample = adjust_volume(sample, cur->volume);
                        out[i] = adjustedSample;
                    }

                    // for looping
                    for (int i = 0; i < samplesLeftInPlaybackBuffer; i++)
                    {
                        u16 sample = cur->data->pcm_data[i];
                        u16 adjustedSample = adjust_volume(sample, cur->volume);
                        out[i + audioSamplesToPlay] = adjustedSample;
                    }
                }
                else
                {
                    for (int i = 0; i < audioSamplesToPlay; i++)
                    {
                        u16 sample = cur->data->pcm_data[i +
                                                         sampleStartPosition];
                        u16 adjustedSample = adjust_volume(sample, cur->volume);
                        out[i] = mix_and_clip(out[i], adjustedSample);
                    }

                    // for looping
                    for (int i = 0; i < samplesLeftInPlaybackBuffer; i++)
                    {
                        u16 sample = cur->data->pcm_data[i];
                        u16 adjustedSample = adjust_volume(sample, cur->volume);
                        out[i +
                            audioSamplesToPlay] = mix_and_clip(out[i +
                                                                   audioSamplesToPlay],
                                                               adjustedSample);
                    }
                }

                if (nextCursorPosition >= cursorEnd)
                {
                    /*logf("Stopping audio: c%i nextc%i loop %d, %i samples
                       left", cur->cursor_position, nextCursorPosition,
                         cur->loop,
                         samplesLeftInPlaybackBuffer);*/
                    cur->is_playing = false;
                    cur->cursor_position = 0;
                }
                else
                {
                    cur->cursor_position = nextCursorPosition;
                }
            }
        }
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
            curOut[outS++] = MASTER_LEVEL * outputSample;
        }
    }
}

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
