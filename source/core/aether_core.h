#pragma once
#include <nds.h>
#include "../quantum/quantum_core.h"
namespace aether {
struct SystemState {
    bool sdReady; bool touchReady; bool benchmarkComplete; bool safeMode;
    bool sdWriteReady; bool audioReady; bool quantumReady; bool projectSaved;
    u32 frame; int selectedModule; int screen; bool networkReady; bool gatewayConfigured;
};
void init(SystemState&);
void update(SystemState&);
void render(const SystemState&);
void shutdown();
quantum::Simulator& simulator();
}