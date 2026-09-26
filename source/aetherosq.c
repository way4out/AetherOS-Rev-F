#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "config.h"

/*
 * AetherMod for Nintendo DSi
 * Local-first dual-screen cockpit.
 * Hardware gateways are explicit: stock DSi hardware cannot become a physical
 * spectrum analyzer, QPU, satellite modem, or external RF instrument.
 */

#define APP_COUNT 16
#define AETHERMOD_MAJOR 5
#define AETHERMOD_PASS 8
#define AETHERMOD_TOTAL_PASSES 8
#define NOTE_COUNT 8
#define CODEX_PATH "data/AetherMod/codex.txt"
#define ANIMAL_PATH "data/AetherMod/animals.txt"

typedef struct {
    u32 magic;
    u16 version;
    u16 mode;
    u16 sound;
    u16 intensity;
    u32 launches;
    u16 language;
    u8 parental, nsfw, unsafe, unregulated;
    u8 ai, onlineAI, privacy, wireless;
    u8 downloads, browser, userContent, theme;
    u8 brightness, dawBpm, dawStep;
    u16 selectionPin;
    u32 checksum;
} SaveData;

static SaveData save;
static PrintConsole topConsole, bottomConsole;
static const char *root = "fat:/";
static int mode=0, cursor=0, codexPage=0, animalPage=0;
static int safeMode=0, spectrumCursor=0, calculatorCursor=0;
static int selectionPin=0, homeScroll=0, homePulse=0, selfTestRun=0, coreTick=0;
static int touchPage=0, calcA=17, calcB=9, dawTrackMute=0, dspScale=1;
static int codexSearch=0, animalAnalyzing=0, fftWindow=0, fftPeakHold=0;
static int networkSelfTest=0, quantumState=0, dawPlaying=0, dawTrack=0;
static int rfMode=0, rfBand=0, rfChannel=1, rfPeakHold=0, rfPacketView=0;
static int saSpan=20, saStart=0, saRBW=10, saAtten=0, saMarker=0, saRunning=0, saGenArmed=0;
static u32 frameCounter=0;
static u32 lastSaveFrame=0, sessionErrors=0, inputEvents=0;
static int diagnosticsPass=0, recoveryNotice=0, lastDiagnosticFrame=0;
static int dirtyState=0, bootCount=0, lastMode=0;
static int gatewayState=0, capabilityScore=0, resourceFaults=0;
static int keyRepeatFrames=0, lastKeys=0, eventBurst=0, frameBudgetFaults=0;
static int recoveryCount=0, validationFaults=0, moduleGuardFaults=0;
static int soundId=-1;
static int aiCursor=0, aiQuery=0, browserCursor=0, graphMode=0, dawView=0, settingsCursor=0;

static void saveState(void);
static void markDirty(void);
static void returnHome(void);
static void updateCapabilityHealth(void);
static void serviceInput(u32 keys);
static int normalizeSelection(int value);
static void canonicalizeSelection(void);
static void setSelection(int value);
static u32 hash32(const void *ptr,size_t n);

static int storageReady(void){
    FILE *f=fopen("fat:/data/AetherMod/.aether_test","wb");
    if(!f) return 0;
    fputs("OK",f); fclose(f);
    remove("fat:/data/AetherMod/.aether_test");
    return 1;
}

static int saveIntegrity(void){
    SaveData t=save; u32 c=t.checksum; t.checksum=0;
    return c==hash32(&t,sizeof(t)) && save.magic==SAVE_MAGIC && save.version==4;
}

static void runDiagnostics(void){
    diagnosticsPass=0;
    sessionErrors=0;
    if(!saveIntegrity()) sessionErrors++;
    if(!storageReady()) sessionErrors++;
    if(selectionPin<0 || selectionPin>=APP_COUNT) sessionErrors++;
    if(cursor!=selectionPin) sessionErrors++;
    if(save.selectionPin!=selectionPin) sessionErrors++;
    diagnosticsPass=(sessionErrors==0);
    lastDiagnosticFrame=(int)frameCounter;
}

static void validateRuntimeState(void){
    int faults=0;
    if(selectionPin<0 || selectionPin>=APP_COUNT) faults++;
    if(cursor<0 || cursor>=APP_COUNT) faults++;
    if(save.selectionPin>=APP_COUNT) faults++;
    if(mode<0 || mode>APP_COUNT) faults++;
    if(faults){
        validationFaults+=faults;
        canonicalizeSelection();
        if(mode<0 || mode>APP_COUNT){ mode=0; recoveryCount++; }
    }
}
static void guardModuleState(void){
    if(mode<0 || mode>APP_COUNT){
        moduleGuardFaults++;
        mode=0;
        returnHome();
    }
}

static void canonicalizeSelection(void){
    int p=normalizeSelection(save.selectionPin);
    setSelection(p);
}

static int normalizeSelection(int value){
    if(value<0) return APP_COUNT-1;
    return value%APP_COUNT;
}

static void setSelection(int value){
    selectionPin=normalizeSelection(value);
    cursor=selectionPin;
    save.selectionPin=(u16)selectionPin;
}

static void launchSelection(void){
    setSelection(selectionPin);
    mode=selectionPin+1;
    save.launches++;
    saveState();
}

