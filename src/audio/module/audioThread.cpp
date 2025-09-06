#include "../internal.h"
#include <climits>
#include <cstdint>

// 16 loaded audio clips
int BufferCount = 16;
int BufferIndex = 0;
ma_audio_buffer* Buffers;

atomic<int> AudioEvent(0);
atomic<int> FramesPassed(0);

// FIXME: there seems to be a lag on the second run if looping is enabled
//  -> it seems like on buffer switch there are some samples dropped or
//  something
//  => Some drum beats are just a little bit late, but it's noticable!
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

    FramesPassed.fetch_add(frameCount, std::memory_order_release);
    if (AudioEvent.load() == 0)
    {
        AudioEvent.fetch_or(AUDIO_START, memory_order_acq_rel);
        AudioEvent.fetch_or(AUDIO_START);
    }

    PlaybackPool* pool = (PlaybackPool*)device->pUserData;
    // TODO: needs to match the defined type
    u16* out = (u16*)output;

    if (pool)
    {
        bool isFirstSound = true;
        for (int i = 0; i < pool->max_size; i++)
        {
            Playback* cur = &pool->data[i];
            if (cur->is_playing)
            {

                int cursorPositionBefore = cur->cursor_position;
                cur->cursor_position += frameCount;

                // TEST: dunno if this works properly for solo audio,
                // probably not, because it needs to be mixed differently
                int sampleDataCount = cur->data->channels * frameCount;
                // sound ends before buffer end
                int samplesReadBefore = cursorPositionBefore *
                                        cur->data->channels;

                if (cur->cursor_position >= cur->data->samples_per_channel)
                {
                    int framesToRead = cur->data->total_samples -
                                       samplesReadBefore;
                    sampleDataCount = framesToRead;
                }

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
                        out[i] = mix_defensively(out[i], adjustedSample);
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
}
