#pragma once
#include <nds.h>
namespace aether::session {
enum State { DOWN, DISCOVERING, AUTHENTICATING, ONLINE, DEGRADED };
struct Peer { State state; u32 id; u32 lastSeen; u32 latency; u32 bandwidth; bool authenticated; };
void init(); void tick(); bool begin(u32); bool receiveHello(u32); bool authenticate(u32); Peer status();
}