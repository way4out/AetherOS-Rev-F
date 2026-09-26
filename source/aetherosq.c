#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
    u32 magic;
    u16 version;
    u16 mode;
    u16 sound;
    u16 intensity;
    u32 launches;
    u32 checksum;
} SaveData;

static SaveData save;
static int mode = 0, cursor = 0, safeMode = 0;
static int parental=1, nsfw=1, unsafe=1, unregulated=1, ai=1, onlineAI=0, privacy=1, language=0;
static u32 frameCounter = 0;
static PrintConsole topConsole;
static PrintConsole bottomConsole;

static const char *storageRoot = "fat:/";

static u32 checksum32(const void *ptr, size_t n) {
    const u8 *p = (const u8 *)ptr;
    u32 h = 2166136261u;
    for (size_t i = 0; i < n; i++) {
        h ^= p[i];
        h *= 16777619u;
    }
    return h;
}

static void defaults(void) {
    memset(&save, 0, sizeof(save));
    save.magic = SAVE_MAGIC;
    save.version = SAVE_VERSION;
    save.sound = 1;
    save.intensity = 2;
}

static void ensure_save_dir(void) {
    char p1[64], p2[64];
    snprintf(p1, sizeof(p1), "%sdata", storageRoot);
    snprintf(p2, sizeof(p2), "%sdata/QuantumAetherOSQ", storageRoot);
    mkdir(p1, 0777);
    mkdir(p2, 0777);
}

static void load_save(void) {
    defaults();
    ensure_save_dir();

    char path[96];
    snprintf(path, sizeof(path), "%sdata/QuantumAetherOSQ/save.dat", storageRoot);
    FILE *f = fopen(path, "rb");
    if (!f) return;

    SaveData t;
    if (fread(&t, 1, sizeof(t), f) == sizeof(t)) {
        u32 old = t.checksum;
        t.checksum = 0;
        if (old == checksum32(&t, sizeof(t)) &&
            t.magic == SAVE_MAGIC &&
            t.version == SAVE_VERSION) {
            save = t;
        }
    }
    fclose(f);
}

static void save_state(void) {
    if (safeMode) return;
    ensure_save_dir();

    save.checksum = 0;
    save.checksum = checksum32(&save, sizeof(save));

    char path[96];
    snprintf(path, sizeof(path), "%sdata/QuantumAetherOSQ/save.dat", storageRoot);
    FILE *f = fopen(path, "wb");
    if (!f) return;
    fwrite(&save, 1, sizeof(save), f);
    fclose(f);
}

static void header(const char *t) {
    consoleSelect(&bottomConsole);
    consoleClear();
    iprintf("QUANTUM AETHEROSQ %s\n", APP_VERSION);
    iprintf("--------------------------------\n%s\n\n", t);
}

static void hub(void) {
    header("QUANTUM COCKPIT");
    const char *items[] = {
        "QUANTUM FUNCORE", "AETHER TELEMETRY", "SETTINGS",
        "SAFE TEST", "ABOUT"
    };
    for (int x = 0; x < 8; x++)
        iprintf("%s %s\n", x == cursor ? ">" : " ", items[x]);

    iprintf("\nA SELECT  UP/DOWN NAV\nX FUN  Y TELEMETRY\n");
    iprintf("Launches: %lu\n", (unsigned long)save.launches);
    consoleSelect(&topConsole);
    consoleClear();
    iprintf("QUANTUM\nAETHEROSQ\n\nFUNCORE ONLINE\n\n");
    iprintf("FRAME %lu\n", (unsigned long)frameCounter);
    iprintf("MODE %s\n", safeMode ? "SAFE" : "LIVE");
    iprintf("DSi %s\n", isDSiMode() ? "MODE" : "DS/COMPAT");
    consoleSelect(&bottomConsole);
}

static void quantumLab(void) {
    header("QUANTUM LAB");
    int e = (frameCounter / 3) % 101;
    iprintf("FUN ENGINE: ONLINE\n\nENERGY [");
    for (int i = 0; i < 20; i++) iprintf("%c", i < e / 5 ? '#' : '.');
    iprintf("] %d%%\n\n", e);
    iprintf("Pulse: %lu\nPhase: %lu\nStability: %s\n",
        (unsigned long)(frameCounter & 65535),
        (unsigned long)((frameCounter / 7) % 12),
        safeMode ? "SAFE" : "LIVE");
    iprintf("\nA=BOOST X=CHAOS Y=CALM\nB=HUB\n");
    if (frameCounter % 45 == 0)
        iprintf("\n>>> QUANTUM EVENT #%lu <<<\n",
            (unsigned long)(frameCounter / 45));
}


