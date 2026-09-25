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
#include "../settings/aether_settings.h"
#include "../theme/aether_theme.h"
#include <nds.h>

namespace aether::ui {
static PrintConsole topConsole, bottomConsole;
static const char* names[MOD_COUNT]={"CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS","RF LAB","MARAUDER","STUDIO","SYSTEM","SETTINGS"};
static const char* glyphs[MOD_COUNT]={"[CORE]","[QBIT]","[SND ]","[DSP ]","[LAB ]","[AI  ]","[NET ]","[FILE]","[RF  ]","[RFX ]","[DAW ]","[SYS ]","[SET ]"};

static void selectTop(){consoleSelect(&topConsole);}
static void selectBottom(){consoleSelect(&bottomConsole);}
static void clearTop(){selectTop();consoleClear();}
static void clearBottom(){selectBottom();consoleClear();}

static void title(const char* t,const SystemState&s){
    clearTop(); selectTop();
    char stamp[40]; settings::timestamp(stamp,sizeof(stamp));
    iprintf("%sAETHEROS REV7+%s // %s
","[36m","[37m",t);
    iprintf("%s%s%s
",theme::accent(),theme::sky(),"[37m");
    iprintf("%s%s%s
",theme::accent(),theme::ground(),"[37m");
    iprintf("%s%s  %s%s
",theme::accent(),theme::icon(),stamp,"[37m");
    iprintf("[34m-----------------------------------------------
");
    iprintf("[37m%s  %s  |  %s  |  %s
",s.safeMode?"SAFE":"READY",s.networkReady?"WIFI":"OFFLINE",s.sdReady?"SD":"NO SD",settings::themeName(theme::active()));
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
    iprintf("%s%s %-10s",active?"[33m>":"[37m ",glyphs[n]);
    if((n&1)==1) iprintf("
");
}
static void statusRibbon(const SystemState&s){
    auto hr=hil::report(); auto st=studio::state(); auto dg=diag::report();
    iprintf("[36mCORE %s Q %s AUD %s
",s.coreTicks?"LIVE":"IDLE",s.quantumReady?"OK":"--",s.audioReady?"OK":"--");
    iprintf("HIL %u%% GW %u/6 HP %u BPM %u
",hr.score,(unsigned)gate::onlineCount(),dg.score,st.bpm);
}
static void topDesktop(const SystemState&s){
    title("AETHER HOME",s);
    selectTop();
    auto p=settings::current();
    if(p.layout==settings::LAYOUT_MYSPACE){
        iprintf("[33m AETHER SPACE  [37m%s
",p.locationValid?"LOCATION READY":"HOBBIT FALLBACK");
        iprintf(" %s
",theme::sky());
        statusRibbon(s);
        iprintf("
[36mYOUR MODULES
");
        for(int i=0;i<MOD_COUNT;i++)card(i,s);
    } else if(p.layout==settings::LAYOUT_FOCUS){
        iprintf("[33m FOCUS DESK
[37m");
        iprintf(" %s
",names[s.selectedModule]);
        iprintf(" Personal control surface

");
        statusRibbon(s);
        iprintf("
A OPEN   L/R MODULE
TOUCH CENTER ACTION
");
    } else {
        iprintf("[32m AETHER VALLEY DESKTOP
[37m");
        iprintf(" %s
",theme::ground());
        statusRibbon(s);
        iprintf("
 MODULES
");
        for(int i=0;i<MOD_COUNT;i++)card(i,s);
    }
}
static void bottomDesktop(const SystemState&s){
    clearBottom(); selectBottom();
    auto p=settings::current();
    iprintf("%sAETHER SPACE%s
",theme::accent(),"[37m");
    iprintf("PROFILE: YOU
THEME: %s
LAYOUT: %s
",settings::themeName(theme::active()),settings::layoutName(p.layout));
    iprintf("%s
",settings::locationLabel());
    iprintf("
[36mQUICK CONTROL
");
    iprintf("A  Open / Enter
D  Navigate
TOUCH  Direct
B  Back
START Safe Mode
");
    iprintf("
%s
",theme::ground());
    iprintf("SD:%s WS:%s BENCH:%s
",s.sdReady?"OK":"--",s.sdWriteReady?"OK":"--",s.benchmarkComplete?"OK":"RUN");
    iprintf("QBIT > DSP > SOUND > PROJECTS");
}
static void settingLine(u8 i,const char*label,const char*value,bool selected){
    iprintf("%s%s %-10s %s%s
",selected?"[33m>":"[37m",label,value,selected?" *":"","[37m");
}
static void settingsScreen(const SystemState&s){
    title("PERSONALIZE / SETTINGS",s); selectTop();
    const auto&p=settings::current();
    char lat[20],lon[20];
    snprintf(lat,sizeof(lat),"%s%d.%03d",p.latitudeE3<0?"-":"",p.latitudeE3<0?(-p.latitudeE3)/1000:p.latitudeE3/1000,p.latitudeE3<0?(-p.latitudeE3)%1000:p.latitudeE3%1000);
    snprintf(lon,sizeof(lon),"%s%d.%03d",p.longitudeE3<0?"-":"",p.longitudeE3<0?(-p.longitudeE3)/1000:p.longitudeE3/1000,p.longitudeE3<0?(-p.longitudeE3)%1000:p.longitudeE3%1000);
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
}
static void settingsBottom(const SystemState&s){
    clearBottom();selectBottom();const auto&p=settings::current();
    iprintf("%sSETTINGS CONTROL%s
",theme::accent(),"[37m");
    iprintf("Selected: %u / 17

",p.selectedSetting);
    iprintf("UP/DOWN  Choose
");
    iprintf("LEFT/RIGHT Change
");
    iprintf("A         Apply
");
    iprintf("X         Save
");
    iprintf("Y         Reset layout
");
    iprintf("SELECT    Save
");
    iprintf("B         Home

");
    iprintf("TIME IS LIVE RTC
");
    iprintf("Location: %s
",settings::locationLabel());
    iprintf("Theme: %s
",theme::name());
    iprintf("
%s",theme::ground());
}
static void actionPanel(int m){
    clearBottom(); selectBottom();
    iprintf("%s%s / ACTIONS%s
",theme::accent(),names[m],"[37m");
    iprintf("----------------
");
    switch(m){
    case MOD_QUANTUM: iprintf("A Bell  X Grover
Y Measure
L Deutsch-Jozsa
R QFT
SELECT Reset"); break;
    case MOD_STUDIO: iprintf("A Play/trigger
X Performance
L/R View
SELECT Stop"); break;
    case MOD_NETWORK: iprintf("A Link test
X Gateway view
SELECT Reset"); break;
    case MOD_DSP: iprintf("A Analyze
X FFT snapshot
SELECT Reset"); break;
    case MOD_AI: iprintf("A Generate
X Classify
SELECT Reset"); break;
    case MOD_LAB: iprintf("A Run lab tick
X New sample
SELECT Reset"); break;
    case MOD_PROJECTS: iprintf("A Save project
X Fabric
SELECT Reset"); break;
    case MOD_RF: iprintf("A Sample
X Analyze band
SELECT Reset"); break;
    case MOD_MARAUDER: iprintf("A Authorized scan
X Telemetry
SELECT Reset"); break;
    default: iprintf("A Open action
B Home
SELECT Reset"); break;
    }
    iprintf("
%sL/R Module  B Home%s",theme::accent(),"[37m");
}
static void module(const SystemState&s){
    const int m=s.selectedModule;
    if(m==MOD_SETTINGS){settingsScreen(s);settingsBottom(s);return;}
    title(names[m],s); selectTop();
    switch(m){
    case MOD_CORE: iprintf("SYSTEM FABRIC
WATCHDOG RECOVERY STORAGE
FRAME %lu
CORE %lu  TOUCH %s
SD %s / WORKSPACE %s", (unsigned long)s.frame,(unsigned long)s.coreTicks,s.touchReady?"READY":"ERROR",s.sdReady?"READY":"ERROR",s.sdWriteReady?"READY":"ERROR"); break;
    case MOD_QUANTUM:{auto&q=simulator();auto gq=gate::status(gate::GATE_QPU);iprintf("QUANTUM CORE
LOCAL SIM + EXTERNAL QPU
QUBITS %d SHOTS %d ALG %d
BELL %s QPU %s
",q.qubits,q.shots,q.algorithm,q.bellState?"ON":"OFF",gq.online?"ONLINE":"READY");for(int i=0;i<(1<<q.qubits)&&i<8;i++)iprintf("|%d> %3d%% ",i,(int)(q.probability[i]*100));break;}
    case MOD_SOUND: iprintf("AETHER SOUND
SYNTH FM DRUMS SAMPLER MIXER
A TEST TONE
X PERFORMANCE
AUDIO %s",s.audioReady?"READY":"WAIT");break;
    case MOD_DSP:{auto mtr=dsp::metrics();iprintf("AETHER DSP
INPUT > FFT128 > FX > OUT
RMS %u PEAK %u
BIN %u ENERGY %u
REAL FFT / PHASE ENGINE",mtr.rms,mtr.peak,mtr.dominantBin,mtr.energy);break;}
    case MOD_LAB:{auto mtr=lab::metrics();iprintf("AETHER LAB
SCIENCE + PROCEDURAL
ENTROPY %u
MONTE %u
AUTOMATA %u
PHYSICS %u",mtr.entropy,mtr.monteCarlo,mtr.automata,mtr.physics);break;}
    case MOD_AI:{auto r=ai::result();auto ga=gate::status(gate::GATE_AI);iprintf("AETHER AI
LOCAL MICRO-AI + GATEWAY
CLASS %u CONF %u%%
PATTERN %lu
GATEWAY %s", (unsigned)r.classId,r.confidence,(unsigned long)r.pattern,ga.online?"ONLINE":"READY");break;}
    case MOD_NETWORK: iprintf("NETWORK FABRIC
WIFI %s
5G EXTERNAL GATEWAY
SAT EXTERNAL GATEWAY
BT EXTERNAL GATEWAY
SDR EXTERNAL GATEWAY
QPU EXTERNAL GATEWAY
AUTHORIZED HARDWARE ONLY",s.networkReady?"LINK":"OFF");break;
    case MOD_PROJECTS: iprintf("PROJECT SPACE
APRJ / CIRCUITS / SAMPLES
PRESETS / PLUGINS
CURRENT: %s
A SAVE  X FABRIC",s.projectSaved?"SAVED":"NEW");break;
    case MOD_RF: iprintf("RF LAB
RECEIVE-ONLY / AUTHORIZED
SPECTRUM / WATERFALL
PEAKS / RSSI / BANDWIDTH
SDR/TINYSA GATEWAY
SAMPLES %lu",(unsigned long)s.rfSamples);break;
    case MOD_MARAUDER: iprintf("RF GATEWAY
AUTHORIZED DISCOVERY
RSSI / CHANNEL / STATE
NO JAMMING / NO CREDENTIALS
FRAMES %lu",(unsigned long)s.marauderFrames);break;
    case MOD_STUDIO:{auto st=studio::state();iprintf("AETHER STUDIO
DAW / TRACKER / PERFORMANCE
BPM %u STEP %u/16 NOTE %u
VOICES %u
QUANTUM -> MUSIC
LAB -> AUDIO",st.bpm,st.step,st.note,st.voices);break;}
    case MOD_SYSTEM:{auto hr=hil::report();auto dg=diag::report();iprintf("SYSTEM HEALTH
HIL PROTOCOL %s
STORAGE %s
HIL %u%% DIAG %u
FAULTS %u GRAPH %u
GATEWAYS %u",hr.protocol?"PASS":"FAIL",hr.storage?"PASS":"FAIL",hr.score,dg.score,dg.faults,dg.graphTicks,dg.gatewayOnline);break;}
    }
    actionPanel(m);
}
void render(const SystemState&s){ if(s.screen==0){topDesktop(s);bottomDesktop(s);} else module(s); }
}
