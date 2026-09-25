#pragma once
#include <nds.h>

namespace aether::quantum {
struct BackendInfo { const char* name; bool local; bool externalCapable; };
struct Simulator {
    int qubits;
    int shots;
    int lastMeasurement;
    float probability[8];
    bool bellState;
};
BackendInfo backend();
void init(Simulator& q);
void reset(Simulator& q);
void hadamard(Simulator& q, int qbit);
void pauliX(Simulator& q, int qbit);
void cnot(Simulator& q, int control, int target);
void runBell(Simulator& q);
void runGrover2(Simulator& q);
int measure(Simulator& q);
void tick(Simulator& q);
}
