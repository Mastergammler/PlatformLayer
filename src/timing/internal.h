#pragma once

#include "imports.h"
#include "module.h"
#include <chrono>

// TODO: precision clock might by steady_clock or system_clock on different
// systems
//  and then the time_span is not convrtable between those
// using precision_clock = std::chrono::high_resolution_clock;
using precision_clock = std::chrono::steady_clock;
using duration_s = std::chrono::duration<float>;
using duration_ms = std::chrono::duration<float, std::milli>;
using ms = std::chrono::milliseconds;
using namespace std::this_thread;
