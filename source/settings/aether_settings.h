#pragma once
#include <nds.h>

namespace aether::settings {
enum Theme : u8 { THEME_AUTO=0, THEME_AETHER, THEME_NIGHT, THEME_OCEAN, THEME_QUANTUM, THEME_FOREST, THEME_COUNT };
enum Layout : u8 { LAYOUT_CLASSIC=0, LAYOUT_MYSPACE, LAYOUT_FOCUS, LAYOUT_STUDIO, LAYOUT_LAB, LAYOUT_COUNT };

struct Profile {
    Theme theme;
    Layout layout;
    u8 accent;
    u8 density;
    bool animations;
    bool sounds;
    bool clock24;
    bool showTelemetry;
    bool showQuotes;
    bool autoLocationTheme;
    bool locationValid;
    s32 latitudeE3;
    s32 longitudeE3;
    s16 utcOffsetMinutes;
    u8 quantumQubits;
    u8 audioVoices;
    u8 visualLevel;
    u8 networkQuality;
    u8 selectedSetting;
};

void init();
void tick();
bool load();
bool save();
Profile& profile();
const Profile& current();
void nextSetting();
void previousSetting();
void adjust(int delta);
void activate();
const char* themeName(Theme t);
const char* layoutName(Layout l);
const char* locationLabel();
void timestamp(char* out, unsigned n);
}