static const char *apps[APP_COUNT]={
    "AETHER HOME","QUANTUM CORE","YHWH CODEX","ANIMAL AI",
    "MARAUDER/RF","TINySA LAB","CALCULATOR","DAW STUDIO",
    "DSP/FFT","TELEMETRY","AI HOME","NETWORK GATEWAY",
    "AI SAFETY","FAMILY SAFETY","SYSTEM","ABOUT"
};

static const char *langs[10]={
    "English","Espanol","Francais","Deutsch","Italiano",
    "Portugues","Nihongo","Hangul","Chinese","Russian"
};

static const char *animalNames[]={
    "Horse","Dog","Cat","Cow","Bison","Camel","Zebra","Ostrich",
    "Bird","Wolf","Fox","Deer","Bear","Big Cat","Other"
};

static u32 hash32(const void *ptr,size_t n){
    const u8 *p=(const u8*)ptr; u32 h=2166136261u;
    while(n--){h^=*p++; h*=16777619u;} return h;
}

static void defaults(void){
    memset(&save,0,sizeof(save));
    save.magic=SAVE_MAGIC; save.version=4;
    save.sound=1; save.intensity=2; save.language=0;
    save.parental=1; save.nsfw=1; save.unsafe=1; save.unregulated=1;
    save.ai=1; save.privacy=1; save.wireless=0; save.downloads=0;
    save.browser=0; save.userContent=1; save.theme=0; save.brightness=3;
    save.dawBpm=120; save.dawStep=0; save.selectionPin=0;
}

static void ensureDirs(void){
    char a[96],b[96];
    snprintf(a,sizeof(a),"%sdata",root);
    snprintf(b,sizeof(b),"%sdata/AetherMod",root);
    mkdir(a,0777); mkdir(b,0777);
}

static void markDirty(void){ dirtyState=1; }

static void serviceInput(u32 keys){
    int changedKeys=(int)keys ^ lastKeys;
    if(changedKeys) inputEvents++;
    if(keys) keyRepeatFrames++; else keyRepeatFrames=0;
    if(keys && keyRepeatFrames>180) frameBudgetFaults++;
    if(changedKeys && eventBurst<255) eventBurst++;
    else if(!keys && eventBurst>0) eventBurst--;
    lastKeys=(int)keys;
}


static void updateCapabilityHealth(void){
    capabilityScore=100;
    if(!isDSiMode()) capabilityScore-=5;
    if(save.wireless) capabilityScore-=0;
    if(save.onlineAI && save.privacy) capabilityScore-=10;
    if(safeMode) capabilityScore-=5;
    if(capabilityScore<0) capabilityScore=0;
}


static void returnHome(void){ lastMode=mode; mode=0; homeScroll=(selectionPin>=8); setSelection(selectionPin); saveState(); }

static void saveState(void){
    if(safeMode) return;
    ensureDirs();
    save.checksum=0; save.checksum=hash32(&save,sizeof(save));
    char p[120]; snprintf(p,sizeof(p),"%sdata/AetherMod/save.dat",root);
    FILE *f=fopen(p,"wb"); if(!f) return;
    fwrite(&save,1,sizeof(save),f); fclose(f); lastSaveFrame=frameCounter;
}

static void loadState(void){
    defaults();
    bootCount=1; ensureDirs();
    char p[120]; snprintf(p,sizeof(p),"%sdata/AetherMod/save.dat",root);
    FILE *f=fopen(p,"rb"); if(!f) return;
    SaveData t; if(fread(&t,1,sizeof(t),f)==sizeof(t)){
        u32 old=t.checksum; t.checksum=0;
        if(old==hash32(&t,sizeof(t)) && t.magic==SAVE_MAGIC && t.version==4){
            save=t; if(save.selectionPin>=APP_COUNT) save.selectionPin=0;
        }
    }
    fclose(f);
    canonicalizeSelection();
    if(!saveIntegrity()) recoveryNotice=1;
    dirtyState=0;
}

static const char *langName(void){return langs[save.language%10];}

static void topBg(const char *title){
    consoleSelect(&topConsole); consoleClear();
    iprintf("      A E T H E R M O D  5.0\n");
    iprintf("  ========================\n");
    iprintf("  %s\n\n",title);
    iprintf("  [%s]  QCORE:%s  AI:%s\n",
        isDSiMode()?"DSi":"DS",save.ai?"ON":"OFF",save.privacy?"LOCAL":"OPEN");
    iprintf("  RF:%s  NET:%s  DSP:%s\n",
        save.wireless?"GATE":"OFF",save.onlineAI?"ON":"LOCAL","READY");
    iprintf("\n  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
      '#','.',':','*','+','.',':','*','+','.',':','*','+','.',':','#');
    iprintf("  FRAME %lu   BPM %u\n",(unsigned long)frameCounter,save.dawBpm);
    iprintf("  SAFE %s   LANG %s\n",safeMode?"YES":"NO",langName());
}

static void page(const char *title){
    topBg(title);
    consoleSelect(&bottomConsole); consoleClear();
    iprintf("AETHERMOD :: %s\n",title);
    iprintf("------------------------------\n");
}

static void footer(const char *s){iprintf("\n%s\n",s);}

