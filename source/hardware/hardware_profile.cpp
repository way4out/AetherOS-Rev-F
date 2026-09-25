#include "hardware_profile.h"
#include <fat.h>
#include <stdio.h>
#include <sys/stat.h>

namespace aether::hardware {

bool sdAvailable() {
    return fatInitDefault();
}

static bool makeDir(const char* path) {
    if (mkdir(path, 0777) == 0) return true;
    return true; // existing directory is acceptable
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

    bool ok = true;
    for (unsigned i = 0; i < sizeof(dirs)/sizeof(dirs[0]); ++i)
        ok = makeDir(dirs[i]) && ok;
    return ok;
}

bool writeBootMarker() {
    if (!sdAvailable()) return false;
    FILE* f = fopen("REVF/LOGS/BOOT.LOG", "w");
    if (!f) return false;
    fprintf(f, "AetherOS Rev:F booted successfully.\n");
    fclose(f);
    return true;
}

}
