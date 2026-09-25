#include "aether_ui.h"
#include <nds.h>
#include <stdio.h>
namespace aether::ui {
void init() {}
void update(const SystemState&) {}
void render(const SystemState& state) {
    consoleClear();
    iprintf("AETHEROS REV:F CORE 0.1\n\n");
    iprintf("SYSTEM ONLINE\n");
    iprintf("Frame: %lu\n",(unsigned long)state.frame);
    iprintf("SD: %s\n",state.sdReady?"READY":"CHECK");
    iprintf("TOUCH: %s\n",state.touchReady?"READY":"CHECK");
    iprintf("BENCH: %s\n",state.benchmarkComplete?"DONE":"RUNNING");
    iprintf("\nSTART = SAFE MODE\n");
    iprintf("\nQUANTUM  SOUND  LAB\n");
    iprintf("NETWORK   HARDWARE   PROJECTS\n");
    touchPosition t; touchRead(&t);
    iprintf("\nTouch: %d,%d",t.px,t.py);
    if(state.safeMode) iprintf("\n\nSAFE MODE REQUESTED");
}
}
