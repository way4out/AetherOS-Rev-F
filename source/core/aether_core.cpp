#include "aether_core.h"
#include "../benchmark/benchmark.h"
#include "../hardware/hardware_profile.h"
#include "../ui/aether_ui.h"
namespace aether {
void init(SystemState& state) {
    state = {false,false,false,false,0};
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);
    consoleDemoInit();
    state.touchReady = true;
    state.sdReady = hardware::sdAvailable();
    benchmark::runQuick(state.benchmarkComplete);
    ui::init();
}
void update(SystemState& state) {
    scanKeys();
    state.frame++;
    if (keysDown() & KEY_START) state.safeMode = true;
    ui::update(state);
}
void render(const SystemState& state) { ui::render(state); }
}
