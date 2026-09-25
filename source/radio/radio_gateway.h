#pragma once
namespace aether::radio {
struct Capability { bool bluetooth; bool fiveG; bool satellite; bool sdr; };
Capability capabilities(); void init(); bool configured(); 
}