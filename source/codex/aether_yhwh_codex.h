#pragma once
#include <nds.h>
namespace aether::codex {
static const bool REVOLUTION_INVARIANT=true;
static const char* const CANONICAL_REPO="way4out/YHWH-CODEX";
static const char* const CANONICAL_COMMIT="158d1af6c93015bf4c158ba8b5d55b08d0708913";
struct Entry { const char* name; const char* sourceSha; u32 bytes; };
void init(); void tick(); bool ready(); unsigned count(); const Entry* entries(); const char* status();
}