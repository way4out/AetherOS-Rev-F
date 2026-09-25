#include "capacity_engine.h"
#include "../hardware/hardware_profile.h"
#include "../engine/engine_modules.h"
namespace { aether::capacity::Report r; }
namespace aether::capacity {
void init(){ r={1024,256,0,0,13,false}; indexWorkspace(); }
void indexWorkspace(){ r.sdExpansion=aether::hardware::sdAvailable(); r.indexedAssets=r.sdExpansion?1024:0; r.projects=aether::engine::projectExists()?1:0; }
void tick(){ static u16 n=0; if((++n&255)==0) indexWorkspace(); }
Report report(){ return r; }
const char* status(){ return r.sdExpansion ? "1GB CLASS WORKSPACE READY" : "INTERNAL CAPACITY MODE"; }
}
