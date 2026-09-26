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
#include "../core/mission_control.h"
#include "../studio/aether_studio.h"
#include "../settings/aether_settings.h"
#include "../theme/aether_theme.h"
#include "../security/aether_security_lab.h"
#include "../core/capacity_engine.h"
#include "../i18n/aether_i18n.h"
#include "../animal/aether_animal.h"
#include <nds.h>

namespace aether::ui {
static PrintConsole topConsole, bottomConsole;
static int bgTop=-1, bgBottom=-1;
static u16* topPixels=nullptr;
static u16* bottomPixels=nullptr;
static void rect(u16* p,int x0,int y0,int x1,int y1,u16 col){ if(x0<0)x0=0;if(y0<0)y0=0;if(x1>255)x1=255;if(y1>191)y1=191; for(int y=y0;y<=y1;y++){u16* r=p+y*256+x0; for(int x=x0;x<=x1;x++) *r=col;}}
static void scenery(u16* p, settings::Theme t, u32 frame){
    u16 sky=ARGB16(1,3,6,13), mid=ARGB16(1,5,12,24), glow=ARGB16(1,0,18,28), land=ARGB16(1,4,12,8), hi=ARGB16(1,0,24,24), dark=ARGB16(1,2,5,9);
    for(int y=0;y<192;y++){ int v=2+(y*8/192); u16 col=ARGB16(1,2,v/2,v); if(t==settings::THEME_OCEAN) col=ARGB16(1,1,5+v/2,10+v); if(t==settings::THEME_FOREST) col=ARGB16(1,1+v/4,5+v/2,4+v/3); if(t==settings::THEME_NIGHT) col=ARGB16(1,1,2,7+v/3); for(int x=0;x<256;x++)p[y*256+x]=col; }
    for(int i=0;i<10;i++){int x=(i*47+17)%248;int y=12+(i*19)%72; p[y*256+x]=hi; if(x+1<256)p[y*256+x+1]=hi;}
    if(t==settings::THEME_OCEAN){ for(int y=126;y<158;y++) for(int x=0;x<256;x++) if(((x+y+(int)(frame/8))&15)<7)p[y*256+x]=glow; }
    else { for(int y=132;y<192;y++) for(int x=0;x<256;x++) if(((x/9+y/5)&3)==0)p[y*256+x]=land; }
    // Keep the scenic pixels intact; the console layer provides the interactive chrome.
    for(int x=8;x<248;x++){p[8*256+x]=hi;p[184*256+x]=dark;} for(int y=8;y<185;y++){p[y*256+8]=hi;p[y*256+247]=dark;}
}
static const char* names[MOD_COUNT]={"CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS","RF LAB","MARAUDER","STUDIO","SYSTEM","ANIMAL","SETTINGS"};
static const char* glyphs[MOD_COUNT]={"[CORE]","[QBIT]","[SND ]","[DSP ]","[LAB ]","[AI  ]","[NET ]","[FILE]","[RF  ]","[RFX ]","[DAW ]","[SYS ]","[BIO ]","[SET ]"};

static void selectTop(){consoleSelect(&topConsole);}
static void selectBottom(){consoleSelect(&bottomConsole);}
static void clearTop(){selectTop();consoleClear();}
static void clearBottom(){selectBottom();consoleClear();}

static void title(const char* t,const SystemState&s){
    clearTop(); selectTop();
    char stamp[40]; settings::timestamp(stamp,sizeof(stamp));
    iprintf("%sAETHEROS REVOLUTION X%s // %s\n","\x1b[36m","\x1b[37m",t);
    iprintf("%s%s%s\n",theme::accent(),theme::sky(),"\x1b[37m");
    iprintf("%s%s%s\n",theme::accent(),theme::ground(),"\x1b[37m");
    iprintf("%s%s  %s%s\n",theme::accent(),theme::icon(),stamp,"\x1b[37m");
    iprintf("\x1b[34m-----------------------------------------------\n");
    iprintf("\x1b[37m%s  %s  |  %s  |  %s\n",s.safeMode?"SAFE":"READY",s.networkReady?"WIFI":"OFFLINE",s.sdReady?"SD":"NO SD",settings::themeName(theme::active()));
}

void init(){
    // Real bitmap backdrops + transparent text UI: graphics stay underneath the controls.
    videoSetMode(MODE_3_2D); videoSetModeSub(MODE_3_2D);
    vramSetBankA(VRAM_A_MAIN_BG); vramSetBankC(VRAM_C_SUB_BG);
    bgTop=bgInit(3,BgType_Bmp16,BgSize_B16_256x256,1,0);
    bgBottom=bgInitSub(3,BgType_Bmp16,BgSize_B16_256x256,1,0);
    topPixels=bgGetGfxPtr(bgTop); bottomPixels=bgGetGfxPtr(bgBottom);
    scenery(topPixels,settings::THEME_AETHER,0); scenery(bottomPixels,settings::THEME_AETHER,0);
    consoleInit(&topConsole,0,BgType_Text4bpp,BgSize_T_256x256,0,0,true,true);
    consoleInit(&bottomConsole,0,BgType_Text4bpp,BgSize_T_256x256,0,0,false,true);
    clearTop(); clearBottom();
}
void update(const SystemState&){}

static void card(int n,const SystemState&s){
    bool active=(n==s.selectedModule); const int col=n&1,row=n>>1;
    iprintf("\x1b[%d;%dH%s[%s] %-9s%s",2+row*2,1+col*16,active?"\x1b[33m>\x1b[47m":"\x1b[37m",active?"*":" ",names[n],active?"\x1b[0m":"\x1b[37m");
}
static void statusRibbon(const SystemState&s){
    auto hr=hil::report(); auto st=studio::state(); auto dg=diag::report();
    iprintf("\x1b[36mCORE %s Q %s AUD %s\n",s.coreTicks?"LIVE":"IDLE",s.quantumReady?"OK":"--",s.audioReady?"OK":"--");
    iprintf("HIL %u%% GW %u/6 HP %u BPM %u\n",hr.score,(unsigned)gate::onlineCount(),dg.score,st.bpm);
}
static void topDesktop(const SystemState&s){
    auto p=settings::current(); scenery(topPixels,theme::active(),s.frame); scenery(bottomPixels,theme::active(),s.frame);
    title("AETHER HOME",s); selectTop();
    iprintf("\x1b[1;1H\x1b[36mAETHEROS1.1+\x1b[37m   UNIVERSAL DSi WORKSTATION\n");
    iprintf("\x1b[4;1H");
    if(p.layout==settings::LAYOUT_MYSPACE){
        iprintf("\x1b[33m AETHER SPACE  \x1b[37m%s\n",p.locationValid?"LOCATION READY":"AETHER DEFAULT");
        iprintf(" %s\n",theme::sky());
        statusRibbon(s);
        iprintf("\n\x1b[36mYOUR MODULES\n");
        for(int i=0;i<MOD_COUNT;i++)card(i,s);
    } else if(p.layout==settings::LAYOUT_FOCUS){
        iprintf("\x1b[33m FOCUS DESK\n\x1b[37m");
        iprintf(" %s\n",names[s.selectedModule]);
        iprintf(" Personal control surface\n\n");
        statusRibbon(s);
        iprintf("\nA OPEN   L/R MODULE\nTOUCH CENTER ACTION\n");
    } else {
        iprintf("\x1b[32m AETHER VALLEY DESKTOP\n\x1b[37m");
        iprintf(" %s\n",theme::ground());
        statusRibbon(s);
        iprintf("\n\x1b[36mMODULE DECK\x1b[37m\n");
        for(int i=0;i<MOD_COUNT;i++)card(i,s);
    }
}
static void bottomDesktop(const SystemState&s){
    clearBottom(); selectBottom(); auto p=settings::current();
    iprintf("\x1b[36mAETHEROS1.1+ CONTROL DECK\x1b[37m\n");
    iprintf("TOUCH A MODULE  •  A OPEN  •  B HOME\n");
    iprintf("PROFILE: YOU\nTHEME: %s\nLAYOUT: %s\n",settings::themeName(theme::active()),settings::layoutName(p.layout));
    iprintf("%s\n",settings::locationLabel());
    iprintf("\n\x1b[36mQUICK CONTROL\n");
    iprintf("A  Open / Enter\nD  Navigate\nTOUCH  Direct\nB  Back\nSTART Safe Mode\n");
    iprintf("\n%s\n",theme::ground());
    iprintf("SD:%s WS:%s BENCH:%s\n",s.sdReady?"OK":"--",s.sdWriteReady?"OK":"--",s.benchmarkComplete?"OK":"RUN");
    iprintf("QBIT > DSP > SOUND > PROJECTS");
}
static void settingLine(u8 i,const char*label,const char*value,bool selected){
    iprintf("%s%s %-10s %s%s\n",selected?"\x1b[33m>":"\x1b[37m",label,value,selected?" *":"","\x1b[37m");
}
static void settingsScreen(const SystemState&s){
    title("PERSONALIZE / SETTINGS",s); selectTop();
    const auto&p=settings::current();
    char lat[20],lon[20];
    snprintf(lat,sizeof(lat),"%s%d.%03d",p.latitudeE3<0?"-":"",(int)(p.latitudeE3<0?(-p.latitudeE3)/1000:p.latitudeE3/1000),(int)(p.latitudeE3<0?(-p.latitudeE3)%1000:p.latitudeE3%1000));
    snprintf(lon,sizeof(lon),"%s%d.%03d",p.longitudeE3<0?"-":"",(int)(p.longitudeE3<0?(-p.longitudeE3)/1000:p.longitudeE3/1000),(int)(p.longitudeE3<0?(-p.longitudeE3)%1000:p.longitudeE3%1000));
    const u8 sel=p.selectedSetting;
    settingLine(0,"THEME",settings::themeName(p.theme),sel==0);
    settingLine(1,"LAYOUT",settings::layoutName(p.layout),sel==1);
    settingLine(2,"ACCENT","0-15",sel==2);
    settingLine(3,"DENSITY",p.density==0?"LOW":p.density==1?"MED":p.density==2?"HIGH":"MAX",sel==3);
    settingLine(4,"ANIMATE",p.animations?"ON":"OFF",sel==4);
    settingLine(5,"SOUNDS",p.sounds?"ON":"OFF",sel==5);
    settingLine(6,"CLOCK",p.clock24?"24H":"12H",sel==6);
    settingLine(7,"TELEMETRY",p.showTelemetry?"ON":"OFF",sel==7);
    settingLine(8,"QUOTES",p.showQuotes?"ON":"OFF",sel==8);
    settingLine(9,"AUTO THEME",p.autoLocationTheme?"ON":"OFF",sel==9);
    settingLine(10,"LOCATION",p.locationValid?"SET":"UNSET",sel==10);
    settingLine(11,"LAT",lat,sel==11);
    settingLine(12,"LON",lon,sel==12);
    settingLine(13,"UTC MIN",p.utcOffsetMinutes>=0?"+":"-",sel==13);
    settingLine(14,"QUBITS","2-8",sel==14);
    settingLine(15,"VOICES","1-16",sel==15);
    settingLine(16,"VISUAL","0-4",sel==16);
    settingLine(17,"SAVE","WRITE CFG",sel==17);
    settingLine(18,"LANG",i18n::languageName(),sel==18);
}
static void settingsBottom(const SystemState&s){
    clearBottom();selectBottom();const auto&p=settings::current();
    iprintf("%sSETTINGS CONTROL%s\n",theme::accent(),"\x1b[37m");
    iprintf("Selected: %u / 18\n\n",p.selectedSetting);
    iprintf("UP/DOWN  Choose\n");
    iprintf("LEFT/RIGHT Change\n");
    iprintf("A         Apply\n");
    iprintf("X         Save\n");
    iprintf("Y         Reset layout\n");
    iprintf("SELECT    Save\n");
    iprintf("B         Home\n\n");
    iprintf("TIME IS LIVE RTC\n");
    iprintf("Location: %s\n",settings::locationLabel());
    iprintf("Theme: %s\n",theme::name());
    iprintf("\n%s",theme::ground());
}
static void actionPanel(int m){
    clearBottom(); selectBottom();
    iprintf("\x1b[36m[%02d] %s\x1b[37m   L/R MODULE\n",m+1,names[m]);
    iprintf("+------------------------------+\n");
    switch(m){
    case MOD_CORE: iprintf("A System tick / refresh\nX Health snapshot\nY Recovery heartbeat\nSELECT Safe reset"); break;
    case MOD_QUANTUM: iprintf("A Bell / trigger\nX Grover search\nY Measure\nL Deutsch-Jozsa\nR QFT\nSELECT Reset"); break;
    case MOD_SOUND: iprintf("A Test tone\nX Performance tone\nY Stop audio\nL/R Pitch step\nSELECT Reset"); break;
    case MOD_DSP: iprintf("A Analyze signal\nX FFT snapshot\nY Recompute\nSELECT Reset"); break;
    case MOD_LAB: iprintf("A Run experiment\nX New sample\nY Recompute\nSELECT Reset"); break;
    case MOD_AI: iprintf("A Generate pattern\nX Classify\nY Regenerate\nSELECT Reset"); break;
    case MOD_NETWORK: iprintf("A Refresh links\nX Gateway/remote view\nY Queue test\nSELECT Reset"); break;
    case MOD_PROJECTS: iprintf("A Save project\nX Fabric view\nY New project\nSELECT Reset"); break;
    case MOD_RF: iprintf("A Capture sample\nX Analyze band\nY Refresh telemetry\nSELECT Reset"); break;
    case MOD_MARAUDER: iprintf("A Sample + analyze\nX Analyze\nY Consent/acknowledge\nL Passive RF\nR Lab Simulation\nSELECT Reset"); break;
    case MOD_STUDIO: iprintf("A Play/trigger\nX Performance hit\nY Stop\nL/R View/step\nSELECT Reset"); break;
    case MOD_SYSTEM: iprintf("A Mission refresh\nX Diagnostics snapshot\nY Recovery heartbeat\nSELECT Safe mode"); break;
    case MOD_ANIMAL: iprintf("A Analyze animal signal\nX Animal > Human\nY Human > Animal\nL/R Species\nSELECT Reset"); break;
    case MOD_SETTINGS: iprintf("A Apply\nX Save config\nY Reset layout\nL/R Choose\nSELECT Save\nLANG %s",i18n::languageName()); break;
    }
    iprintf("\n+------------------------------+\n");
    iprintf("\x1b[33m A\x1b[37m PRIMARY   \x1b[33mX\x1b[37m ALT   \x1b[33mY\x1b[37m SECONDARY\n");
    iprintf("\x1b[36m TOUCH CENTER\x1b[37m = PRIMARY ACTION\n");
    iprintf("B HOME   L/R MODULE   SELECT RESET");
}
static void module(const SystemState&s){
    const int m=s.selectedModule;
    if(m==MOD_SETTINGS){settingsScreen(s);settingsBottom(s);return;}
    title(names[m],s); selectTop();
    switch(m){
    case MOD_CORE: iprintf("SYSTEM FABRIC\nWATCHDOG RECOVERY STORAGE\nFRAME %lu\nCORE %lu  TOUCH %s\nSD %s / WORKSPACE %s", (unsigned long)s.frame,(unsigned long)s.coreTicks,s.touchReady?"READY":"ERROR",s.sdReady?"READY":"ERROR",s.sdWriteReady?"READY":"ERROR"); break;
    case MOD_QUANTUM:{auto&q=simulator();auto gq=gate::status(gate::GATE_QPU);iprintf("QUANTUM CORE\nLOCAL SIM + EXTERNAL QPU\nQUBITS %d SHOTS %d ALG %d\nBELL %s QPU %s\n",q.qubits,q.shots,q.algorithm,q.bellState?"ON":"OFF",gq.online?"ONLINE":"READY");for(int i=0;i<(1<<q.qubits)&&i<8;i++)iprintf("|%d> %3d%% ",i,(int)(q.probability[i]*100));break;}
    case MOD_SOUND: iprintf("AETHER SOUND\nSYNTH FM DRUMS SAMPLER MIXER\nA TEST TONE\nX PERFORMANCE\nAUDIO %s",s.audioReady?"READY":"WAIT");break;
    case MOD_DSP:{auto mtr=dsp::metrics();iprintf("AETHER DSP\nINPUT > FFT128 > FX > OUT\nRMS %u PEAK %u\nBIN %u ENERGY %u\nREAL FFT / PHASE ENGINE",mtr.rms,mtr.peak,mtr.dominantBin,mtr.energy);break;}
    case MOD_LAB:{auto mtr=lab::metrics();iprintf("AETHER LAB\nSCIENCE + PROCEDURAL\nENTROPY %u\nMONTE %u\nAUTOMATA %u\nPHYSICS %u",mtr.entropy,mtr.monteCarlo,mtr.automata,mtr.physics);break;}
    case MOD_AI:{auto r=ai::result();auto ga=gate::status(gate::GATE_AI);iprintf("AETHER AI\nLOCAL MICRO-AI + GATEWAY\nCLASS %u CONF %u%%\nPATTERN %lu\nGATEWAY %s", (unsigned)r.classId,r.confidence,(unsigned long)r.pattern,ga.online?"ONLINE":"READY");break;}
    case MOD_NETWORK: iprintf("NETWORK FABRIC\nWIFI %s\n5G EXTERNAL GATEWAY\nSAT EXTERNAL GATEWAY\nBT EXTERNAL GATEWAY\nSDR EXTERNAL GATEWAY\nQPU EXTERNAL GATEWAY\nAUTHORIZED HARDWARE ONLY",s.networkReady?"LINK":"OFF");break;
    case MOD_PROJECTS: { auto cp=capacity::report(); iprintf("PROJECT SPACE\nAPRJ / CIRCUITS / SAMPLES\nPRESETS / PLUGINS\nCURRENT: %s\nA SAVE  X FABRIC\nCAPACITY %s",s.projectSaved?"SAVED":"NEW",capacity::status());break;}
    case MOD_RF: iprintf("RF LAB\nRECEIVE-ONLY / AUTHORIZED\nSPECTRUM / WATERFALL\nPEAKS / RSSI / BANDWIDTH\nSDR/TINYSA GATEWAY\nSAMPLES %lu",(unsigned long)s.rfSamples);break;
    case MOD_MARAUDER:{auto sr=securitylab::report();iprintf("AETHER MARAUDER / SECURITY LAB\nMODE %s\nCONSENT %s\nTX LOCKED %s\nCRED CAPTURE LOCKED %s\n\nPASSIVE RF: RSSI / CHANNEL / WATERFALL\nAUTHORIZED NET: OWNED/LAB TRAFFIC METADATA\nLAB SIM: SAFE ATTACK-CONCEPT SIMULATION\nGATEWAY HARDEN: PROTOCOL / AUTH / CRC\n\nSAMPLES %lu DEVICES %lu PACKETS %lu\nALERTS %lu LAB RUNS %lu\n\n%s",securitylab::modeName(sr.mode),sr.consent?"YES":"REQUIRED",sr.txLocked?"YES":"NO",sr.credentialCaptureLocked?"YES":"NO",(unsigned long)sr.samples,(unsigned long)sr.devices,(unsigned long)sr.packets,(unsigned long)sr.alerts,(unsigned long)sr.labRuns,securitylab::warning());break;}
    case MOD_STUDIO:{auto st=studio::state();iprintf("AETHER STUDIO\nDAW / TRACKER / PERFORMANCE\nBPM %u STEP %u/16 NOTE %u\nVOICES %u\nQUANTUM -> MUSIC\nLAB -> AUDIO",st.bpm,st.step,st.note,st.voices);break;}
    case MOD_ANIMAL:{
        auto ar=animal::report();
        iprintf("AETHER UNIVERSAL COMMUNICATION");
        iprintf(" ANIMAL < > HUMAN BRIDGE");
        iprintf(" SPECIES %s",animal::speciesName());
        iprintf(" MODE %s",animal::directionName());
        iprintf(" CONF %u%% SIGNAL %u",ar.confidence/10,ar.signalScore);
        iprintf(" SAMPLES %lu",(unsigned long)ar.samples);
        iprintf(" LOCAL ANALYSIS READY");
        iprintf(" MODEL GATEWAY %s",animal::gatewayRequired()?"REQUIRED":"READY");
        iprintf(" WEB: AETHERLINK / CRITTER CHAT");
        iprintf(" TRANSLATE > VERIFY > SYNTHESIZE");
        break;
    }
    case MOD_SYSTEM:{auto hr=hil::report();auto dg=diag::report();auto mr=mission::report();iprintf("SYSTEM HEALTH / MISSION CONTROL\nREADY SCORE %u%%\nCORE %s  SD %s  CFG %s\nQ %s  AUD %s  DSP %s  LAB %s\nAI %s  NET %s  SEC %s  REC %s\nGATEWAY %s\nHIL %u%%  DIAG %u  FAULTS %u\nGRAPH %u  ONLINE %u\nTX/CREDS/DESTRUCTIVE LOCKED",mr.score,mission::state(mr.boot),mission::state(mr.sd),mission::state(mr.config),mission::state(mr.quantum),mission::state(mr.audio),mission::state(mr.dsp),mission::state(mr.lab),mission::state(mr.ai),mission::state(mr.network),mission::state(mr.security),mission::state(mr.recovery),mission::state(mr.gateway),hr.score,dg.score,dg.faults,dg.graphTicks,dg.gatewayOnline);break;}
    }
    actionPanel(m);
}
void render(const SystemState&s){ if(s.screen==0){topDesktop(s);bottomDesktop(s);} else module(s); }
}