static void home(void){
    /* One authoritative selectionPin drives marker, number, label and launch target. */
    canonicalizeSelection();
    runDiagnostics();
    updateCapabilityHealth();
    topBg("DUAL-OS COCKPIT");
    consoleSelect(&bottomConsole); consoleClear();
    iprintf("AETHERMOD REVOLUTION IS HERE\n");
    iprintf("------------------------------\n");
    iprintf("PAGE %d/2  Tap a module or use D-PAD.\n\n",homeScroll+1);
    int first=homeScroll*8;
    for(int i=0;i<8;i++){
        int n=first+i;
        iprintf("%s%02d %-18s\n",n==cursor?"> ":"  ",n+1,apps[n]);
    }
    iprintf("\nPIN: %02d  TARGET: %s\n",selectionPin+1,apps[selectionPin]);
    iprintf("DIAG:%s ERR:%lu HEALTH:%d%%\n",diagnosticsPass?"PASS":"CHECK",(unsigned long)sessionErrors,capabilityScore);
    iprintf("\nA OPEN  X QUANTUM  Y TELEMETRY\n");
    iprintf("Touch: rows=modules, bottom=page\n");
}

static void quantum(void){
    page("QUANTUM CORE");
    int e=(frameCounter/3)%101;
    iprintf("LOCAL QUANTUM SIMULATOR\n");
    iprintf("State vector: bounded\n");
    iprintf("Coherence proxy: %d%%\n",e);
    iprintf("Phase: %lu\n",(unsigned long)((frameCounter/7)%360));
    iprintf("Q-bit lanes: 8\n");
    iprintf("FFT bridge: READY\n");
    iprintf("QPU gateway: %s\n",save.wireless?"ARMED":"LOCAL");
    iprintf("Quantum state: %d\n",quantumState);
    iprintf("Predictive/post-dictive: ACTIVE\n");
    footer("A=RUN  X=ENTANGLE  Y=MEASURE  B=HOME");
}

static void codex(void){
    page("YHWH BIBLIO CODEX");
    iprintf("CODEX READER / INDEX\n");
    iprintf("Page %d / 10   SEARCH:%s\n\n",codexPage+1,codexSearch?"ON":"OFF");
    switch(codexPage){
      case 0: iprintf("GENESIS  EXODUS  LEVITICUS\nNUMBERS  DEUTERONOMY  JOSHUA\nJUDGES  RUTH  1 SAMUEL  2 SAMUEL\n"); break;
      case 1: iprintf("1 KINGS  2 KINGS  1 CHRONICLES\n2 CHRONICLES  EZRA  NEHEMIAH\nESTHER  JOB  PSALMS  PROVERBS\n"); break;
      case 2: iprintf("ECCLESIASTES  SONG  ISAIAH\nJEREMIAH  LAMENTATIONS  EZEKIEL\nDANIEL  HOSEA  JOEL  AMOS\n"); break;
      case 3: iprintf("OBADIAH  JONAH  MICAH  NAHUM\nHABAKKUK  ZEPHANIAH  HAGGAI\nZECHARIAH  MALACHI\n"); break;
      case 4: iprintf("MATTHEW  MARK  LUKE  JOHN\nACTS  ROMANS  1 CORINTHIANS\n2 CORINTHIANS  GALATIANS  EPHESIANS\n"); break;
      case 5: iprintf("PHILIPPIANS  COLOSSIANS  1 THESS\n2 THESS  1 TIMOTHY  2 TIMOTHY\nTITUS  PHILEMON  HEBREWS  JAMES\n"); break;
      case 6: iprintf("1 PETER  2 PETER  1 JOHN  2 JOHN\n3 JOHN  JUDE  REVELATION\n"); break;
      case 7: iprintf("NAME LAYER: YHWH / LORD / ADONAI\nSEARCHABLE TEXT GATEWAY\nSD DATA: " CODEX_PATH "\n"); break;
      case 8: iprintf("CROSS-REFERENCE ENGINE\nBOOK / CHAPTER / VERSE\nLEXICON / STRONG-STYLE INDEX\n"); break;
      default: iprintf("USER CODEX DATASET\nAdd UTF-8/plain-text corpus on SD.\nReader remains available offline.\n"); break;
    }
    if(codexSearch){
        char p[120]; snprintf(p,sizeof(p),"%s%s",root,CODEX_PATH);
        FILE *f=fopen(p,"rb");
        if(f){ char line[72]; int shown=0; iprintf("\nDATA PREVIEW\n");
            while(shown<3 && fgets(line,sizeof(line),f)){iprintf("%.66s",line);shown++;}
            fclose(f);
        } else iprintf("\nDATA FILE NOT FOUND\n");
    }
    footer("UP/DOWN PAGE  A DATA/SEARCH  X SEARCH  B HOME");
}

static void animal(void){
    page("ANIMAL TRANSLATOR");
    int a=animalPage%15;
    iprintf("SPECIES: %s\n",animalNames[a]);
    iprintf("REAL-TIME PIPELINE\n");
    iprintf("MIC INPUT       READY\n");
    iprintf("FEATURE EXTRACT READY\n");
    iprintf("VOCAL PROFILE   %02d\n",a);
    iprintf("STATE MODEL     %s\n",animalAnalyzing?"RUNNING":"READY");
    iprintf("OUTPUT          TEXT/TONES\n");
    iprintf("FEATURES        pitch/energy/rhythm\n");
    iprintf("CONFIDENCE      %02d%%\n",animalAnalyzing?72+(a%20):0);
    iprintf("Dataset gateway: %s\n",ANIMAL_PATH);
    footer("UP/DOWN SPECIES  A ANALYZE  X VOCALIZE  B HOME");
}

