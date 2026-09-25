#pragma once
namespace aether::studio {
struct State { unsigned bpm; unsigned step; unsigned pattern; unsigned note; unsigned velocity; unsigned voices; };
void init(); void tick(); void reset(); void setBpm(unsigned); void trigger(unsigned,unsigned); State state();
}
