#include "aether_ui.h"
#include <nds.h>
#include <stdio.h>

namespace aether::ui {

static const char* moduleName(int i) {
    switch (i) {
        case 0: return "CORE";
        case 1: return "QUANTUM";
        case 2: return "SOUND";
        case 3: return "DSP";
        case 4: return "LAB";
        case 5: return "AI";
        case 6: return "NETWORK";
        case 7: return "PROJECTS";
        default: return "SYSTEM";
    }
}

void init() {
    consoleClear();
}

void update(const SystemState&) {}

void render(const SystemState& state) {
    consoleClear();

    iprintf(" AETHEROS REV:F  |  CORE 0.2\n");
    iprintf(" --------------------------------\n");
    iprintf(" SYSTEM STATUS\n");
    iprintf("  SD CARD      : %s\n", state.sdReady ? "READY" : "ERROR");
    iprintf("  SD WORKSPACE : %s\n", state.sdWriteReady ? "READY" : "CHECK");
    iprintf("  TOUCH        : %s\n", state.touchReady ? "READY" : "CHECK");
    iprintf("  BENCHMARK    : %s\n", state.benchmarkComplete ? "COMPLETE" : "RUNNING");
    iprintf("  FRAME        : %lu\n", (unsigned long)state.frame);

    iprintf("\n MODULE SELECTOR\n");
    iprintf("  [%s]   <- touch / D-PAD ->\n", moduleName(state.selectedModule));
    iprintf("\n");

    const char* modules[] = {
        "CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS"
    };
    for (int i=0; i<8; ++i) {
        iprintf("%c %-9s", i == state.selectedModule ? '>' : ' ', modules[i]);
        if ((i & 1) == 1) iprintf("\n");
    }

    iprintf("\n A = OPEN MODULE   START = SAFE MODE\n");
    iprintf(" B = SYSTEM SCREEN\n");

    if (state.safeMode)
        iprintf("\n SAFE MODE ACTIVE\n RECOVERY STATE PRESERVED");
}

}
