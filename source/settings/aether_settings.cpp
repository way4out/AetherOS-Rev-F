#include "aether_settings.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

namespace {
aether::settings::Profile p;
bool dirty=false;
const char* path="REVF/CORE/SETTINGS.CFG";

void defaults(){
    p.theme=aether::settings::THEME_AUTO;
    p.layout=aether::settings::LAYOUT_MYSPACE;
    p.accent=10; p.density=2;
    p.animations=true; p.sounds=true; p.clock24=false;
    p.showTelemetry=true; p.showQuotes=true; p.autoLocationTheme=true;
    p.locationValid=false; p.latitudeE3=0; p.longitudeE3=0; p.utcOffsetMinutes=0;
    p.quantumQubits=4; p.audioVoices=8; p.visualLevel=3; p.networkQuality=2;
    p.selectedSetting=0; p.language=1;
}
}

namespace aether::settings {
void init(){ defaults(); load(); }
Profile& profile(){ return p; }
const Profile& current(){ return p; }
bool load(){
    FILE* f=fopen(path,"rb");
    if(!f) return false;
    size_t n=fread(&p,1,sizeof(p),f); fclose(f);
    if(n!=sizeof(p)){ defaults(); return false; }
    if(p.theme>=THEME_COUNT)p.theme=THEME_AUTO;
    if(p.layout>=LAYOUT_COUNT)p.layout=LAYOUT_MYSPACE;
    if(p.quantumQubits<2||p.quantumQubits>8)p.quantumQubits=4;
    if(p.language>=52)p.language=1;
    if(p.audioVoices<1||p.audioVoices>16)p.audioVoices=8;
    dirty=false; return true;
}
bool save(){
    FILE* f=fopen(path,"wb");
    if(!f) return false;
    bool ok=fwrite(&p,1,sizeof(p),f)==sizeof(p);
    fclose(f); dirty=!ok; return ok;
}
void tick(){ if(dirty && (p.selectedSetting==255)) save(); }
void nextSetting(){ p.selectedSetting=(u8)((p.selectedSetting+1)%19); }
void previousSetting(){ p.selectedSetting=(u8)((p.selectedSetting+18)%19); }

void adjust(int d){
    switch(p.selectedSetting){
    case 0: p.theme=(Theme)((p.theme+d+THEME_COUNT)%THEME_COUNT); break;
    case 1: p.layout=(Layout)((p.layout+d+LAYOUT_COUNT)%LAYOUT_COUNT); break;
    case 2: p.accent=(u8)((p.accent+d+16)%16); break;
    case 3: p.density=(u8)((p.density+d+4)%4); break;
    case 4: p.animations=!p.animations; break;
    case 5: p.sounds=!p.sounds; break;
    case 6: p.clock24=!p.clock24; break;
    case 7: p.showTelemetry=!p.showTelemetry; break;
    case 8: p.showQuotes=!p.showQuotes; break;
    case 9: p.autoLocationTheme=!p.autoLocationTheme; break;
    case 10: p.locationValid=!p.locationValid; break;
    case 11: p.latitudeE3+=d*1000; if(p.latitudeE3>90000)p.latitudeE3=90000;if(p.latitudeE3<-90000)p.latitudeE3=-90000; break;
    case 12: p.longitudeE3+=d*1000; if(p.longitudeE3>180000)p.longitudeE3=180000;if(p.longitudeE3<-180000)p.longitudeE3=-180000; break;
    case 13: p.utcOffsetMinutes+=d*30; if(p.utcOffsetMinutes>840)p.utcOffsetMinutes=840;if(p.utcOffsetMinutes<-720)p.utcOffsetMinutes=-720; break;
    case 14: p.quantumQubits=(u8)((p.quantumQubits-2+d+7)%7+2); break;
    case 15: p.audioVoices=(u8)((p.audioVoices-1+d+16)%16+1); break;
    case 16: p.visualLevel=(u8)((p.visualLevel+d+5)%5); break;
    case 17: save(); break;
    case 18: p.language=(u8)((p.language+d+52)%52); break;
    default: break;
    }
    dirty=true;
}
void activate(){ adjust(1); }

const char* themeName(Theme t){
    static const char* n[]={"AUTO","AETHER VALLEY","NIGHT SKY","OCEAN","QUANTUM","FOREST"};
    return n[t<THEME_COUNT?t:0];
}
const char* layoutName(Layout l){
    static const char* n[]={"CLASSIC","MYSPACE","FOCUS","STUDIO","LAB"};
    return n[l<LAYOUT_COUNT?l:0];
}
const char* locationLabel(){
    if(!p.locationValid) return "LOCATION UNSET";
    if(p.autoLocationTheme) return "MANUAL LOCATION / AUTO THEME";
    return "MANUAL LOCATION";
}
void timestamp(char* out,unsigned n){
    time_t now=time(NULL);
    struct tm* t=localtime(&now);
    if(!t){ if(n) out[0]=0; return; }
    if(p.clock24) snprintf(out,n,"%04d-%02d-%02d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    else {
        int h=t->tm_hour%12; if(h==0)h=12;
        snprintf(out,n,"%04d-%02d-%02d %02d:%02d:%02d %s",t->tm_year+1900,t->tm_mon+1,t->tm_mday,h,t->tm_min,t->tm_sec,t->tm_hour>=12?"PM":"AM");
    }
}
}
