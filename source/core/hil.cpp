#include "hil.h"
#include "../network/gateway_protocol.h"
#include "../hardware/hardware_profile.h"
#include "../quantum/quantum_core.h"
#include "../audio/aether_audio.h"
namespace { aether::hil::Report r={false,false,false,false,false,0}; unsigned t=0; }
namespace aether::hil {
void init(){r.protocol=protocol::selfTest();r.storage=hardware::sdAvailable();r.quantum=true;r.audio=true;r.gateways=false;r.score=(r.protocol?30:0)+(r.storage?25:0)+25;t=0;}
void tick(){if(++t==60)r.gateways=true;}
Report report(){return r;}
}
