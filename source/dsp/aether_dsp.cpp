#include "aether_dsp.h"
#include <math.h>
namespace{aether::dsp::Metrics m={0,0,0,0};}
namespace aether::dsp{
void init(){m={0,0,0,0};}
void analyze(const unsigned char*s,unsigned n){if(!s||!n)return;unsigned long sum=0,peak=0;for(unsigned i=0;i<n;i++){unsigned v=s[i];sum+=v*v;if(v>peak)peak=v;}m.rms=(unsigned)sqrtf((float)sum/n);m.peak=peak;m.dominantBin=(sum>>8)%128;m.energy=(unsigned)(sum/n);}
void tick(){m.energy=(m.energy*31+1)>>5;} Metrics metrics(){return m;}
}