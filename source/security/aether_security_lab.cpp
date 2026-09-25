#include "aether_security_lab.h"
namespace { aether::securitylab::Report r={aether::securitylab::PASSIVE_RF,false,true,true,true,0,0,0,0,0}; unsigned seed=0xA37E91u; }
namespace aether::securitylab {
void init(){r={PASSIVE_RF,false,true,true,true,0,0,0,0,0};}
void tick(){seed=seed*1664525u+1013904223u; if(r.mode==PASSIVE_RF&&r.consent&&((seed>>20)&31)==0)++r.samples;}
void setMode(Mode m){if(m>=MODE_COUNT)m=PASSIVE_RF;r.mode=m;r.consent=false;}
void acknowledge(){r.consent=true;}
void sample(){if(!r.consent)return;++r.samples;r.devices=(r.devices+((seed>>24)&3))%64;}
void analyze(){if(!r.consent)return;r.alerts=(r.samples+r.devices)%17;}
void runLabSimulation(){if(r.mode!=LAB_SIMULATION)return;if(!r.consent)return;++r.labRuns;r.packets+=8;r.alerts=(r.labRuns*3)%19;}
Report report(){return r;}
const char* modeName(Mode m){static const char*n[]={"PASSIVE RF","AUTHORIZED NET","LAB SIM","GATEWAY HARDEN"};return n[m<MODE_COUNT?m:0];}
const char* warning(){return "AUTHORIZED USE ONLY: TX/jamming, credential theft, covert interception and unauthorized access are locked."; }
}
