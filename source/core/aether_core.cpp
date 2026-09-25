#include "aether_core.h"
#include "../benchmark/benchmark.h"
#include "../hardware/hardware_profile.h"
#include "../ui/aether_ui.h"

namespace aether {

void init(SystemState& state) {
    state = {false, false, false, false, false, 0, 0};

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);
    consoleDemoInit();

    state.touchReady = true;
    state.sdReady = hardware::sdAvailable();
    if (state.sdReady) {
        state.sdWriteReady = hardware::ensureDirectories();
        hardware::writeBootMarker();
    }

    benchmark::runQuick(state.benchmarkComplete);
    ui::init();
}

void update(SystemState& state) {
    scanKeys();
    state.frame++;

    const u16 down = keysDown();
    if (down & KEY_START) state.safeMode = true;
    if (down & KEY_LEFT)  state.selectedModule = (state.selectedModule + 7) % 8;
    if (down & KEY_RIGHT) state.selectedModule = (state.selectedModule + 1) % 8;
    if (down & KEY_UP)    state.selectedModule = (state.selectedModule + 6) % 8;
    if (down & KEY_DOWN)  state.selectedModule = (state.selectedModule + 2) % 8;

    touchPosition t;
    touchRead(&t);
    if (t.px < 256 && t.py < 192 && (down & KEY_TOUCH)) {
        state.selectedModule = (t.py / 24) % 8;
    }

    ui::update(state);
}

void render(const SystemState& state) {
    ui::render(state);
}

void shutdown() {
    consoleClear();
}

}
