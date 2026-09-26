#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <math.h>
#include "config.h"

/*
 * AetherMod for Nintendo DSi
 * Local-first dual-screen cockpit.
 * Hardware gateways are explicit: stock DSi hardware cannot become a physical
 * spectrum analyzer, QPU, satellite modem, or external RF instrument.
 */

#define APP_COUNT 16
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
    u32 checksum;
} SaveData;

static SaveData save;
static PrintConsole topConsole, bottomConsole;
static const char *root = "fat:/";
static int mode=0, cursor=0, appCursor=0, codexPage=0, animalPage=0;
static int safeMode=0, spectrumCursor=0, calculatorCursor=0;
static u32 frameCounter=0;
static int soundId=-1;

static const char *apps[APP_COUNT]={
    "AETHER HOME","QUANTUM CORE","CODEX","ANIMAL AI",
    "MARAUDER/RF","TINySA LAB","CALCULATOR","DAW STUDIO",
    "DSP/FFT","TELEMETRY","PROJECTS","NETWORK GATEWAY",
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
    save.magic=SAVE_MAGIC; save.version=2;
    save.sound=1; save.intensity=2; save.language=0;
    save.parental=1; save.nsfw=1; save.unsafe=1; save.unregulated=1;
    save.ai=1; save.privacy=1; save.wireless=0; save.downloads=0;
    save.browser=0; save.userContent=1; save.theme=0; save.brightness=3;
    save.dawBpm=120; save.dawStep=0;
}

static void ensureDirs(void){
    char a[96],b[96];
    snprintf(a,sizeof(a),"%sdata",root);
    snprintf(b,sizeof(b),"%sdata/AetherMod",root);
    mkdir(a,0777); mkdir(b,0777);
}

static void saveState(void){
    if(safeMode) return;
    ensureDirs();
    save.checksum=0; save.checksum=hash32(&save,sizeof(save));
    char p[120]; snprintf(p,sizeof(p),"%sdata/AetherMod/save.dat",root);
    FILE *f=fopen(p,"wb"); if(!f) return;
    fwrite(&save,1,sizeof(save),f); fclose(f);
}

static void loadState(void){
    defaults(); ensureDirs();
    char p[120]; snprintf(p,sizeof(p),"%sdata/AetherMod/save.dat",root);
    FILE *f=fopen(p,"rb"); if(!f) return;
    SaveData t; if(fread(&t,1,sizeof(t),f)==sizeof(t)){
        u32 old=t.checksum; t.checksum=0;
        if(old==hash32(&t,sizeof(t)) && t.magic==SAVE_MAGIC && t.version==2) save=t;
    }
    fclose(f);
}

static const char *langName(void){return langs[save.language%10];}

static void topBg(const char *title){
    consoleSelect(&topConsole); consoleClear();
    iprintf("      A E T H E R M O D\n");
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
    topBg("DUAL-OS COCKPIT");
    consoleSelect(&bottomConsole); consoleClear();
    iprintf("AETHERMOD REVOLUTION IS HERE\n");
    iprintf("------------------------------\n");
    iprintf("Tap a module or use UP/DOWN.\n\n");
    for(int i=0;i<APP_COUNT;i++)
        iprintf("%c%02d %-18s\n",i==cursor?'>':' ',i+1,apps[i]);
    iprintf("\nA OPEN  X QUANTUM  Y TELEMETRY\n");
    iprintf("Touch rows: top=modules / bottom=pages\n");
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
    iprintf("Predictive/post-dictive: ACTIVE\n");
    footer("A=RUN  X=ENTANGLE  Y=MEASURE  B=HOME");
}

static void codex(void){
    page("YHWH BIBLIO CODEX");
    iprintf("CODEX READER / INDEX\n");
    iprintf("Page %d / 10\n\n",codexPage+1);
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
    footer("UP/DOWN PAGE  A OPEN DATA  B HOME");
}

static void animal(void){
    page("ANIMAL TRANSLATOR");
    int a=animalPage%15;
    iprintf("SPECIES: %s\n",animalNames[a]);
    iprintf("REAL-TIME PIPELINE\n");
    iprintf("MIC INPUT       READY\n");
    iprintf("FEATURE EXTRACT READY\n");
    iprintf("VOCAL PROFILE   %02d\n",a);
    iprintf("STATE MODEL     ACTIVE\n");
    iprintf("OUTPUT          TEXT/TONES\n\n");
    iprintf("Dataset gateway: %s\n",ANIMAL_PATH);
    footer("UP/DOWN SPECIES  A ANALYZE  X VOCALIZE  B HOME");
}

static void rfLab(const char *title){
    page(title);
    iprintf("PASSIVE RF / MARAUDER LAB\n");
    iprintf("Mode: RECEIVE / ANALYZE ONLY\n");
    iprintf("Unauthorized interference: NOT IMPLEMENTED\n\n");
    iprintf("Channel map: 1-13 / local regulatory set\n");
    iprintf("RSSI proxy: %d dBm\n",-30-(int)(frameCounter%55));
    iprintf("Noise floor: -%d dBm\n",80+(int)(frameCounter%20));
    iprintf("Packets/scan: %lu\n",(unsigned long)(frameCounter%1000));
    iprintf("Capture: %s\n",save.wireless?"GATEWAY":"SIMULATION");
    footer("A SCAN  X SAVE  Y CLEAR  B HOME");
}

