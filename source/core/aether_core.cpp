#include "aether_core.h"
#include "../benchmark/benchmark.h"
#include "../hardware/hardware_profile.h"
#include "../ui/aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"
#include "../audio/aether_audio.h"
#include "../dsp/aether_dsp.h"
#include "../lab/aether_lab.h"
#include "../ai/aether_ai.h"
#include "../network/network_fabric.h"
#include "../network/network_fabric.h"
#include "../radio/radio_gateway.h"
#include "../network/gateway_session.h"
#include "../network/gateway_security.h"
#include "system_graph.h"
#include "recovery.h"
#include "governor.h"

namespace { aether::quantum::Simulator q; bool servicesStarted=false; }

namespace aether {
void init(SystemState&s){
    s={false,false,false,false,false,false,false,false,0,0,0,false,false,0,0,0,0,0,0,0};
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);
    consoleDemoInit();
    consoleClear();
    s.touchReady=true;
    quantum::init(q);
    engine::init();
    graph::init();
    recovery::init();
    governor::init();
    ui::init();
}
static void startDeferredServices(SystemState&s){
    if(servicesStarted) return;
    servicesStarted=true;
    s.sdReady=hardware::sdAvailable();
    if(s.sdReady) s.sdWriteReady=hardware::ensureDirectories()&&hardware::writeBootMarker();
    benchmark::runQuick(s.benchmarkComplete);
    radio::init();
    session::init();
    security::init();
    s.quantumReady=true;
    s.audioReady=audio::init();
    dsp::init(); lab::init(); ai::init();
    s.networkReady=false;
    s.gatewayConfigured=radio::configured();
    s.projectSaved=engine::projectExists();
}
void update(SystemState&s){
    scanKeys(); s.frame++;
    if(s.frame==30) startDeferredServices(s);
    u16 d=keysDown();
    if(d&KEY_START){s.safeMode=!s.safeMode;s.screen=0;}
    if(d&KEY_B)s.screen=0;
    if(s.screen==0){
        if(d&KEY_LEFT)s.selectedModule=(s.selectedModule+MOD_COUNT-1)%MOD_COUNT;
        if(d&KEY_RIGHT)s.selectedModule=(s.selectedModule+1)%MOD_COUNT;
        if(d&KEY_UP)s.selectedModule=(s.selectedModule+MOD_COUNT-1)%MOD_COUNT;
        if(d&KEY_DOWN)s.selectedModule=(s.selectedModule+1)%MOD_COUNT;
        if(d&KEY_A)s.screen=s.selectedModule+1;
        touchPosition t; touchRead(&t);
        if((d&KEY_TOUCH)&&t.px<256&&t.py<192){
            s.selectedModule=(t.py/(192/MOD_COUNT))%MOD_COUNT;
            s.screen=s.selectedModule+1;
        }
    } else {
        if(d&KEY_A){
            switch(s.selectedModule){
                case MOD_QUANTUM: quantum::runBell(q); break;
                case MOD_SOUND: audio::tone(440,250); break;
                case MOD_DSP: ++s.dspTicks; break;
                case MOD_LAB: ++s.labTicks; break;
                case MOD_AI: ++s.aiTicks; break;
                case MOD_NETWORK: network::tick(); break;
                case MOD_PROJECTS: engine::saveProject(); break;
                case MOD_RF: ++s.rfSamples; break;
                case MOD_MARAUDER: ++s.marauderFrames; break;
                case MOD_STUDIO: ++s.studioTicks; break;
                default: ++s.coreTicks; break;
            }
            s.projectSaved=engine::projectExists();
        }
        if(d&KEY_X&&s.selectedModule==MOD_QUANTUM) quantum::runGrover2(q);
        if(d&KEY_Y&&s.selectedModule==MOD_QUANTUM) quantum::measure(q);
        if(d&KEY_L&&s.selectedModule==MOD_QUANTUM) quantum::runDeutschJozsa(q);
        if(d&KEY_R&&s.selectedModule==MOD_QUANTUM) quantum::runQFT2(q);
        if(d&KEY_X&&s.selectedModule==MOD_STUDIO) { audio::tone(660,180); ++s.studioTicks; }
        if(d&KEY_SELECT){quantum::reset(q);audio::stop();}
        if(!s.safeMode) quantum::tick(q);
        dsp::tick(); lab::tick(); ai::tick();
        engine::tick();
        if((s.frame & 63)==0){ dsp::Metrics dm=dsp::metrics(); (void)dm; ai::generate(); }
        if(s.selectedModule==MOD_NETWORK && (s.frame & 127)==0) network::tick();
        graph::tick(); governor::tick(); recovery::heartbeat();
    }
    ui::update(s);
}
void render(const SystemState&s){ui::render(s);}
void shutdown(){audio::stop();consoleClear();}
quantum::Simulator& simulator(){return q;}
}