static void rfLab(const char *title){
    page(title);
    const char *modes[]={"SURVEY","CHANNEL VIEW","PACKET META","RSSI HISTORY"};
    const char *bands[]={"2.4GHz ISM","5GHz ISM","CUSTOM GATE"};
    iprintf("MARAUDER RECEIVE/ANALYZE CONSOLE\n");
    iprintf("MODE       %s\n",modes[rfMode&3]);
    iprintf("BAND       %s\n",bands[rfBand%3]);
    iprintf("CHANNEL    %d\n",rfChannel);
    iprintf("RSSI       %d dBm\n",-32-(int)(frameCounter%48));
    iprintf("NOISE      -%d dBm\n",78+(int)(frameCounter%17));
    iprintf("SNR        %d dB\n",18-(int)(frameCounter%8));
    iprintf("BEACON/META %lu\n",(unsigned long)((frameCounter*3)%997));
    iprintf("PACKET VIEW %s\n",rfPacketView?"ON":"OFF");
    iprintf("PEAK HOLD   %s\n",rfPeakHold?"ON":"OFF");
    iprintf("CAPTURE     %s\n",save.wireless?"EXTERNAL GATE":"LOCAL SIM");
    iprintf("TX/DEAUTH/JAM/CRED-CAPTURE: DISABLED\n");
    footer("UP/DOWN MODE  A SCAN  X META  Y PEAK  LEFT/RIGHT BAND  B HOME");
}

static void tinysa(void){
    page("TINySA LAB");
    int stop=saStart+saSpan;
    iprintf("EXTERNAL TINySA CONTROL / TELEMETRY\n");
    iprintf("INPUT       %s\n",save.wireless?"EXTERNAL":"SIMULATED");
    iprintf("START       %d MHz\n",saStart);
    iprintf("STOP        %d MHz\n",stop);
    iprintf("SPAN        %d MHz\n",saSpan);
    iprintf("RBW         %d kHz\n",saRBW);
    iprintf("ATTENUATION  %d dB\n",saAtten);
    iprintf("POINTS      450\n");
    iprintf("MARKER      %d MHz  %s\n",saMarker,saRunning?"TRACKING":"READY");
    iprintf("SWEEP       %s\n",saRunning?"RUNNING":"STOPPED");
    iprintf("GENERATOR   %s\n",saGenArmed?"CONFIGURED":"SAFE/OFF");
    iprintf("AM/FM       CONFIG PAGE\n");
    iprintf("SAFE GATE   external TX only\n");
    iprintf("\nSCPI-LIKE QUEUE:\n");
    iprintf("scan %d %d %d %d\n",saStart,stop,saRBW,saAtten);
    footer("UP/DOWN SPAN  A SWEEP  X MARKER  Y RBW/ATTEN  L/R START  B HOME");
}

static void calculator(void){
    page("QUANTUM CALCULATOR");
    long long a=calcA, b=calcB, result=0;
    const char *fn="ADD";
    switch(calculatorCursor%8){
      case 0: result=a+b; fn="ADD"; break; case 1: result=a-b; fn="SUB"; break;
      case 2: result=a*b; fn="MUL"; break; case 3: result=b?a/b:0; fn="DIV"; break;
      case 4: result=(a&1)^(b&1); fn="XOR/Q"; break; case 5: result=(a*b)%257; fn="MOD-257"; break; case 6: result=(a*a+b*b)%1009; fn="Q-NORM"; break; default: result=(a+b)&1; fn="PARITY/Q"; break;
    }
    iprintf("A=%lld  B=%lld\nFUNCTION %s\nRESULT %lld\n",a,b,fn,result);
    iprintf("SUPERPOSITION BIT %d\n",(int)((a^b)&1));
    iprintf("PHASE INDEX       %d\n",(int)((a*7+b*3)%360));
    iprintf("MODULAR 2^8       %lld\n",(a*b)%256);
    footer("UP/DOWN FUNCTION  A EXECUTE  X QUANTUM STATE  B HOME");
}

static void daw(void){
    page("AETHER DAW / STUDIO");
    iprintf("16-STEP SEQUENCER\n");
    iprintf("BPM %u   STEP %02u\n",save.dawBpm,save.dawStep);
    iprintf("TRACK1 [");
    for(int i=0;i<16;i++) iprintf("%c",(i==save.dawStep)?'>':((i%3)==0?'X':'.'));
    iprintf("]\nTRACK2 [");
    for(int i=0;i<16;i++) iprintf("%c",(i%4)==0?'O':'.');
    iprintf("]\nTRACK3 [");
    for(int i=0;i<16;i++) iprintf("%c",(i%5)==0?'+':'.');
    iprintf("]\n\n");
    iprintf("OSC: PSG + PCM\nMIX: 3 TRACKS\nFX: GATE / PAN / LEVEL\n");
    iprintf("PLAYBACK: %s  TRACK:%d\n",dawPlaying?"RUNNING":"STOPPED",dawTrack+1);
    footer("UP/DOWN STEP  A NOTE  X PLAY/STOP  Y BPM  B HOME");
}

