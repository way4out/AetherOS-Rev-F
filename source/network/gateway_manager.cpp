#include "gateway_manager.h"
#include <stdio.h>
#include <string.h>
namespace { 
aether::gate::Status st[aether::gate::GATE_COUNT];
char ep[aether::gate::GATE_COUNT][48];
unsigned ports[aether::gate::GATE_COUNT];
unsigned tickCount=0;
}
namespace aether::gate {
const char* name(Kind k){static const char*n[GATE_COUNT]={"5G","SAT","BLUETOOTH","SDR","QPU","AI"};return (k<GATE_COUNT)?n[k]:"UNKNOWN";}
void init(){memset(st,0,sizeof(st));memset(ep,0,sizeof(ep));memset(ports,0,sizeof(ports));tickCount=0;}
bool configure(Kind k,const char* endpoint,unsigned port){
 if(k>=GATE_COUNT||!endpoint||!endpoint[0])return false;
 strncpy(ep[k],endpoint,sizeof(ep[k])-1); ep[k][sizeof(ep[k])-1]=0; ports[k]=port; st[k].configured=true; return true;
}
void tick(){
 ++tickCount;
 for(unsigned i=0;i<GATE_COUNT;i++){
   if(!st[i].configured) continue;
   /* The DSi side never pretends an external gateway is online until a real
      transport reports success. This keeps capability display honest. */
   if((tickCount%120)==0 && st[i].online){st[i].packets++; if(st[i].latencyMs>1) st[i].latencyMs--;}
 }
}
Status status(Kind k){return k<GATE_COUNT?st[k]:Status{false,false,0,0,0,0};}
bool anyOnline(){for(unsigned i=0;i<GATE_COUNT;i++)if(st[i].online)return true;return false;}
unsigned onlineCount(){unsigned n=0;for(unsigned i=0;i<GATE_COUNT;i++)if(st[i].online)n++;return n;}
}
