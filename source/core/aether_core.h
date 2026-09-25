#pragma once
#include <nds.h>

namespace aether {
struct SystemState {
    bool sdReady;
    bool touchReady;
    bool benchmarkComplete;
    bool safeMode;
    bool sdWriteReady;
    bool audioReady;
    bool quantumReady;
    bool projectSaved;
    u32 frame;
    int selectedModule;
    int screen;
};

void init(SystemState& state);
void update(SystemState& state);
void render(const SystemState& state);
void shutdown();
}
