#pragma once
#include <nds.h>
namespace aether {
struct SystemState {
    bool sdReady;
    bool touchReady;
    bool benchmarkComplete;
    bool safeMode;
    u32 frame;
};
void init(SystemState& state);
void update(SystemState& state);
void render(const SystemState& state);
}