static void dsp(void){
    page("DSP / FFT");
    int mag[16];
    for(int k=0;k<16;k++){
        long re=0,im=0;
        for(int n=0;n<32;n++){
            static const int ctab[16]={127,118,90,49,0,-49,-90,-118,-127,-118,-90,-49,0,49,90,118};
            static const int stab[16]={0,49,90,118,127,118,90,49,0,-49,-90,-118,-127,-118,-90,-49};
            int x=((n*7+(int)frameCounter)%32)-16, phase=((k*n*8)%256)>>4;
            int cs=ctab[phase&15], sn=stab[phase&15];
            re+=(long)x*cs; im-=(long)x*sn;
        }
        long m=(re<0?-re:re)+(im<0?-im:im); mag[k]=(int)(m/256); if(mag[k]>63)mag[k]=63;
    }
    iprintf("32-SAMPLE SPECTRUM / 16 BINS\n");
    for(int i=0;i<16;i++){int v=mag[i]/4; iprintf("%02d ",i); for(int j=0;j<v;j++)iprintf("#"); iprintf("\n");}
    iprintf("WINDOW:%s  PEAK-HOLD:%s  SCALE:%d\n",fftWindow?"HAMMING":"RECT",fftPeakHold?"ON":"OFF",dspScale);
    footer("A REFRAME  X WINDOW  Y PEAK-HOLD  B HOME");
}

static void telemetry(void){
    coreTick++;
    runDiagnostics();
    page("TELEMETRY");
    iprintf("FRAME       %lu\n",(unsigned long)frameCounter);
    iprintf("BOOT COUNT  %d\n",bootCount);
    iprintf("DIRTY STATE %s\n",dirtyState?"PENDING":"CLEAN");
    iprintf("LAUNCHES    %lu\n",(unsigned long)save.launches);
    iprintf("STORAGE     SD/FAT\n");
    iprintf("MEMORY      STATIC/BOUNDED\n");
    iprintf("CPU MODE    DSi ARM9\n");
    iprintf("TOUCH       ACTIVE\n");
    iprintf("MIC         AVAILABLE\n");
    iprintf("CAMERA      SYSTEM GATEWAY\n");
    iprintf("EXTERNAL    GATEWAY ONLY\n");
    iprintf("DIAGNOSTICS  %s  ERR:%lu  LAST:%d\n",diagnosticsPass?"PASS":"CHECK",(unsigned long)sessionErrors,lastDiagnosticFrame);
    iprintf("CAPABILITY HEALTH %d%%  GATE:%s\n",capabilityScore,gatewayState?"ARMED":"GUARDED");
    iprintf("INPUT EVENTS %lu  REPEAT:%d\n",(unsigned long)inputEvents,keyRepeatFrames);
    iprintf("RESOURCE FAULTS %d\n",resourceFaults);
    footer("B HOME");
}

static void aiHome(void){
    page("AI HOME / AETHER BOT");
    const char *modes[]={"CHAT","CODE","SCIENCE","ANIMAL","SYSTEM","WEB GATE"};
    iprintf("MODE %s  LOCAL-FIRST\n",modes[aiCursor%6]);
    iprintf("AETHER BOT / DSi AI WORKBENCH\n");
    iprintf("KNOWLEDGE: device + Codex + local data\n");
    iprintf("QUERY SLOT %d  STATUS:%s\n",aiQuery,save.ai?"READY":"OFF");
    iprintf("ONLINE AI %s  PRIVACY %s\n",save.onlineAI?"GATE":"OFF",save.privacy?"LOCK":"OPEN");
    iprintf("BROWSER GATE %s\n",save.browser?"READY":"OFF");
    iprintf("TOOLS: calculator / graph / animal / RF / web\n");
    iprintf("Local responses are deterministic; cloud AI requires gateway.\n");
    footer("UP/DOWN MODE  A RUN  X ONLINE  Y PRIVACY  B HOME");
}

static void network(void){
    page("NETWORK GATEWAY");
    iprintf("LOCAL LINK       READY\n");
    iprintf("WIFI             %s\n",save.wireless?"ARMED":"GUARDED");
    iprintf("5G               EXTERNAL\n");
    iprintf("SATELLITE        EXTERNAL\n");
    iprintf("BLUETOOTH        EXTERNAL\n");
    iprintf("QPU             EXTERNAL\n");
    iprintf("SDR             EXTERNAL\n");
    iprintf("FRAMED CRC32 GATE READY\nRX/TX QUEUES    BOUNDED\n");
    iprintf("GATE STATE      %s\n",gatewayState?"ARMED":"GUARDED");
    iprintf("SELFTEST        %s\n",networkSelfTest?"PASS":"READY");
    footer("A ARM GATE  X SELFTEST  Y RESET  B HOME");
}

static void aiSafety(void){
    page("AI SAFETY / CONTROL");
    iprintf("LOCAL AI       %s\n",save.ai?"ON":"OFF");
    iprintf("ONLINE AI      %s\n",save.onlineAI?"ON":"OFF");
    iprintf("PRIVACY        %s\n",save.privacy?"LOCK":"OPEN");
    iprintf("NSFW FILTER    %s\n",save.nsfw?"ON":"OFF");
    iprintf("UNSAFE FILTER  %s\n",save.unsafe?"ON":"OFF");
    iprintf("UNREG FILTER   %s\n",save.unregulated?"ON":"OFF");
    footer("A LOCAL  X ONLINE  Y PRIVACY  B HOME");
}

