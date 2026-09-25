#include "aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"
#include <nds.h>
#include <stdio.h>
namespace aether::ui {
static const char* name(int i){static const char*n[]={"CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS"};return (i>=0&&i<8)?n[i]:"SYSTEM";}
void init(){consoleClear();}
void update(const SystemState&){}
static void header(const char* t){consoleClear();iprintf(" AETHEROS REV:F // %s\n",t);iprintf(" =================================\n");}
void render(const SystemState&s){
 if(s.screen==0){
  header("AETHER DESKTOP");
  iprintf(" SD:%s  WORKSPACE:%s\n",s.sdReady?"READY":"ERR",s.sdWriteReady?"READY":"CHECK");
  iprintf(" QUANTUM:%s  PROJECT:%s\n",s.quantumReady?"READY":"CHECK",s.projectSaved?"SAVED":"NEW");
  iprintf(" BENCH:%s  FRAME:%lu\n\n",s.benchmarkComplete?"DONE":"RUN", (unsigned long)s.frame);
  iprintf(" MODULES (A=OPEN)\n");
  const char*m[]={"CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS"};
  for(int i=0;i<8;i++){iprintf("%c %-10s",i==s.selectedModule?'>':' ',m[i]);if((i&1)==1)iprintf("\n");}
  iprintf("\n D-PAD/TOUCH SELECT   START SAFE MODE\n");
  iprintf(" A OPEN   B DESKTOP\n");
  if(s.safeMode)iprintf("\n SAFE MODE: PLUGINS/EXPERIMENTS PAUSED\n");
  return;
 }
 if(s.screen==1){header("CORE / DIAGNOSTICS");iprintf("FRAME %lu\nSD %s\nTOUCH %s\nBENCHMARK %s\n\nA=SAVE PROJECT\nB=DESKTOP\n",(unsigned long)s.frame,s.sdReady?"READY":"ERROR",s.touchReady?"READY":"ERROR",s.benchmarkComplete?"COMPLETE":"RUNNING");return;}
 if(s.screen==2){header("QUANTUM CORE");auto&q=simulator();iprintf("BACKEND: LOCAL STATE VECTOR\nQUBITS: %d  SHOTS: %d\n",q.qubits,q.shots);iprintf("BELL: %s  LAST: %d\n\n",q.bellState?"ON":"OFF",q.lastMeasurement);for(int i=0;i<(1<<q.qubits)&&i<8;i++)iprintf("|%d>  %3d%%\n",i,(int)(q.probability[i]*100));iprintf("\nA RUN BELL  SELECT RESET\nB DESKTOP\n");return;}
 if(s.screen==3){header("AETHER SOUND");iprintf("AUDIO ENGINE SHELL READY\n\nTRACKER  [READY]\nSYNTH    [READY]\nSAMPLER  [READY]\nMIXER    [READY]\nDSP FX   [READY]\n\nA CREATE PROJECT\nB DESKTOP\n");return;}
 if(s.screen==4){header("AETHER DSP");iprintf("MODULAR DSP GRAPH\n\nNODE: INPUT -> FILTER -> EQ -> OUTPUT\nLAB SEED: %lu\n\nA ADVANCE\nB DESKTOP\n",(unsigned long)engine::labValue());return;}
 if(s.screen==5){header("AETHER LAB");iprintf("ANALYSIS ENGINE ONLINE\n\nFFT / WAVEFORM / MATRIX\nMONTE CARLO / PHYSICS\nPROCEDURAL / ENTROPY\nVALUE: %lu\n\nA ADVANCE\nB DESKTOP\n",(unsigned long)engine::labValue());return;}
 if(s.screen==6){header("MICRO-AI");iprintf("LOCAL LIGHTWEIGHT ENGINE\n\nPATTERN GENERATION\nONSET / CLASSIFICATION\nPROCEDURAL COMPOSITION\nSEED: %lu\n\nA GENERATE\nB DESKTOP\n",(unsigned long)engine::aiValue());return;}
 if(s.screen==7){header("NETWORK FABRIC");iprintf("DSi WI-FI GATEWAY\n\nOFFLINE QUEUE: READY\nREMOTE COMPUTE: READY\nEXTERNAL QPU: READY\n5G/SATELLITE: EXTERNAL GATEWAY\n\nB DESKTOP\n");return;}
 if(s.screen==8){header("PROJECTS");iprintf("DEFAULT PROJECT: %s\n\nA SAVE PROJECT\nSELECT RESET QUANTUM\nB DESKTOP\n",s.projectSaved?"FOUND":"NEW");}
}
}