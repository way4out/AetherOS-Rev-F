#include <stdio.h>
#include "aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"
#include "../dsp/aether_dsp.h"
#include "../lab/aether_lab.h"
#include "../ai/aether_ai.h"
#include <nds.h>

namespace aether::ui {
static PrintConsole topConsole, bottomConsole;
static const char* names[MOD_COUNT]={"CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS","RF LAB","MARAUDER","STUDIO","SYSTEM"};

static void selectTop(){consoleSelect(&topConsole);}
static void selectBottom(){consoleSelect(&bottomConsole);}
static void header(const char* title){
    selectTop(); consoleClear();
    iprintf("\x1b[36m AETHEROS REV:F // %s\n",title);
    iprintf("\x1b[37m ================================================\n");
}
void init(){
    videoSetMode(MODE_0_2D); videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG); vramSetBankC(VRAM_C_SUB_BG);
    consoleInit(&topConsole,0,BgType_Text4bpp,BgSize_T_256x256,31,0,true,true);
    consoleInit(&bottomConsole,0,BgType_Text4bpp,BgSize_T_256x256,31,0,false,true);
    selectTop(); consoleClear(); selectBottom(); consoleClear();
}
void update(const SystemState&){}

static void topDesktop(const SystemState&s){
    header("AETHER DESKTOP / IMMERSIVE");
    selectTop();
    iprintf("\x1b[32m      .  .       AETHER VALLEY       .  .\n");
    iprintf("   ~~~ /\\ ~~~  sun  wind  water  ~~~ /\\ ~~~\n");
    iprintf("\x1b[33m      /  \\       CORE:%s  SD:%s       /  \\ \n",
        s.coreTicks?"LIVE":"BOOT",s.sdReady?"OK":"ERR");
    iprintf("\x1b[32m  ____/____\\______/\\______/\\______/____\\____\n");
    iprintf("\x1b[36m Q:%s AUD:%s NET:%s\n",s.quantumReady?"OK":"ERR",s.audioReady?"OK":"ERR",s.networkReady?"LINK":"OFF");
    iprintf("\x1b[37m DSP:%lu LAB:%lu AI:%lu RF:%lu\n",(unsigned long)s.dspTicks,(unsigned long)s.labTicks,(unsigned long)s.aiTicks,(unsigned long)s.rfSamples);
    iprintf("\n");
    for(int i=0;i<MOD_COUNT;i++){iprintf("%c%-10s",i==s.selectedModule?'>':' ',names[i]);if((i&3)==3)iprintf("\n");}
}
static void bottomDesktop(const SystemState&s){
    selectBottom(); consoleClear();
    iprintf("\x1b[36m AETHER CONTROL SURFACE\n\x1b[37m--------------------------------\n");
    iprintf("MODE: %s   FRAME:%lu\n",s.safeMode?"SAFE":"CREATIVE", (unsigned long)s.frame);
    iprintf("SELECTED: %s\n\n",names[s.selectedModule]);
    iprintf("\x1b[33mA\x1b[37m OPEN   \x1b[33mTOUCH\x1b[37m SELECT\n");
    iprintf("\x1b[33mD-PAD\x1b[37m NAV   \x1b[33mSTART\x1b[37m SAFE\n");
    iprintf("\nCORE GRAPH\n");
    iprintf("QUANTUM > DSP > SOUND\n");
    iprintf("   |       |       |\n");
    iprintf("  LAB <--> AI <--> PROJECTS\n");
    iprintf("\nSD:%s WS:%s BENCH:%s\n",s.sdReady?"OK":"ERR",s.sdWriteReady?"OK":"ERR",s.benchmarkComplete?"OK":"RUN");
}
static void module(const SystemState&s){
    const int m=s.selectedModule; header(names[m]); selectTop();
    switch(m){
    case MOD_CORE: iprintf("SYSTEM FABRIC / WATCHDOG / RECOVERY\nFRAME %lu CORE %lu\nSD:%s WS:%s TOUCH:%s\n", (unsigned long)s.frame,(unsigned long)s.coreTicks,s.sdReady?"READY":"ERROR",s.sdWriteReady?"READY":"ERROR",s.touchReady?"READY":"ERROR"); break;
    case MOD_QUANTUM:{auto&q=simulator();iprintf("LOCAL STATE-VECTOR QPU SIMULATOR\nQUBITS %d SHOTS %d ALG %d\nBELL:%s LAST:%d\n",q.qubits,q.shots,q.algorithm,q.bellState?"ON":"OFF",q.lastMeasurement);for(int i=0;i<(1<<q.qubits)&&i<8;i++)iprintf("|%d> %3d%% ",i,(int)(q.probability[i]*100));break;}
    case MOD_SOUND: iprintf("AETHER SOUND / SYNTH / SAMPLER / TRACKER\nWAVE FM DRUM SAMPLE GRANULAR MIXER\nA PLAY TEST   X SAVE PROJECT\n");break;
    case MOD_DSP:{auto mtr=dsp::metrics();iprintf("AETHER DSP / LIVE ANALYZER\nINPUT > FFT* > FILTER > EQ > FX > OUTPUT\nRMS:%u PEAK:%u BIN:%u ENERGY:%u\n*LIGHTWEIGHT DSi ANALYSIS CORE",mtr.rms,mtr.peak,mtr.dominantBin,mtr.energy);break;}
    case MOD_LAB:{auto mtr=lab::metrics();iprintf("AETHER LAB / SCIENCE WORKBENCH\nENTROPY:%u MONTE:%u AUTO:%u PHYS:%u\nMATRICES / PROCEDURAL / SIMULATION",mtr.entropy,mtr.monteCarlo,mtr.automata,mtr.physics);break;}
    case MOD_AI:{auto r=ai::result();iprintf("MICRO-AI / LOCAL INFERENCE\nCLASS:%u CONF:%u%%\nPATTERN:%lu\nNETWORK MODELS -> GATEWAY",(unsigned)r.classId,r.confidence,(unsigned long)r.pattern);break;}
    case MOD_NETWORK: iprintf("NETWORK FABRIC / AUTHORIZED LINKS\nDSI WIFI / PHONE HOTSPOT / REMOTE COMPUTE\n5G SATELLITE BLUETOOTH = EXTERNAL GATEWAYS\nOFFLINE QUEUE / LINK TELEMETRY");break;
    case MOD_PROJECTS: iprintf("PROJECT VAULT / COMMON DATA FABRIC\nAPRJ / CIRCUITS / SAMPLES / PRESETS / LOGS\nDEFAULT PROJECT: %s",s.projectSaved?"SAVED":"NEW");break;
    case MOD_RF: iprintf("RF LAB / FREQUENCY ANALYSIS\nRECEIVE-ONLY AUTHORIZED MEASUREMENT\nSPECTRUM / WATERFALL / PEAKS / RSSI / BANDWIDTH\nEXTERNAL SDR / TINYSA GATEWAY\nSAMPLES:%lu",(unsigned long)s.rfSamples);break;
    case MOD_MARAUDER: iprintf("MARAUDER BOARD GATEWAY\nAUTHORIZED ESP32 TELEMETRY / DISCOVERY\nRSSI / CHANNEL / DEVICE STATE\nNO JAMMING / NO CREDENTIAL CAPTURE\nFRAMES:%lu",(unsigned long)s.marauderFrames);break;
    case MOD_STUDIO: iprintf("AETHER STUDIO / DAW\nARRANGE / PATTERNS / PIANO ROLL / TRACKER\nAUTOMATION / SAMPLER / MIXER / MASTER\nQUANTUM -> MUSIC   LAB -> AUDIO");break;
    default: iprintf("SYSTEM / PERFORMANCE / CREATIVE / LAB\nSAFE:%s BENCH:%s\nSELF-DIAGNOSTICS / RECOVERY",s.safeMode?"ON":"OFF",s.benchmarkComplete?"DONE":"RUN");break;
    }
    selectBottom(); consoleClear(); iprintf("\x1b[36mAETHER TOUCH / ACTION PANEL\n\x1b[37m--------------------------------\n");
    if(m==MOD_QUANTUM) iprintf("A BELL  X GROVER  Y MEASURE\nL DJ  R QFT  SELECT RESET");
    else if(m==MOD_STUDIO) iprintf("A PERFORMANCE  X PROJECT SAVE\nL/R = MODE BANKS");
    else if(m==MOD_DSP||m==MOD_LAB||m==MOD_AI) iprintf("A RUN / GENERATE\nSELECT RESET   B DESKTOP");
    else iprintf("A ACTION   B DESKTOP\nTOUCH = SELECT MODULE");
}
void render(const SystemState&s){
    if(s.screen==0){topDesktop(s);bottomDesktop(s);} else module(s);
}
}
