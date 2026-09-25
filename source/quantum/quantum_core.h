#pragma once
#include <nds.h>
namespace aether::quantum {
struct BackendInfo { const char* name; bool local; bool externalCapable; };
struct Simulator { int qubits; int shots; int lastMeasurement; float probability[8]; bool bellState; int algorithm; };
BackendInfo backend(); void init(Simulator&); void reset(Simulator&); void setQubits(Simulator&,int); void hadamard(Simulator&,int); void pauliX(Simulator&,int); void cnot(Simulator&,int,int); void runBell(Simulator&); void runGrover2(Simulator&); void runDeutschJozsa(Simulator&); void runQFT2(Simulator&); void runTeleportation(Simulator&); int measure(Simulator&); void tick(Simulator&);
}