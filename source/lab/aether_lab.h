#pragma once
namespace aether::lab { struct Metrics{unsigned entropy,monteCarlo,automata,physics;}; void init(); void tick(); Metrics metrics(); }