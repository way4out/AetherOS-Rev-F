#pragma once
namespace aether::graph {
enum Node { NODE_QUANTUM, NODE_AUDIO, NODE_DSP, NODE_LAB, NODE_AI, NODE_NETWORK, NODE_RADIO, NODE_COUNT };
struct State { bool enabled[NODE_COUNT]; unsigned ticks; };
void init(); void tick(); State state();
}