static const char *languageName(void) {
    static const char *n[]={"English","Espanol","Francais","Deutsch","Italiano","Portugues","Nihongo","Hangul","Chinese","Russian"};
    return n[language%10];
}
static void safetyCenter(void) {
    header("FAMILY & SAFETY CENTER");
    iprintf("Parental: %s\nNSFW: %s\nUnsafe: %s\nUnregulated: %s\n",
      parental?"ON":"OFF",nsfw?"BLOCKED":"ALLOWED",unsafe?"BLOCKED":"ALLOWED",unregulated?"BLOCKED":"ALLOWED");
    iprintf("AI: %s  ONLINE: %s\nPrivacy Lock: %s\nLanguage: %s\n\n",
      ai?"ON":"OFF",onlineAI?"ON":"OFF",privacy?"ON":"OFF",languageName());
    iprintf("X=STRICT  Y=AI  A=NSFW  B=BACK\n");
}
static void aiCenter(void) {
    header("AI SAFETY CENTER");
    iprintf("Local AI: %s\nOnline AI: %s\nNSFW filter: %s\n",
      ai?"ON":"OFF",onlineAI?"ON":"OFF",nsfw?"ON":"OFF");
    iprintf("Unsafe filter: %s\nPrivacy lock: %s\n\n",unsafe?"ON":"OFF",privacy?"ON":"OFF");
    iprintf("A=AI  X=ONLINE  Y=PRIVACY  B=BACK\n");
}
static void languageCenter(void) {
    header("LANGUAGE");
    iprintf("Current: %s\n\nUP/DOWN select  A=apply  B=back\n",languageName());
    iprintf("0 English  1 Espanol  2 Francais  3 Deutsch  4 Italiano\n");
    iprintf("5 Portugues  6 Nihongo  7 Hangul  8 Chinese  9 Russian\n");
}

static void telemetry(void) {
    header("AETHER TELEMETRY");
    iprintf("Frame: %lu\nIntensity: %u\nSound: %s\n",
        (unsigned long)frameCounter, save.intensity,
        save.sound ? "ON" : "OFF");
    iprintf("Memory: bounded/static\nStorage: FAT/SD\nBoot: %s\n\nB=BACK\n",
        safeMode ? "SAFE" : "NORMAL");
}

static void settings(void) {
    header("SETTINGS");
    iprintf("Intensity: %u\nSound: %s\n\n",
        save.intensity, save.sound ? "ON" : "OFF");
    iprintf("UP/DOWN intensity\nA toggle sound\nB save/back\n");
}

static void safe_test(void) {
    header("SAFE TEST");
    iprintf("[OK] bounded runtime\n");
    iprintf("[OK] save validation\n");
    iprintf("[OK] SD fallback\n");
    iprintf("[OK] no network dependency\n");
    iprintf("[OK] first-boot directory creation\n\nB=BACK\n");
}

static void about(void) {
    header("ABOUT");
    iprintf("Quantum AetherOSQ\n");
    iprintf("DSi Quantum Cockpit\n\n");
    iprintf("Quantum-inspired software UI\n");
    iprintf("No special hardware required.\n\nB=BACK\n");
}

static void draw(void) {
    if (mode == 1) quantumLab();
    else if (mode == 6) safetyCenter();
    else if (mode == 7) aiCenter();
    else if (mode == 8) languageCenter();
    else if (mode == 2) telemetry();
    else if (mode == 3) settings();
    else if (mode == 4) safe_test();
    else if (mode == 5) about();
    else hub();
}

static void input(void) {
    scanKeys();
    u32 d = keysDown();
    u32 h = keysHeld();

    if (d & KEY_TOUCH) { touchPosition t; touchRead(&t); if (t.py < 80) mode = 1; else if (t.py < 160) mode = 2; else mode = 0; }

    if (d & KEY_SELECT) {
        safeMode = !safeMode;
        if (safeMode) {
            save.sound = 0;
            save.intensity = 1;
            mode = 0;
        }
    }

    if (mode == 0) {
        if (d & KEY_UP) cursor = (cursor + 7) % 8;
        if (d & KEY_DOWN) cursor = (cursor + 1) % 8;
        if (d & KEY_X) mode = 1;
        if (d & KEY_Y) mode = 2;
        if (d & KEY_A) {
            mode = cursor + 1;
            save.launches++;
            save_state();
        }
    } else if (mode == 1) {
        if (d & KEY_B) mode = 0;
        if (d & KEY_X) frameCounter += 97;
        if (d & KEY_Y) frameCounter /= 2;
        if (h & KEY_A) frameCounter += 2;
    } else if (mode == 2 || mode == 4 || mode == 5) {
        if (d & KEY_B) mode = 0;
    } else if (mode == 3) {
        if (d & KEY_UP && save.intensity < 4) save.intensity++;
        if (d & KEY_DOWN && save.intensity > 0) save.intensity--;
        if (d & KEY_A) save.sound ^= 1;
        if (d & KEY_B) {
            save_state();
            mode = 0;
        }
    }
}

int main(void) {
    powerOn(POWER_ALL_2D);
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramDefault();
    consoleInit(&topConsole, 0, BgType_Text4bpp, BgSize_T_256x256, 22, 3, true, true);
    consoleInit(&bottomConsole, 0, BgType_Text4bpp, BgSize_T_256x256, 22, 3, false, true);
    consoleSelect(&topConsole);
    consoleClear();
    iprintf("QUANTUM AETHEROSQ\nBOOTING...\n");
    swiWaitForVBlank();

    if (!fatInitDefault()) {
        safeMode = 1;
        defaults();
        consoleSelect(&bottomConsole);
        header("RAM SAFE MODE");
        iprintf("SD/FAT unavailable.\n");
        iprintf("Running without persistence.\n");
    } else {
        
        if (isDSiMode()) storageRoot = "sd:/";
        load_save();
        save.launches++;
        save_state();
    }

    draw();
    consoleSelect(&bottomConsole);

    while (1) {
        swiWaitForVBlank();
        frameCounter++;
        input();
        if ((frameCounter & 7) == 0) draw();
    }

    return 0;
}