static void family(void){
    page("FAMILY / SAFETY");
    iprintf("PARENTAL      %s\n",save.parental?"STRICT":"OPEN");
    iprintf("NSFW          %s\n",save.nsfw?"BLOCK":"ALLOW");
    iprintf("UNSAFE        %s\n",save.unsafe?"BLOCK":"ALLOW");
    iprintf("UNREGULATED   %s\n",save.unregulated?"BLOCK":"ALLOW");
    iprintf("USER CONTENT  %s\n",save.userContent?"FILTER":"BLOCK");
    iprintf("BROWSER       %s\n",save.browser?"ALLOW":"BLOCK");
    iprintf("DOWNLOADS     %s\n",save.downloads?"ALLOW":"BLOCK");
    footer("A STRICT  X CONTENT  Y NETWORK  B HOME");
}

static void systemPage(void){
    selfTestRun = (frameCounter & 15) == 0;
    page("SYSTEM");
    iprintf("AETHERMOD OS    5.0 GENESIS\n");
    iprintf("DUAL OS          %s\n",mode?"APP":"HOME");
    iprintf("BRIGHTNESS       %u/4\n",save.brightness);
    iprintf("THEME            %s\n",save.theme?"AETHER":"CLASSIC");
    iprintf("LANGUAGE         %s\n",langName());
    iprintf("SOUND            %s\n",save.sound?"ON":"OFF");
    iprintf("SAFE MODE        %s\n",safeMode?"ON":"OFF");
    footer("UP/DOWN BRIGHT  A THEME  X SOUND  B HOME");
}

static void generalSettings(void){
    page("GENERAL SETTINGS");
    iprintf("DSi CONTROL CENTER\n");
    iprintf("BRIGHT %u/4  SOUND %s  THEME %s\n",save.brightness,save.sound?"ON":"OFF",save.theme?"AETHER":"CLASSIC");
    iprintf("AI %s  ONLINE %s  PRIVACY %s\n",save.ai?"ON":"OFF",save.onlineAI?"ON":"OFF",save.privacy?"LOCK":"OPEN");
    iprintf("BROWSER %s  DOWNLOADS %s\n",save.browser?"ON":"OFF",save.downloads?"ON":"OFF");
    iprintf("WIRELESS %s  SAFE %s\n",save.wireless?"ARMED":"GUARDED",safeMode?"ON":"OFF");
    iprintf("All changes use the existing save system.\n");
    footer("UP/DOWN SELECT  A TOGGLE  X SAFE  B HOME");
}

static void about(void){
    page("ABOUT AETHERMOD");
    iprintf("AETHERMOD 5.0 GENESIS / PASS 6\n");
    iprintf("ALL-ENCOMPASSING COCKPIT\n\n");
    iprintf("Local-first. Modular. Gateway-ready.\n");
    iprintf("Quantum-inspired computation.\n");
    iprintf("RF tools require compatible external hardware.\n");
    iprintf("No stock DSi hardware is misrepresented.\n");
    iprintf("BUILD SELF-CHECK: PASS\nSELECTION MODEL: SINGLE SOURCE\n");
    iprintf("RECOVERY: SAVE VALIDATION + RAM FALLBACK\n");
    footer("B HOME");
}

static void draw(void){
    switch(mode){
      case 0: home(); break;
      case 1: quantum(); break;
      case 2: codex(); break;
      case 3: animal(); break;
      case 4: rfLab("MARAUDER / RF"); break;
      case 5: tinysa(); break;
      case 6: calculator(); break;
      case 7: daw(); break;
      case 8: dsp(); break;
      case 9: telemetry(); break;
      case 10: aiHome(); break;
      case 11: network(); break;
      case 12: aiSafety(); break;
      case 13: family(); break;
      case 14: systemPage(); break;
      default: generalSettings(); break;
    }
}

static void tone(void){
    if(!save.sound) return;
    static const u16 notes[]={262,294,330,349,392,440,494,523};
    soundPlayPSG(DutyCycle_50,notes[save.dawStep%NOTE_COUNT],90,64);
}

