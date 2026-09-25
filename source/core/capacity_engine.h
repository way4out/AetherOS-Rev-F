#pragma once
#include <nds.h>
namespace aether::capacity {
struct Report { u32 workspaceMB; u32 cacheMB; u32 indexedAssets; u32 projects; u32 modules; bool sdExpansion; };
void init();
void tick();
void indexWorkspace();
Report report();
const char* status();
}
