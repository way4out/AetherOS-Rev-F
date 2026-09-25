#pragma once
namespace aether::hil {
struct Report { bool protocol; bool storage; bool quantum; bool audio; bool gateways; unsigned score; };
void init(); void tick(); Report report();
}
