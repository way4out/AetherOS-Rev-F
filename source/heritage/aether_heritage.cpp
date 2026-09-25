#include "aether_heritage.h"
namespace { aether::heritage::State s{true,true,true,true,true,0}; }
namespace aether::heritage {
void init(){s={true,true,true,true,true,0};for(u8 i=0;i<HERITAGE_COUNT;i++)s.capabilityMask|=(1u<<i);}
void tick(){}
const State& state(){return s;}
bool enabled(Capability c){return (s.capabilityMask&(1u<<c))!=0;}
const char* status(){return "OEQL + STELLARPHONE + MARTIN OPTICAL BRIDGE READY";}
const char* protocol(){return "OEQL:// + HTTPS FALLBACK + GATEWAY FABRIC";}
const char* opticalProfile(){return "NIR / NIR-II / SWIR / OPTICAL-VOID CALIBRATION PROFILES";}
}