#pragma once
#include <nds.h>

namespace aether {
struct SystemState {
    bool sdReady;
    bool touchReady;
    bool benchmarkComplete;
    bool safeMode;
    bool sdWriteReady;
    u32 frame;
    int selectedModule;
};

void init(SystemState& state);
void update(SystemState& state);
void render(const SystemState& state);
void shutdown();
}
