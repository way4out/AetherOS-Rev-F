#pragma once
namespace aether::recovery {
void init(); void heartbeat(); void fault(const char* module); bool safeMode(); const char* lastFault();
}