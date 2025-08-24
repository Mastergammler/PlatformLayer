#pragma once

#include <chrono>

#include "types.h"

using timepoint = std::chrono::steady_clock::time_point;

void timer_start(Clock& timer);
void timer_update(Clock& timer);
void timer_wait_till_next_frame(Clock& timer);

bool time_to_update(Clock& timer, float updateIntervalS);
float time_since_update(Clock& timer);
float time_since_start(Clock& timer);
timepoint time_stamp(Clock& clock);
