#include "internal.h"

DivisionCounter* beat_find_division(BeatClock& clock, float divisionsPerBeat);

void beat_init(BeatClock& clock,
               float bpm,
               int beatsPerMeasure,
               float* divisions,
               int divisionCount);
void beat_start(BeatClock& clock);
void beat_update(BeatClock& clock);
