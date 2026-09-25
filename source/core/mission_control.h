#pragma once
#include <nds.h>

namespace aether::mission {
struct Report {
    bool boot;
    bool sd;
    bool config;
    bool project;
    bool quantum;
    bool audio;
    bool dsp;
    bool lab;
    bool ai;
    bool network;
    bool security;
    bool recovery;
    bool gateway;
    u8 score;
    u32 ticks;
};
void init();
void tick();
void refresh(const Report* hint = 0);
Report report();
const char* state(bool ok);
}
