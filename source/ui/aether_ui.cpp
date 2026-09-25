#include <stdio.h>
#include "aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"
#include <nds.h>

namespace aether::ui {

static const char* names[MOD_COUNT] = {
    "CORE","QUANTUM","SOUND","DSP","LAB","AI",
    "NETWORK","PROJECTS","RF LAB","MARAUDER","STUDIO","SYSTEM"
};

static void color(const char* c){ iprintf("%s", c); }
static void header(const char* title){
    consoleClear();
    color("\x1b[36m");
    iprintf(" AETHEROS REV:F // %s\n", title);
    color("\x1b[37m");
    iprintf(" ================================================\n");
}

void init(){ consoleClear(); }
void update(const SystemState&){}

static void desktop(const SystemState&s){
    header("AETHER DESKTOP / IMMERSIVE");
    color("\x1b[32m");
    iprintf("       .  .       AETHER VALLEY       .  .\n");
    color("\x1b[36m");
    iprintf("   ~~~ /\\ ~~~   sun  wind  water   ~~~ /\\ ~~~\n");
    color("\x1b[33m");
    iprintf("      /  \\       [CORE:%s] [SD:%s]       /  \\ \\n",
            s.coreTicks?"LIVE":"BOOT",s.sdReady?"OK":"ERR");
    color("\x1b[32m");
    iprintf("  ____/____\\______/\\______/\\______/____\\____\n");
    color("\x1b[37m");
    iprintf("Q:%s AUDIO:%s DSP:%lu LAB:%lu AI:%lu RF:%lu\n",
        s.quantumReady?"OK":"ERR",s.audioReady?"OK":"ERR",
        (unsigned long)s.dspTicks,(unsigned long)s.labTicks,(unsigned long)s.aiTicks,
        (unsigned long)s.rfSamples);
    color("\x1b[36m");
    iprintf("\n");
    for(int i=0;i<MOD_COUNT;i++){
        color(i==s.selectedModule?"\x1b[33m":"\x1b[37m");
        iprintf("%c%-10s",i==s.selectedModule?'>':' ',names[i]);
        if((i&3)==3) iprintf("\n");
    }
    color("\x1b[37m");
    iprintf("\nA OPEN  TOUCH SELECT  D-PAD NAV  START SAFE");
}

static void module(const SystemState&s){
    const int m=s.selectedModule;
    header(names[m]);
    switch(m){
    case MOD_CORE:
        iprintf("SYSTEM FABRIC / WATCHDOG / RECOVERY\nFRAME %lu  CORE TICKS %lu\nSD:%s WS:%s TOUCH:%s\n",
            (unsigned long)s.frame,(unsigned long)s.coreTicks,
            s.sdReady?"READY":"ERROR",s.sdWriteReady?"READY":"ERROR",
            s.touchReady?"READY":"ERROR");
        iprintf("\nA DIAGNOSTIC   X SAVE   B DESKTOP");
        break;
    case MOD_QUANTUM:{
        auto&q=simulator();
        iprintf("LOCAL QUBIT SIMULATOR / EXTERNAL QPU READY\nQUBITS %d  SHOTS %d  ALG %d\nBELL %s  LAST %d\n",
            q.qubits,q.shots,q.algorithm,q.bellState?"ON":"OFF",q.lastMeasurement);
        for(int i=0;i<(1<<q.qubits)&&i<8;i++) iprintf("|%d> %3d%%  ",i,(int)(q.probability[i]*100));
        iprintf("\n\nA BELL  X GROVER  Y MEASURE  L DJ  R QFT\nSELECT RESET");
        break;}
    case MOD_SOUND:
        iprintf("AETHER SOUND / SYNTH / SAMPLER / TRACKER\n");
        iprintf("WAVE  FM  DRUM  SAMPLE  GRANULAR  MIXER\n");
        iprintf("REALTIME AUDIO + PROJECT PIPELINE\n\nA PLAY TEST  X SAVE PROJECT  B DESKTOP");
        break;
    case MOD_DSP:
        iprintf("AETHER DSP GRAPH\nINPUT > FFT > FILTER > EQ > FX > OUTPUT\n");
        iprintf("REALTIME PRIORITY / OFFLINE RENDER\nTICKS %lu\n\nA RUN GRAPH  B DESKTOP",(unsigned long)s.dspTicks);
        break;
    case MOD_LAB:
        iprintf("AETHER LAB / SCIENCE WORKBENCH\nFFT  WAVEFORM  MATRICES  PHYSICS\n");
        iprintf("MONTE CARLO  ENTROPY  PROCEDURAL  AUTOMATA\nTICKS %lu\n\nA RUN LAB  B DESKTOP",(unsigned long)s.labTicks);
        break;
    case MOD_AI:
        iprintf("MICRO-AI / LOCAL INFERENCE\nONSET  CLASSIFY  PATTERN  GENERATE\n");
        iprintf("HEAVY MODELS -> AUTHORIZED NETWORK GATEWAY\nTICKS %lu\n\nA GENERATE  B DESKTOP",(unsigned long)s.aiTicks);
        break;
    case MOD_NETWORK:
        iprintf("NETWORK FABRIC / AUTHORIZED LINKS\nDSI WIFI  PHONE HOTSPOT  REMOTE COMPUTE\n");
        iprintf("5G / SATELLITE / BLUETOOTH GATEWAYS: EXTERNAL\n");
        iprintf("OFFLINE QUEUE / LINK TELEMETRY\n\nA TEST LINK  B DESKTOP");
        break;
    case MOD_PROJECTS:
        iprintf("PROJECTS / FILE + EXPERIMENT WORKSPACE\n");
        iprintf("APRJ  CIRCUITS  SAMPLES  PRESETS  LOGS\n");
        iprintf("DEFAULT: %s\n\nA SAVE  X NEW SESSION  SELECT RESET  B DESKTOP",
            s.projectSaved?"SAVED":"NEW");
        break;
    case MOD_RF:
        iprintf("RF LAB / FREQUENCY ANALYSIS\n");
        iprintf("RECEIVE-ONLY / AUTHORIZED MEASUREMENT\n");
        iprintf("SPECTRUM  WATERFALL  PEAKS  RSSI  BANDWIDTH\n");
        iprintf("EXTERNAL SDR / TINYSA / ANALYZER GATEWAY\nSAMPLES %lu\n\nA SAMPLE  B DESKTOP",
            (unsigned long)s.rfSamples);
        break;
    case MOD_MARAUDER:
        iprintf("MARAUDER BOARD GATEWAY\n");
        iprintf("EXTERNAL ESP32 TOOL / AUTHORIZED TESTING\n");
        iprintf("DEVICE DISCOVERY / RSSI / CHANNEL TELEMETRY\n");
        iprintf("NO JAMMING / NO CREDENTIAL CAPTURE\nFRAMES %lu\n\nA SCAN TELEMETRY  B DESKTOP",
            (unsigned long)s.marauderFrames);
        break;
    case MOD_STUDIO:
        iprintf("AETHER STUDIO / DAW\n");
        iprintf("ARRANGE  PATTERNS  PIANO ROLL  TRACKER\n");
        iprintf("AUTOMATION  SAMPLER  MIXER  MASTER  VISUALIZER\n");
        iprintf("QUANTUM->MUSIC / LAB->AUDIO ROUTING\n\nA PERFORMANCE  X SAVE  B DESKTOP");
        break;
    default:
        iprintf("SYSTEM / PERFORMANCE / CREATIVE / LAB\n");
        iprintf("SAFE MODE: %s  BENCH: %s\n",s.safeMode?"ON":"OFF",s.benchmarkComplete?"DONE":"RUN");
        iprintf("SELF-DIAGNOSTICS / PROFILE / RECOVERY\n\nA ADVANCE  B DESKTOP");
        break;
    }
}

void render(const SystemState&s){
    if(s.screen==0) desktop(s);
    else module(s);
}

}
