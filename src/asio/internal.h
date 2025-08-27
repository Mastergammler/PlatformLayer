#pragma once

#include "../audio/module.h"
#include "../define.h"
#include "../logging/module.h"
#include "../timing/module.h"
#include "imports.h"
#include "module.h"

using namespace std;

extern AsioDrivers* asioDrivers; // declared in asiodrivers.cpp
extern ASIOBufferInfo* ChannelBuffers;
extern long OutputChannels;
extern long InputChannels;
extern int BufferSize;
extern u16* MixBuffer;

/** Direct ASIO Driver DSP callback */
ASIOTime* bufferSwitchTimeInfo(ASIOTime* params,
                               long doubleBufferIndex,
                               ASIOBool directProcess);

void bufferSwitch(long index, ASIOBool processNow);
void sampleRateDidChange(ASIOSampleRate sRate);
long asioMessage(long selector, long value, void* message, double* opt);
