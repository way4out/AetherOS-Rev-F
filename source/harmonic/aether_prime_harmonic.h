#pragma once
#include <nds.h>
namespace aether::harmonic {
struct Node { u16 prime; u16 harmonic; s16 voidVector; u16 dampener; u16 amplifier; u32 frequencyMilliHz; };
void init(); void tick(); void reset(); void nextPrime(); void setBase(u32 milliHz); void setDampener(u16); void setAmplifier(u16); void setVoid(s16);
const Node& node(); u32 outputMilliHz(); u32 stackHash(); const char* status();
}