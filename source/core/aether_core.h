#pragma once
#include <nds.h>
#include "../quantum/quantum_core.h"

namespace aether {
enum Module {
    MOD_CORE=0, MOD_QUANTUM, MOD_SOUND, MOD_DSP, MOD_LAB, MOD_AI,
    MOD_NETWORK, MOD_PROJECTS, MOD_RF, MOD_MARAUDER, MOD_STUDIO, MOD_SYSTEM,
    MOD_COUNT
};

struct SystemState {
    bool sdReady; bool touchReady; bool benchmarkComplete; bool safeMode;
    bool sdWriteReady; bool audioReady; bool quantumReady; bool projectSaved;
    u32 frame; int selectedModule; int screen; bool networkReady; bool gatewayConfigured;
    u32 dspTicks; u32 labTicks; u32 aiTicks; u32 rfSamples; u32 marauderFrames;
    u32 studioTicks; u32 coreTicks;
};

void init(SystemState&);
void update(SystemState&);
void render(const SystemState&);
void shutdown();
quantum::Simulator& simulator();
}