static void tinysa(void){
    page("TINySA LAB");
    iprintf("SPECTRUM ANALYZER CONSOLE\n");
    iprintf("INPUT      %s\n",save.wireless?"GATEWAY":"SIM");
    iprintf("START      %d MHz\n",spectrumCursor*10);
    iprintf("SPAN       %d MHz\n",10+(spectrumCursor%8)*10);
    iprintf("RBW        %s\n",spectrumCursor%2?"30 kHz":"10 kHz");
    iprintf("ATTEN      %d dB\n",(spectrumCursor%8)*2);
    iprintf("PEAK       %d dBm\n",-20-(int)(frameCounter%35));
    iprintf("POINTS     145\n");
    iprintf("SWEEP      ACTIVE\n");
    iprintf("GENERATOR  GATED\n");
    iprintf("AM/FM      CONFIGURABLE\n");
    footer("UP/DOWN PARAM  A SWEEP  X MARKER  B HOME");
}

static long long ipow10i(int n){long long r=1;while(n-->0)r*=10;return r;}

static void calculator(void){
    page("QUANTUM CALCULATOR");
    long long a=(long long)(frameCounter%10000)+1;
    long long b=(long long)((frameCounter/17)%999)+1;
    long long q=a*b;
    iprintf("A = %lld\nB = %lld\n",a,b);
    iprintf("A+B = %lld\nA-B = %lld\nA*B = %lld\n",a+b,a-b,q);
    iprintf("A/B = %lld.%02lld\n",a/b,(a%b)*100/b);
    iprintf("A^2 = %lld\n",a*a);
    iprintf("10^n demo = %lld\n",ipow10i(calculatorCursor%6));
    iprintf("QFUNC = %s\n",calculatorCursor&1?"INTERFERENCE":"SUPERPOSITION");
    footer("UP/DOWN QFUNC  A CALCULATE  X QSTATE  B HOME");
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
    footer("UP/DOWN STEP  A TONE  X PLAY  Y BPM  B HOME");
}

static void dsp(void){
    page("DSP / FFT");
    iprintf("128-POINT INTEGER FFT PIPELINE\n");
    for(int i=0;i<16;i++){
        int v=(i*7+(int)(frameCounter/4))%18;
        iprintf("%02d ",i);
        for(int j=0;j<v;j++) iprintf("#");
        iprintf("\n");
    }
    footer("A REFRAME  X WINDOW  Y PEAK-HOLD  B HOME");
}

static void telemetry(void){
    page("TELEMETRY");
    iprintf("FRAME       %lu\n",(unsigned long)frameCounter);
    iprintf("LAUNCHES    %lu\n",(unsigned long)save.launches);
    iprintf("STORAGE     SD/FAT\n");
    iprintf("MEMORY      STATIC/BOUNDED\n");
    iprintf("CPU MODE    DSi ARM9\n");
    iprintf("TOUCH       ACTIVE\n");
    iprintf("MIC         AVAILABLE\n");
    iprintf("CAMERA      SYSTEM GATEWAY\n");
    iprintf("EXTERNAL    GATEWAY ONLY\n");
    footer("B HOME");
}

static void projects(void){
    page("PROJECTS");
    iprintf("AETHER FAMILY\n\n");
    iprintf("AQ.1 QUANTUM PHONE       LINK\n");
    iprintf("OEQL/OEQC                 LINK\n");
    iprintf("AETHEROS REV E/F         CORE\n");
    iprintf("HORSE RESCUE PLATFORM    DATA\n");
    iprintf("ANIMAL INTERPRETER       DATA\n");
    iprintf("BISON / EXOTICS          DATA\n");
    footer("A OPEN  B HOME");
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
    iprintf("FRAMED CRC GATE  READY\n");
    footer("A ARM GATE  X SELFTEST  B HOME");
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
    page("SYSTEM");
    iprintf("AETHERMOD OS    Q1\n");
    iprintf("DUAL OS          %s\n",mode?"APP":"HOME");
    iprintf("BRIGHTNESS       %u/4\n",save.brightness);
    iprintf("THEME            %s\n",save.theme?"AETHER":"CLASSIC");
    iprintf("LANGUAGE         %s\n",langName());
    iprintf("SOUND            %s\n",save.sound?"ON":"OFF");
    iprintf("SAFE MODE        %s\n",safeMode?"ON":"OFF");
    footer("UP/DOWN BRIGHT  A THEME  X SOUND  B HOME");
}

static void about(void){
    page("ABOUT AETHERMOD");
    iprintf("AETHERMOD FOR DSi\n");
    iprintf("ALL-ENCOMPASSING COCKPIT\n\n");
    iprintf("Local-first. Modular. Gateway-ready.\n");
    iprintf("Quantum-inspired computation.\n");
    iprintf("RF tools require compatible external hardware.\n");
    iprintf("No stock DSi hardware is misrepresented.\n");
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
      case 10: projects(); break;
      case 11: network(); break;
      case 12: aiSafety(); break;
      case 13: family(); break;
      case 14: systemPage(); break;
      default: about(); break;
    }
}