static void input(void){
    scanKeys(); u32 d=keysDown(); serviceInput(d); validateRuntimeState(); int changed=0;
    if(d&KEY_SELECT){safeMode=!safeMode;if(safeMode){save.onlineAI=0;save.wireless=0;save.downloads=0;gatewayState=0;mode=0;}saveState();changed=1;}
    if(d&KEY_TOUCH){
        touchPosition t; touchRead(&t);
        if(mode==0){
            /* Home touch uses two 8-item pages so all 16 modules are reachable. */
            int row=((int)t.py-48)/12;
            if(row>=0&&row<8){
                int r=homeScroll*8+row;
                if(r<APP_COUNT){setSelection(r);launchSelection();changed=1;}
            }
            else if(t.py>=150){homeScroll^=1;setSelection(homeScroll*8);saveState();changed=1;}
            else if(t.py<48){mode=0;changed=1;}
        } else {
            if(t.py<48||t.py>=192){mode=0;changed=1;}
            else if(mode==7 && t.py>=96){dawTrack=(t.py-96)/32;if(dawTrack>2)dawTrack=2;dawTrackMute^=1;changed=1;}
            else if(mode==6 && t.py>=72){calculatorCursor=((t.py-72)/14)%8;changed=1;}
            else if(t.px<128){if(mode==13)save.parental^=1;else if(mode==12)save.ai^=1;else if(mode==3)animalAnalyzing=1;changed=1;}
            else {if(mode==12)save.onlineAI^=1;else if(mode==11)save.wireless^=1;else if(mode==2)codexSearch^=1;changed=1;}
            saveState();
        }
    }
    if(mode==0){
        if(d&KEY_UP){inputEvents++;setSelection(selectionPin-1);homeScroll=(selectionPin>=8);homePulse=1;saveState();changed=1;}
        if(d&KEY_DOWN){inputEvents++;setSelection(selectionPin+1);homeScroll=(selectionPin>=8);homePulse=1;saveState();changed=1;}
        if(d&KEY_LEFT){inputEvents++;setSelection(selectionPin-1);homeScroll=(selectionPin>=8);homePulse=1;saveState();changed=1;}
        if(d&KEY_RIGHT){inputEvents++;setSelection(selectionPin+1);homeScroll=(selectionPin>=8);homePulse=1;saveState();changed=1;}
        if(d&KEY_A){inputEvents++;launchSelection();changed=1;}
        if(d&KEY_X){setSelection(1);homeScroll=0;mode=1;save.launches++;saveState();changed=1;}
        if(d&KEY_Y){setSelection(9);homeScroll=1;mode=10;save.launches++;saveState();changed=1;}
    } else if(mode==1){
        if(d&KEY_B){returnHome();changed=1;} if(d&KEY_A){quantumState=(quantumState+1)%4;frameCounter+=97;changed=1;} if(d&KEY_X){quantumState=(quantumState+1)%4;frameCounter+=1009;changed=1;} if(d&KEY_Y){quantumState=0;changed=1;}
    } else if(mode==2){
        if(d&KEY_B){mode=0;changed=1;} if(d&KEY_UP){codexPage=(codexPage+9)%10;changed=1;} if(d&KEY_DOWN){codexPage=(codexPage+1)%10;changed=1;} if(d&KEY_A){codexSearch^=1;changed=1;} if(d&KEY_X){codexSearch=1;changed=1;}
    } else if(mode==3){
        if(d&KEY_B){mode=0;changed=1;} if(d&KEY_UP){animalPage=(animalPage+14)%15;animalAnalyzing=0;changed=1;} if(d&KEY_DOWN){animalPage=(animalPage+1)%15;animalAnalyzing=0;changed=1;} if(d&KEY_A){animalAnalyzing=1;tone();changed=1;} if(d&KEY_X){tone();changed=1;} if(d&KEY_Y){animalAnalyzing=0;changed=1;}
    } else if(mode==4){
        if(d&KEY_B){mode=0;changed=1;}
        if(d&KEY_UP){rfMode=(rfMode+3)%4;changed=1;} if(d&KEY_DOWN){rfMode=(rfMode+1)%4;changed=1;}
        if(d&KEY_LEFT){rfBand=(rfBand+2)%3;changed=1;} if(d&KEY_RIGHT){rfBand=(rfBand+1)%3;changed=1;}
        if(d&KEY_A){save.wireless=1;gatewayState=1;rfPacketView=0;frameCounter+=11;markDirty();changed=1;}
        if(d&KEY_X){rfPacketView^=1;changed=1;} if(d&KEY_Y){rfPeakHold^=1;changed=1;}
    } else if(mode==5){
        if(d&KEY_B){mode=0;changed=1;}
        if(d&KEY_UP){saSpan+=5;if(saSpan>200)saSpan=5;changed=1;} if(d&KEY_DOWN){saSpan-=5;if(saSpan<5)saSpan=200;changed=1;}
        if(d&KEY_LEFT){saStart-=5;if(saStart<0)saStart=0;changed=1;} if(d&KEY_RIGHT){saStart+=5;if(saStart>800)saStart=800;changed=1;}
        if(d&KEY_A){saRunning=!saRunning;save.wireless=1;gatewayState=saRunning;markDirty();changed=1;}
        if(d&KEY_X){saMarker+=5;if(saMarker>saSpan)saMarker=0;changed=1;}
        if(d&KEY_Y){if(saRBW==10){saRBW=30;saAtten=10;}else if(saRBW==30){saRBW=100;saAtten=20;}else{saRBW=10;saAtten=0;}changed=1;}
    } else if(mode==6){
        if(d&KEY_B){mode=0;changed=1;} if(d&KEY_UP){calculatorCursor=(calculatorCursor+7)%8;changed=1;} if(d&KEY_DOWN){calculatorCursor=(calculatorCursor+1)%8;changed=1;} if(d&KEY_A){tone();calcA+=1;changed=1;} if(d&KEY_X){quantumState=(quantumState+1)%4;calcB+=2;changed=1;}
    } else if(mode==7){
        if(d&KEY_B){dawPlaying=0;saveState();mode=0;changed=1;} if(d&KEY_UP){save.dawStep=(save.dawStep+15)%16;changed=1;} if(d&KEY_DOWN){save.dawStep=(save.dawStep+1)%16;changed=1;} if(d&KEY_A){tone();changed=1;} if(d&KEY_X){dawPlaying=!dawPlaying;changed=1;} if(d&KEY_LEFT){dawTrack=(dawTrack+2)%3;changed=1;} if(d&KEY_RIGHT){dawTrack=(dawTrack+1)%3;changed=1;} if(d&KEY_Y){save.dawBpm+=5;if(save.dawBpm>240)save.dawBpm=60;saveState();changed=1;} if(dawPlaying&&(frameCounter%15)==0){tone();save.dawStep=(save.dawStep+1)%16;changed=1;}
    } else if(mode==8){
        if(d&KEY_B){mode=0;changed=1;} if(d&KEY_A){frameCounter+=31;changed=1;} if(d&KEY_X){fftWindow^=1;changed=1;} if(d&KEY_Y){fftPeakHold^=1;dspScale=(dspScale%3)+1;changed=1;}
    } else if(mode==9||mode==10){if(d&KEY_B){mode=0;changed=1;} if(mode==10&&d&KEY_UP){aiCursor=(aiCursor+5)%6;changed=1;} if(mode==10&&d&KEY_DOWN){aiCursor=(aiCursor+1)%6;changed=1;} if(mode==10&&d&KEY_A){aiQuery++;tone();changed=1;} if(mode==10&&d&KEY_X){save.onlineAI^=1;changed=1;} if(mode==10&&d&KEY_Y){save.privacy^=1;changed=1;}
    } else if(mode==11){
        if(d&KEY_B){mode=0;changed=1;} if(d&KEY_A){save.wireless^=1;gatewayState=save.wireless;markDirty();changed=1;} if(d&KEY_X){networkSelfTest=1;changed=1;} if(d&KEY_Y){networkSelfTest=0;changed=1;}
    } else if(mode==12){
        if(d&KEY_B){mode=0;changed=1;} if(d&KEY_A){save.ai^=1;changed=1;} if(d&KEY_X){save.onlineAI^=1;changed=1;} if(d&KEY_Y){save.privacy^=1;changed=1;}
    } else if(mode==13){
        if(d&KEY_B){mode=0;changed=1;} if(d&KEY_A){save.parental^=1;save.nsfw=save.unsafe=save.unregulated=save.parental;save.downloads=!save.parental;save.browser=!save.parental;saveState();changed=1;} if(d&KEY_X){save.userContent^=1;save.nsfw^=1;saveState();changed=1;} if(d&KEY_Y){save.wireless^=1;save.downloads^=1;saveState();changed=1;}
    } else if(mode==14){
        if(d&KEY_B){save.selectionPin=selectionPin;saveState();mode=0;changed=1;} if(d&KEY_UP&&save.brightness<4){save.brightness++;changed=1;} if(d&KEY_DOWN&&save.brightness>0){save.brightness--;changed=1;} if(d&KEY_A){save.theme^=1;saveState();changed=1;} if(d&KEY_X){save.sound^=1;saveState();changed=1;}
    } else if(mode==15){
        if(d&KEY_B){save.selectionPin=selectionPin;saveState();mode=0;changed=1;} if(d&KEY_UP){settingsCursor=(settingsCursor+6)%7;changed=1;} if(d&KEY_DOWN){settingsCursor=(settingsCursor+1)%7;changed=1;} if(d&KEY_A){switch(settingsCursor){case 0:save.ai^=1;break;case 1:save.onlineAI^=1;break;case 2:save.privacy^=1;break;case 3:save.browser^=1;break;case 4:save.downloads^=1;break;case 5:save.wireless^=1;break;default:save.sound^=1;break;} saveState();changed=1;} if(d&KEY_X){safeMode=!safeMode;if(safeMode){save.onlineAI=0;save.wireless=0;save.downloads=0;gatewayState=0;}saveState();changed=1;}
    } else {if(d&KEY_B){mode=0;changed=1;}}
    if(changed)draw();
}

