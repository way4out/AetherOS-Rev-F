#include "aether_core.h"
#include "../benchmark/benchmark.h"
#include "../hardware/hardware_profile.h"
#include "../ui/aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"

namespace { aether::quantum::Simulator q; }

namespace aether {
void init(SystemState& s){
    s={false,false,false,false,false,false,false,false,0,0,0};
    videoSetMode(MODE_0_2D); videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG); vramSetBankC(VRAM_C_SUB_BG);
    consoleDemoInit();
    s.touchReady=true;
    s.sdReady=hardware::sdAvailable();
    if(s.sdReady){s.sdWriteReady=hardware::ensureDirectories();hardware::writeBootMarker();}
    benchmark::runQuick(s.benchmarkComplete);
    quantum::init(q); engine::init(); s.quantumReady=true; s.projectSaved=engine::projectExists();
    ui::init();
}
void update(SystemState& s){
    scanKeys(); s.frame++; const u16 d=keysDown();
    if(d&KEY_START){s.safeMode=!s.safeMode;s.screen=0;}
    if(d&KEY_B){s.screen=0;}
    if(s.screen==0){
        if(d&KEY_LEFT)s.selectedModule=(s.selectedModule+7)%8;
        if(d&KEY_RIGHT)s.selectedModule=(s.selectedModule+1)%8;
        if(d&KEY_UP)s.selectedModule=(s.selectedModule+7)%8;
        if(d&KEY_DOWN)s.selectedModule=(s.selectedModule+1)%8;
        if(d&KEY_A)s.screen=s.selectedModule+1;
        touchPosition t;touchRead(&t);
        if((d&KEY_TOUCH)&&t.px<256&&t.py<192){s.selectedModule=(t.py/24)%8;s.screen=s.selectedModule+1;}
    }else{
        if(d&KEY_A){
            if(s.screen==2) quantum::runBell(q);
            else if(s.screen==3) engine::saveProject();
            else if(s.screen==4) engine::tick();
            else if(s.screen==5) engine::tick();
            s.projectSaved=engine::projectExists();
        }
        if(d&KEY_SELECT){quantum::reset(q);}
        quantum::tick(q);
    }
    ui::update(s);
}
void render(const SystemState& s){ui::render(s);if(s.screen==2)quantum::tick(q);}
void shutdown(){consoleClear();}
quantum::Simulator& simulator(){return q;}
}