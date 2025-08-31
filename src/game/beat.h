#include "internal.h"

DivisionCounter* beat_find_division(BeatCounter& clock, float divisionsPerBeat);

void beat_init(BeatCounter& clock,
               float bpm,
               int beatsPerMeasure,
               float* divisions,
               int divisionCount);
void beat_start(BeatCounter& clock);
void beat_reset(BeatCounter& clock);
void beat_update(BeatCounter& clock);
