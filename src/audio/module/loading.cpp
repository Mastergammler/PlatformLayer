#include "../internal.h"

// TEST: checking basic playback
void audio_load_sound(Audio& sound, std::string file)
{
    if (BufferIndex >= BufferCount)
    {
        logf("Buffer space holds %i items, loading aborted!", BufferCount);
        return;
    }

    Clock timer = {};
    timer_start(timer);

    ma_decoder decoder;
    ma_result result;
    result = ma_decoder_init_file(file.c_str(), NULL, &decoder);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Unable to init decoder for file '%s': %i",
             file.c_str(),
             result);
        return;
    }

    if (decoder.outputFormat != DeviceConfig.playback.format)
    {
        logf("[WARN] Audio in format %i but expected format %i",
             decoder.outputFormat,
             DeviceConfig.playback.format);
    }

    // TODO: resampling is not done automatically
    //  -> need to use the resource manager to do this instead ...

    ma_resource_manager_data_source dataSource;
    result = ma_resource_manager_data_source_init(
                                            &Rm,
                                            file.c_str(),
                                            MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE, // No STREAM flag
                                            NULL, // Async notification (not
                                            &dataSource);
    if (result != MA_SUCCESS)
    {
        logf("[Audio] Unable to init datasource for file '%s': %i",
             file.c_str(),
             result);
        return;
    }

    // 1 frame = 1 sample per channel
    // this returns the resampled length
    ma_uint64 sampleCount;
    ma_data_source_get_length_in_pcm_frames(&dataSource, &sampleCount);

    sound.channels = decoder.outputChannels;
    sound.total_samples = sampleCount * sound.channels;
    sound.samples_per_channel = sampleCount;
    sound.sample_rate = decoder.outputSampleRate;
    sound.pcm_data = new u16[sound.total_samples];
    sound.file = file;
    sound.length_s = (float)sound.samples_per_channel / sound.sample_rate;
    sound.buffer_index = BufferIndex;

    ma_uint64 framesRead;
    // reads all channels simultaneously
    // 1 frame = 1 sample per channel
    ma_data_source_read_pcm_frames(&dataSource,
                                   sound.pcm_data,
                                   sound.total_samples,
                                   &framesRead);
    sound.loaded = true;

    logf("| %.1f ms | Loading audio '%s' %i channels at %i Hz (%i frames - "
         "%.2fs)",
         time_since_start(timer),
         file.c_str(),
         sound.channels,
         sound.sample_rate,
         sound.samples_per_channel,
         sound.length_s);
}
