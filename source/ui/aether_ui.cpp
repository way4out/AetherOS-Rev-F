#include "aether_ui.h"
#include "../quantum/quantum_core.h"
#include "../engine/engine_modules.h"
#include <nds.h>
namespace aether::ui {
void init(){consoleClear();} void update(const SystemState&){}
static void header(const char*t){consoleClear();iprintf(" AETHEROS REV:F // %s\n =================================\n",t);}
void render(const SystemState&s){
if(s.screen==0){header("AETHER DESKTOP");iprintf("SD:%s WS:%s Q:%s AUDIO:%s\n",s.sdReady?"OK":"ERR",s.sdWriteReady?"OK":"ERR",s.quantumReady?"OK":"ERR",s.audioReady?"OK":"ERR");iprintf("BENCH:%s PROJECT:%s F:%lu\n\n",s.benchmarkComplete?"OK":"RUN",s.projectSaved?"SAVED":"NEW",(unsigned long)s.frame);const char*m[]={"CORE","QUANTUM","SOUND","DSP","LAB","AI","NETWORK","PROJECTS"};for(int i=0;i<8;i++){iprintf("%c%-9s",i==s.selectedModule?'>':' ',m[i]);if(i&1)iprintf("\n");}iprintf("\nA OPEN D-PAD/TOUCH SELECT\nSTART SAFE MODE");if(s.safeMode)iprintf("\nSAFE MODE ACTIVE");return;}
if(s.screen==1){header("CORE / DIAGNOSTICS");iprintf("FRAME %lu\nSD %s\nTOUCH %s\nAUDIO %s\nQUANTUM %s\nBENCH %s\nA SAVE\nB DESKTOP",(unsigned long)s.frame,s.sdReady?"READY":"ERROR",s.touchReady?"READY":"ERROR",s.audioReady?"READY":"ERROR",s.quantumReady?"READY":"ERROR",s.benchmarkComplete?"DONE":"RUN");return;}
if(s.screen==2){header("QUANTUM CORE");auto&q=simulator();iprintf("STATE VECTOR / %d QUBITS\nSHOTS %d ALG %d\nBELL %s LAST %d\n",q.qubits,q.shots,q.algorithm,q.bellState?"ON":"OFF",q.lastMeasurement);for(int i=0;i<(1<<q.qubits)&&i<8;i++)iprintf("|%d> %3d%%\n",i,(int)(q.probability[i]*100));iprintf("\nA BELL X GROVER Y MEASURE\nL DJ R QFT SELECT RESET");return;}
if(s.screen==3){header("AETHER SOUND");iprintf("AUDIO %s\nTRACKER / SYNTH / SAMPLER\nDSP FX / MIXER CORE\n\nA PLAY 440Hz TEST\nB DESKTOP",s.audioReady?"READY":"ERROR");return;}
if(s.screen==4){header("AETHER DSP");iprintf("REALTIME DSP GRAPH\nINPUT>FILTER>EQ>OUTPUT\nSTATE %lu\nA ADVANCE\nB DESKTOP",(unsigned long)engine::labValue());return;}
if(s.screen==5){header("AETHER LAB");iprintf("FFT/WAVEFORM/MATRIX\nMONTE CARLO/PHYSICS\nPROCEDURAL/ENTROPY\nVALUE %lu\nA ADVANCE\nB DESKTOP",(unsigned long)engine::labValue());return;}
if(s.screen==6){header("MICRO AI");iprintf("LOCAL LIGHT INFERENCE\nONSET/CLASSIFY/PATTERN\nSEED %lu\nA GENERATE\nB DESKTOP",(unsigned long)engine::aiValue());return;}
if(s.screen==7){header("NETWORK FABRIC");iprintf("DSI WIFI / HOTSPOT\nOFFLINE QUEUE READY\nREMOTE COMPUTE READY\nEXTERNAL QPU READY\n5G/SAT GATEWAY EXT\nB DESKTOP");return;}
header("PROJECTS");iprintf("DEFAULT: %s\nA SAVE\nSELECT RESET Q\nB DESKTOP",s.projectSaved?"FOUND":"NEW");
}}