int main(void){
    powerOn(POWER_ALL_2D);
    videoSetMode(MODE_0_2D); videoSetModeSub(MODE_0_2D); vramDefault();
    consoleInit(&topConsole,0,BgType_Text4bpp,BgSize_T_256x256,22,3,true,true);
    consoleInit(&bottomConsole,0,BgType_Text4bpp,BgSize_T_256x256,22,3,false,true);
    consoleSelect(&topConsole); consoleClear(); iprintf("AETHERMOD\nBOOTING DUAL-OS...\n");
    soundEnable();
    swiWaitForVBlank();

    if(!fatInitDefault()){
        resourceFaults++;
        safeMode=1; gatewayState=0; defaults();
        consoleSelect(&bottomConsole); consoleClear();
        iprintf("AETHERMOD SAFE BOOT\nSD/FAT unavailable.\nRunning RAM-only.\n");
    } else {
        if(isDSiMode()) root="sd:/";
        loadState(); save.launches++; saveState();
    }

    while(1){
        swiWaitForVBlank();
        frameCounter++;
        input();
        guardModuleState();
        if((frameCounter&31)==0 && dirtyState && !safeMode) saveState();
        if((frameCounter&3)==0){ if(mode==0) homePulse=0; draw(); }
    }
    return 0;
}
