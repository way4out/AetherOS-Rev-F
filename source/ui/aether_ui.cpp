#include <stdio.h>
#include "aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"
#include "../dsp/aether_dsp.h"
#include "../lab/aether_lab.h"
#include "../ai/aether_ai.h"
#include "../network/gateway_manager.h"
#include "../core/diagnostics.h"
#include "../core/hil.h"
#include "../studio/aether_studio.h"
#include <nds.h>

namespace aether::ui {
static PrintConsole topConsole, bottomConsole;
static const char* names[MOD_COUNT]={"CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS","RF LAB","MARAUDER","STUDIO","SYSTEM"};
static const char* glyphs[MOD_COUNT]={"[CORE]","[QBIT]","[SND ]","[DSP ]","[LAB ]","[AI  ]","[NET ]","[FILE]","[RF  ]","[RFX ]","[DAW ]","[SYS ]"};

static void selectTop(){consoleSelect(&topConsole);}
static void selectBottom(){consoleSelect(&bottomConsole);}
static void clearTop(){selectTop();consoleClear();}
static void clearBottom(){selectBottom();consoleClear();}
static void title(const char* title,const SystemState&s){
    clearTop();
    selectTop();
    iprintf("\x1b[36mAETHEROS REV:F6\x1b[37m  //  %s\n",title);
    iprintf("\x1b[34m-----------------------------------------------\n");
    iprintf("\x1b[37m%02lu  %s  |  %s  |  %s\n",(unsigned long)(s.frame%100),
        s.safeMode?"SAFE MODE":"READY",
        s.networkReady?"WIFI LINK":"OFFLINE",
        s.sdReady?"SD READY":"SD ERROR");
}
void init(){
    videoSetMode(MODE_0_2D); videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG); vramSetBankC(VRAM_C_SUB_BG);
    consoleInit(&topConsole,0,BgType_Text4bpp,BgSize_T_256x256,31,0,true,true);
    consoleInit(&bottomConsole,0,BgType_Text4bpp,BgSize_T_256x256,31,0,false,true);
    clearTop(); clearBottom();
}
void update(const SystemState&){}

static void card(int n,const SystemState&s){
    bool active=(n==s.selectedModule);
    iprintf("%s%s %-10s",active?"\x1b[33m>":"\x1b[37m ",glyphs[n]);
    if((n&1)==1) iprintf("\n");
}
static void topDesktop(const SystemState&s){
    title("AETHER DESKTOP",s);
    selectTop();
    iprintf("\x1b[32m        A E T H E R   V A L L E Y\n");
    iprintf("\x1b[37m     /\\        .        /\\       .\n");
    iprintf("\x1b[32m  __/  \\______/ \\______/  \\_____/\\__\n");
    auto hr=hil::report(); auto st=studio::state(); auto dg=diag::report();
    iprintf("\x1b[36m CORE %s   Q %s   AUDIO %s\n",
        s.coreTicks?"LIVE":"IDLE",s.quantumReady?"READY":"WAIT",s.audioReady?"READY":"WAIT");
    iprintf(" HIL %u%%   GW %u/6   HEALTH %u   BPM %u\n",hr.score,(unsigned)gate::onlineCount(),dg.score,st.bpm);
    iprintf("\x1b[37m\n MODULES\n");
    for(int i=0;i<MOD_COUNT;i++) card(i,s);
    iprintf("\x1b[36m\n A OPEN   D-PAD MOVE   TOUCH OPEN\n");
}
static void bottomDesktop(const SystemState&s){
    clearBottom(); selectBottom();
    iprintf("\x1b[36mAETHER CONTROL\n\x1b[37m----------------\n");
    iprintf("SELECTED\n\x1b[33m%s\x1b[37m\n\n",names[s.selectedModule]);
    iprintf("A  Open module\n");
    iprintf("D  Navigate\n");
    iprintf("TOUCH  Choose\n");
    iprintf("START  Safe mode\n");
    iprintf("SELECT  Reset/stop\n\n");
    iprintf("\x1b[36mSYSTEM GRAPH\n");
    iprintf("QBIT -> DSP -> SOUND\n");
    iprintf("  |      |      |\n");
    iprintf(" LAB <-> AI -> PROJECTS\n\n");
    iprintf("\x1b[37mSD:%s  WS:%s\nBENCH:%s",s.sdReady?"OK":"ERR",s.sdWriteReady?"OK":"ERR",s.benchmarkComplete?"OK":"RUN");
}

static void actionPanel(int m){
    clearBottom(); selectBottom();
    iprintf("\x1b[36m%s / ACTIONS\n\x1b[37m----------------\n",names[m]);
    switch(m){
    case MOD_QUANTUM: iprintf("A  Bell state\nX  Grover\nY  Measure\nL  Deutsch-Jozsa\nR  QFT\nSELECT Reset"); break;
    case MOD_STUDIO: iprintf("A  Play/trigger\nX  Performance hit\nL/R Switch view\nSELECT Stop"); break;
    case MOD_NETWORK: iprintf("A  Link test\nX  Gateway view\nSELECT Reset"); break;
    case MOD_DSP: iprintf("A  Analyze\nX  FFT snapshot\nSELECT Reset"); break;
    case MOD_AI: iprintf("A  Generate\nX  Classify\nSELECT Reset"); break;
    case MOD_LAB: iprintf("A  Run lab tick\nX  New sample\nSELECT Reset"); break;
    case MOD_PROJECTS: iprintf("A  Save project\nX  Open fabric\nSELECT Reset"); break;
    case MOD_RF: iprintf("A  Sample\nX  Analyze band\nSELECT Reset"); break;
    case MOD_MARAUDER: iprintf("A  Authorized scan\nX  Telemetry\nSELECT Reset"); break;
    default: iprintf("A  Open action\nB  Desktop\nSELECT Reset"); break;
    }
    iprintf("\n\x1b[36mL/R  Module   B  Home");
}

