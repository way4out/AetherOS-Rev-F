#pragma once
#include <nds.h>
namespace aether::securitylab {
enum Mode : u8 { PASSIVE_RF=0, AUTH_NETWORK=1, LAB_SIMULATION=2, GATEWAY_HARDENING=3, MODE_COUNT };
struct Report { Mode mode; bool consent; bool txLocked; bool credentialCaptureLocked; bool destructiveLocked; u32 samples; u32 devices; u32 packets; u32 alerts; u32 labRuns; };
void init(); void tick(); void setMode(Mode); void acknowledge(); void sample(); void analyze(); void runLabSimulation(); Report report(); const char* modeName(Mode); const char* warning();
}
