#pragma once
#include <nds.h>
namespace aether::gate {
enum Kind { GATE_5G=0, GATE_SAT, GATE_BT, GATE_SDR, GATE_QPU, GATE_AI, GATE_COUNT };
struct Status { bool configured; bool online; unsigned latencyMs; unsigned bandwidthKbps; unsigned packets; unsigned errors; };
void init();
void tick();
bool configure(Kind k, const char* endpoint, unsigned port);
Status status(Kind k);
bool anyOnline();
unsigned onlineCount();
const char* name(Kind k);
}