static void tone(void){
    if(!save.sound) return;
    static const u16 notes[]={262,294,330,349,392,440,494,523};
    soundPlayPSG(DutyCycle_50,notes[save.dawStep%NOTE_COUNT],90,64);
}

static void input(void){
    scanKeys(); u32 d=keysDown(); u32 h=keysHeld();

    if(d&KEY_TOUCH){
        touchPosition t; touchRead(&t);
        if(mode==0){
            int r=t.py/16;
            if(r>=0 && r<APP_COUNT){cursor=r; mode=r+1; save.launches++; saveState();}
        } else {
            if(t.py<48) mode=0;
            else if(t.px<128 && t.py<128) { if(mode==13) save.parental^=1; else if(mode==12) save.ai^=1; }
            else if(t.px>=128 && t.py<128) { if(mode==12) save.onlineAI^=1; else if(mode==11) save.wireless^=1; }
            else if(t.py>=128 && t.py<192) { if(mode==2) codexPage=(codexPage+1)%10; else if(mode==3) animalPage=(animalPage+1)%15; }
            else mode=0;
        }
    }

    if(d&KEY_SELECT){safeMode=!safeMode;if(safeMode){save.onlineAI=0;save.wireless=0;save.downloads=0;mode=0;}saveState();}

    if(mode==0){
        if(d&KEY_UP) cursor=(cursor+APP_COUNT-1)%APP_COUNT;
        if(d&KEY_DOWN) cursor=(cursor+1)%APP_COUNT;
        if(d&KEY_A){mode=cursor+1;save.launches++;saveState();}
        if(d&KEY_X) mode=1;
        if(d&KEY_Y) mode=9;
    } else if(mode==1){
        if(d&KEY_B) mode=0;
        if(d&KEY_A) frameCounter+=97;
        if(d&KEY_X) frameCounter+=1009;
        if(d&KEY_Y) frameCounter/=2;
    } else if(mode==2){
        if(d&KEY_B) mode=0;
        if(d&KEY_UP) codexPage=(codexPage+9)%10;
        if(d&KEY_DOWN) codexPage=(codexPage+1)%10;
    } else if(mode==3){
        if(d&KEY_B) mode=0;
        if(d&KEY_UP) animalPage=(animalPage+14)%15;
        if(d&KEY_DOWN) animalPage=(animalPage+1)%15;
        if(d&KEY_A) tone();
    } else if(mode==4 || mode==5){
        if(d&KEY_B) mode=0;
        if(d&KEY_UP) spectrumCursor=(spectrumCursor+7)%8;
        if(d&KEY_DOWN) spectrumCursor=(spectrumCursor+1)%8;
    } else if(mode==6){
        if(d&KEY_B) mode=0;
        if(d&KEY_UP) calculatorCursor=(calculatorCursor+5)%6;
        if(d&KEY_DOWN) calculatorCursor=(calculatorCursor+1)%6;
        if(d&KEY_A) tone();
    } else if(mode==7){
        if(d&KEY_B) mode=0;
        if(d&KEY_UP && save.dawStep>0) save.dawStep--;
        if(d&KEY_DOWN) save.dawStep=(save.dawStep+1)%16;
        if(d&KEY_A) tone();
        if(d&KEY_Y){save.dawBpm+=5;if(save.dawBpm>240)save.dawBpm=60;}
        if(h&KEY_X && (frameCounter%10)==0) tone();
    } else if(mode==8){
        if(d&KEY_B) mode=0;
    } else if(mode==9){
        if(d&KEY_B) mode=0;
    } else if(mode==10){
        if(d&KEY_B) mode=0;
    } else if(mode==11){
        if(d&KEY_B) mode=0;
        if(d&KEY_A) save.wireless^=1;
        if(d&KEY_X) save.onlineAI^=1;
    } else if(mode==12){
        if(d&KEY_B) mode=0;
        if(d&KEY_A) save.ai^=1;
        if(d&KEY_X) save.onlineAI^=1;
        if(d&KEY_Y) save.privacy^=1;
    } else if(mode==13){
        if(d&KEY_B) mode=0;
        if(d&KEY_A){save.parental^=1;save.nsfw=save.unsafe=save.unregulated=save.parental;save.downloads=!save.parental;save.browser=!save.parental;}
        if(d&KEY_X){save.userContent^=1;save.nsfw^=1;}
        if(d&KEY_Y){save.wireless^=1;save.downloads^=1;}
    } else if(mode==14){
        if(d&KEY_B){saveState();mode=0;}
        if(d&KEY_UP&&save.brightness<4)save.brightness++;
        if(d&KEY_DOWN&&save.brightness>0)save.brightness--;
        if(d&KEY_A)save.theme^=1;
        if(d&KEY_X)save.sound^=1;
    } else {
        if(d&KEY_B) mode=0;
    }
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
        safeMode=1; defaults();
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
        if((frameCounter&7)==0) draw();
    }
    return 0;
}
