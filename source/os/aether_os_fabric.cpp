#include "aether_os_fabric.h"
namespace { aether::osfabric::State s={aether::osfabric::AETHEROS,0,0,true}; }
namespace aether::osfabric {
void init(){s={AETHEROS,0,0,true};}
void tick(){++s.ticks;s.healthy=true;}
void cycle(){s.active=(Profile)((s.active+1)%PROFILE_COUNT);++s.switches;}
void select(Profile p){if(p<PROFILE_COUNT){s.active=p;++s.switches;}}
const State& state(){return s;}
const char* name(){switch(s.active){case AETHEROS:return "AETHEROS";case RECOVERY:return "RECOVERY";case SCIENCE:return "SCIENCE";case GATEWAY:return "GATEWAY";default:return "CREATIVE";}}
const char* mode(){switch(s.active){case AETHEROS:return "FULL DESKTOP";case RECOVERY:return "SAFE RECOVERY";case SCIENCE:return "LAB + QUANTUM";case GATEWAY:return "REMOTE FABRIC";default:return "STUDIO + MEDIA";}}
}
