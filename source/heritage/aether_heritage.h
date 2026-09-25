#pragma once
#include <nds.h>
namespace aether::heritage {
enum Capability : u8 { OEQL_PROTOCOL=0, OEQL_QUANTUM, STELLARPHONE_BRIDGE, STELLARPHONE_ACCESS, MARTIN_NIR, MARTIN_SWIR, MARTIN_OPTICAL_VOID, HERITAGE_COUNT };
struct State { bool oeql; bool stellarphone; bool martinOptics; bool quantumGateway; bool commBridge; u32 capabilityMask; };
void init(); void tick(); const State& state(); bool enabled(Capability); const char* status(); const char* protocol(); const char* opticalProfile();
}
