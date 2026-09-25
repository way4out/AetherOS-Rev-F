#include "aether_theme.h"
#include <time.h>
namespace {
aether::settings::Theme effective(){
    auto &p=aether::settings::profile();
    if(p.theme!=aether::settings::THEME_AUTO || !p.autoLocationTheme || !p.locationValid)
        return p.theme==aether::settings::THEME_AUTO ? aether::settings::THEME_HOBBIT : p.theme;
    int lat=p.latitudeE3<0?-p.latitudeE3:p.latitudeE3;
    int lon=p.longitudeE3;
    if(lat>55000) return aether::settings::THEME_NIGHT;
    if(lon<-60000 && lat<45000) return aether::settings::THEME_OCEAN;
    if(lat<30000) return aether::settings::THEME_FOREST;
    return aether::settings::THEME_AETHER;
}
}
namespace aether::theme {
void init(){}
settings::Theme active(){return effective();}
const char* name(){return settings::themeName(active());}
const char* sky(){
    switch(active()){
    case settings::THEME_HOBBIT:return "  . . .  (__)  . . .  HOBBIT HILLS";
    case settings::THEME_AETHER:return "  ^  ^  AETHER VALLEY / SUNRISE";
    case settings::THEME_NIGHT:return "  *  .  *  NIGHT SKY / CONSTELLATION";
    case settings::THEME_OCEAN:return "  ~ ~ ~  OCEAN HORIZON / TIDE";
    case settings::THEME_QUANTUM:return "  o--o  QUANTUM FIELD / PHASE";
    default:return "  ^^^  FOREST CANOPY  ^^^  WILDLANDS";
    }
}
const char* ground(){
    switch(active()){
    case settings::THEME_HOBBIT:return "  ______  GREEN HILLS  ______";
    case settings::THEME_OCEAN:return "  ~~~~~~~~  ~~~~~~~~  TIDELINE  ~~~~~";
    case settings::THEME_QUANTUM:return "  .-o-.  .-o-.  .-o-.  ENTANGLEMENT";
    case settings::THEME_NIGHT:return "  --------  horizon / deep space  -----";
    default:return "  ___/\_____/\_____/\___  WILDLANDS";
    }
}
const char* accent(){
    switch(active()){
    case settings::THEME_HOBBIT:return "\x1b[33m";
    case settings::THEME_OCEAN:return "\x1b[36m";
    case settings::THEME_QUANTUM:return "\x1b[35m";
    case settings::THEME_NIGHT:return "\x1b[34m";
    case settings::THEME_FOREST:return "\x1b[32m";
    default:return "\x1b[36m";
    }
}
const char* icon(){
    switch(active()){
    case settings::THEME_HOBBIT:return "[HO]";
    case settings::THEME_OCEAN:return "[OC]";
    case settings::THEME_QUANTUM:return "[Q]";
    case settings::THEME_NIGHT:return "[NS]";
    case settings::THEME_FOREST:return "[FR]";
    default:return "[AV]";
    }
}
void cycle(){ auto&t=settings::profile().theme; t=(settings::Theme)((t+1)%settings::THEME_COUNT); settings::save(); }
}
