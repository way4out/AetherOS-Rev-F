#include "hardware_profile.h"
#include <fat.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

namespace aether::hardware {

static bool gFatReady = false;

bool sdAvailable() {
    if (gFatReady) return true;
    gFatReady = fatInitDefault();
    return gFatReady;
}

static bool dirExists(const char* path) {
    struct stat st{};
    return stat(path, &st) == 0 && (st.st_mode & S_IFDIR);
}

static bool makeDir(const char* path) {
    if (dirExists(path)) return true;
    if (mkdir(path, 0777) == 0) return true;
    return dirExists(path);
}

bool ensureDirectories() {
    if (!sdAvailable()) return false;

    const char* dirs[] = {
        "REVF", "REVF/CORE", "REVF/QUANTUM", "REVF/SOUND",
        "REVF/DSP", "REVF/LAB", "REVF/AI", "REVF/NETWORK",
        "REVF/PROJECTS", "REVF/SAMPLES", "REVF/PRESETS",
        "REVF/CIRCUITS", "REVF/PLUGINS", "REVF/CACHE",
        "REVF/BENCH", "REVF/LOGS", "REVF/RECOVERY"
    };

    for (unsigned i = 0; i < sizeof(dirs)/sizeof(dirs[0]); ++i)
        if (!makeDir(dirs[i])) return false;

    return true;
}

bool writeBootMarker() {
    if (!sdAvailable()) return false;
    if (!dirExists("REVF") || !dirExists("REVF/LOGS")) return false;

    FILE* f = fopen("REVF/LOGS/BOOT.LOG", "w");
    if (!f) return false;
    fprintf(f, "AetherOS Rev:F booted successfully.\\n");
    fclose(f);
    return true;
}

}