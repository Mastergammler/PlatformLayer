#include "internal.h"

SubbeatCounter* beat_find_subb(BeatCounter& clock, float subbsPerBeat);

void beat_init(BeatCounter& clock,
               float bpm,
               int beatsPerMeasure,
               float* subbs,
               int subbCount);
void beat_start(BeatCounter& clock);
void beat_reset(BeatCounter& clock);
void beat_update(BeatCounter& clock);
