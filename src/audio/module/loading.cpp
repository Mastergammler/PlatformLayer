#include "../internal.h"

ma_decoder Decoder;

// TEST: checking basic playback
void audio_load_sound(Audio& sound, std::string file)
{
    Clock timer = {};
    timer_start(timer);

    ma_result result;
    result = ma_decoder_init_file(file.c_str(), NULL, &Decoder);

    if (result != MA_SUCCESS)
    {
        logf("[Audio] Unable to init decoder for file '%s': %i",
             file.c_str(),
             result);
        return;
    }

    if (Decoder.outputFormat != DeviceConfig.playback.format)
    {
        logf("Audio in format %i but expected format %i",
             Decoder.outputFormat,
             DeviceConfig.playback.format);
    }
    else
    {
        // TODO: is this a good way of handling this?
        // -> it's quite async
        Device.pUserData = &Decoder;
    }

    logf("| %.1f ms | Loading file '%s'",
         time_since_start(timer),
         file.c_str());
}
