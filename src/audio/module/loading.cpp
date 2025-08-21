#include "../internal.h"

// TEST: checking basic playback
void audio_load_sound(Audio& sound, std::string file)
{
    Clock timer = {};
    timer_start(timer);
    // ma_engine_play_sound(&Engine, file.c_str(), NULL);

    logf("| %.1f ms | Loading file '%s'",
         time_since_start(timer),
         file.c_str());
}
