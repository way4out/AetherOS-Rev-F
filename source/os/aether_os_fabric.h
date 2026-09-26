#pragma once
#include <nds.h>
namespace aether::osfabric {
enum Profile : u8 { AETHEROS=0, RECOVERY, SCIENCE, GATEWAY, CREATIVE, PROFILE_COUNT };
struct State { Profile active; u32 ticks; u32 switches; bool healthy; };
void init();
void tick();
void cycle();
void select(Profile p);
const State& state();
const char* name();
const char* mode();
}
