#include "aether_core.h"
#include "../benchmark/benchmark.h"
#include "../hardware/hardware_profile.h"
#include "../ui/aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"
#include "../audio/aether_audio.h"
#include "../network/network_fabric.h"
#include "../radio/radio_gateway.h"
#include "../network/gateway_session.h"
#include "../network/gateway_security.h"
namespace { aether::quantum::Simulator q; }
namespace aether {
void init(SystemState&s){s={false,false,false,false,false,false,false,false,0,0,0,false,false};videoSetMode(MODE_0_2D);videoSetModeSub(MODE_0_2D);vramSetBankA(VRAM_A_MAIN_BG);vramSetBankC(VRAM_C_SUB_BG);consoleDemoInit();s.touchReady=true;s.sdReady=hardware::sdAvailable();if(s.sdReady)s.sdWriteReady=hardware::ensureDirectories()&&hardware::writeBootMarker();benchmark::runQuick(s.benchmarkComplete);quantum::init(q);engine::init();network::init();radio::init();session::init();security::init();s.quantumReady=true;s.audioReady=audio::init();s.networkReady=network::status(network::LINK_WIFI).available;s.gatewayConfigured=radio::configured();s.projectSaved=engine::projectExists();ui::init();}
void update(SystemState&s){scanKeys();s.frame++;u16 d=keysDown();if(d&KEY_START){s.safeMode=!s.safeMode;s.screen=0;}if(d&KEY_B)s.screen=0;if(s.screen==0){if(d&KEY_LEFT)s.selectedModule=(s.selectedModule+7)%8;if(d&KEY_RIGHT)s.selectedModule=(s.selectedModule+1)%8;if(d&KEY_UP)s.selectedModule=(s.selectedModule+7)%8;if(d&KEY_DOWN)s.selectedModule=(s.selectedModule+1)%8;if(d&KEY_A)s.screen=s.selectedModule+1;touchPosition t;touchRead(&t);if((d&KEY_TOUCH)&&t.px<256&&t.py<192){s.selectedModule=(t.py/24)%8;s.screen=s.selectedModule+1;}}else{if(d&KEY_A){if(s.screen==2)quantum::runBell(q);else if(s.screen==3){audio::tone(440,250);engine::saveProject();}else if(s.screen>=4&&s.screen<=6)engine::tick();else if(s.screen==8)engine::saveProject();s.projectSaved=engine::projectExists();}if(d&KEY_X&&s.screen==2)quantum::runGrover2(q);if(d&KEY_Y&&s.screen==2)quantum::measure(q);if(d&KEY_L&&s.screen==2)quantum::runDeutschJozsa(q);if(d&KEY_R&&s.screen==2)quantum::runQFT2(q);if(d&KEY_SELECT){quantum::reset(q);audio::stop();}if(!s.safeMode)quantum::tick(q);network::tick();}ui::update(s);}
void render(const SystemState&s){ui::render(s);}void shutdown(){audio::stop();consoleClear();}quantum::Simulator& simulator(){return q;}
}