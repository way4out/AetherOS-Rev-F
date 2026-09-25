#include "aether_audio.h"
#include <nds.h>
#include <math.h>
namespace { bool gReady=false; s8 wave[4096]; }
namespace aether::audio {
bool init(){soundEnable();for(int i=0;i<4096;i++)wave[i]=(s8)(sinf((float)i*6.2831853f/64.0f)*100.0f);gReady=true;return true;}
void tone(int hz,int ms){if(!gReady)return;if(hz<40)hz=40;if(hz>12000)hz=12000;if(ms<20)ms=20;if(ms>1000)ms=1000;int samples=(hz*ms)/1000;if(samples<256)samples=256;if(samples>4096)samples=4096;soundPlaySample(wave,SoundFormat_8Bit,samples,hz,127,64,false,0);}
void stop(){soundKillAll();} bool ready(){return gReady;} unsigned waveform(unsigned i){return (unsigned)((wave[i&4095]+128)&255);}
}
