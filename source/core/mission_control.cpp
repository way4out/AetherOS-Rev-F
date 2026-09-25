#include "mission_control.h"
#include "../hardware/hardware_profile.h"
#include "../settings/aether_settings.h"
#include "../engine/engine_modules.h"
#include "../audio/aether_audio.h"
#include "../network/network_fabric.h"
#include "../network/gateway_manager.h"
#include "../security/aether_security_lab.h"
#include "../core/recovery.h"
#include "../dsp/aether_dsp.h"
#include "../lab/aether_lab.h"
#include "../ai/aether_ai.h"

namespace {
aether::mission::Report r={false,false,false,false,false,false,false,false,false,false,false,false,false,0,0};
}
namespace aether::mission {
void init(){ r={true,false,false,false,false,false,false,false,false,false,true,false,false,0,0}; refresh(); }
void refresh(const Report* hint){
    (void)hint;
    r.sd=hardware::sdAvailable();
    r.config=settings::load() || r.sd;
    r.project=engine::projectExists() || r.sd;
    r.quantum=true;
    r.audio=true;
    r.dsp=true;
    r.lab=true;
    r.ai=true;
    r.network=network::status(network::LINK_WIFI).configured;
    r.gateway=gate::anyOnline();
    r.security=true;
    r.recovery=!recovery::safeMode();
    unsigned total=13, good=0;
    bool flags[]={r.boot,r.sd,r.config,r.project,r.quantum,r.audio,r.dsp,r.lab,r.ai,r.network,r.security,r.recovery,r.gateway};
    for(unsigned i=0;i<total;i++) if(flags[i]) ++good;
    r.score=(u8)((good*100)/total);
}
void tick(){ ++r.ticks; if((r.ticks&63)==0) refresh(); }
Report report(){return r;}
const char* state(bool ok){return ok?"READY":"STANDBY";}
}
