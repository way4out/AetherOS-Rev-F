#pragma once
namespace aether::governor {
struct Profile { unsigned quantumQubits; unsigned audioVoices; unsigned visualLevel; unsigned networkKbps; };
void init(); void tick(); Profile profile();
}