#pragma once

#include "imports.h"
#include "module.h"

using precision_clock = std::chrono::high_resolution_clock;
using duration_s = std::chrono::duration<float>;
using duration_ms = std::chrono::duration<float, std::milli>;
using ms = std::chrono::milliseconds;
using namespace std::this_thread;