static void module(const SystemState&s){
    const int m=s.selectedModule;
    title(names[m],s); selectTop();
    switch(m){
    case MOD_CORE:
        iprintf("SYSTEM FABRIC\nWATCHDOG  RECOVERY  STORAGE\n\nFRAME %lu\nCORE TICKS %lu\nSD %s   WORKSPACE %s\nTOUCH %s\n",
            (unsigned long)s.frame,(unsigned long)s.coreTicks,s.sdReady?"READY":"ERROR",s.sdWriteReady?"READY":"ERROR",s.touchReady?"READY":"ERROR"); break;
    case MOD_QUANTUM:{auto&q=simulator();auto gq=gate::status(gate::GATE_QPU);iprintf("QUANTUM CORE\nLOCAL SIMULATOR + EXTERNAL QPU\n\nQUBITS %d   SHOTS %d\nALG %d   BELL %s\nQPU GATEWAY %s\n",q.qubits,q.shots,q.algorithm,q.bellState?"ON":"OFF",gq.online?"ONLINE":"READY");for(int i=0;i<(1<<q.qubits)&&i<8;i++)iprintf("|%d> %3d%%  ",i,(int)(q.probability[i]*100));break;}
    case MOD_SOUND: iprintf("AETHER SOUND\nSYNTH  FM  DRUMS  SAMPLER  MIXER\n\nA = TEST TONE\nX = PERFORMANCE\nAudio engine: %s\n",s.audioReady?"READY":"WAIT"); break;
    case MOD_DSP:{auto mtr=dsp::metrics();iprintf("AETHER DSP\nINPUT > FFT128 > FX > OUTPUT\n\nRMS %u   PEAK %u\nBIN %u   ENERGY %u\n\nREAL FFT / PHASE ENGINE READY",mtr.rms,mtr.peak,mtr.dominantBin,mtr.energy);break;}
    case MOD_LAB:{auto mtr=lab::metrics();iprintf("AETHER LAB\nSCIENCE + PROCEDURAL WORKBENCH\n\nENTROPY %u\nMONTE %u\nAUTOMATA %u\nPHYSICS %u",mtr.entropy,mtr.monteCarlo,mtr.automata,mtr.physics);break;}
    case MOD_AI:{auto r=ai::result();auto ga=gate::status(gate::GATE_AI);iprintf("AETHER AI\nLOCAL MICRO-AI + REMOTE GATEWAY\n\nCLASS %u\nCONF %u%%\nPATTERN %lu\nGATEWAY %s\nLOCAL FALLBACK ACTIVE",(unsigned)r.classId,r.confidence,(unsigned long)r.pattern,ga.online?"ONLINE":"READY");break;}
    case MOD_NETWORK:{iprintf("NETWORK FABRIC\n\nWIFI       %s\n5G GATEWAY %s\nSATELLITE  %s\nBLUETOOTH  %s\nSDR        %s\nREMOTE QPU %s\n\nExternal links require authorized hardware.",s.networkReady?"LINK":"OFF","GATEWAY", "GATEWAY","GATEWAY","GATEWAY",gate::status(gate::GATE_QPU).online?"ONLINE":"READY");break;}
    case MOD_PROJECTS: iprintf("PROJECT FABRIC\nAPRJ / CIRCUITS / SAMPLES / PRESETS\n\nCURRENT PROJECT\n%s\n\nA SAVE   X FABRIC",s.projectSaved?"SAVED":"NEW"); break;
    case MOD_RF: iprintf("RF LAB\nRECEIVE-ONLY / AUTHORIZED\n\nSPECTRUM  WATERFALL\nPEAKS     RSSI\nBANDWIDTH SDR/TINYSA GATEWAY\n\nSAMPLES %lu",(unsigned long)s.rfSamples);break;
    case MOD_MARAUDER: iprintf("RF GATEWAY\nAUTHORIZED DISCOVERY / TELEMETRY\n\nRSSI  CHANNEL  DEVICE STATE\nNO JAMMING / NO CREDENTIAL CAPTURE\n\nFRAMES %lu",(unsigned long)s.marauderFrames);break;
    case MOD_STUDIO:{auto st=studio::state();iprintf("AETHER STUDIO\nDAW / TRACKER / PERFORMANCE\n\nBPM %u\nSTEP %u / 16\nNOTE %u\nVOICES %u\n\nQUANTUM -> MUSIC\nLAB -> AUDIO",st.bpm,st.step,st.note,st.voices);break;}
    case MOD_SYSTEM:{auto hr=hil::report();auto dg=diag::report();iprintf("SYSTEM HEALTH\n\nHIL PROTOCOL %s\nSTORAGE %s\nHIL SCORE %u%%\nDIAG SCORE %u\nFAULTS %u\nGRAPH %u\nGATEWAYS %u",hr.protocol?"PASS":"FAIL",hr.storage?"PASS":"FAIL",hr.score,dg.score,dg.faults,dg.graphTicks,dg.gatewayOnline);break;}
    }
    actionPanel(m);
}
void render(const SystemState&s){ if(s.screen==0){topDesktop(s);bottomDesktop(s);} else module(s); }
}
