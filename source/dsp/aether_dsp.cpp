#include "aether_dsp.h"
#include <math.h>
namespace{aether::dsp::Metrics m={0,0,0,0};}
namespace aether::dsp{
void init(){m={0,0,0,0};}
void analyze(const unsigned char*s,unsigned n){if(!s||!n)return;unsigned long sum=0,peak=0;for(unsigned i=0;i<n;i++){unsigned v=s[i];sum+=v*v;if(v>peak)peak=v;}m.rms=(unsigned)sqrtf((float)sum/n);m.peak=peak;m.dominantBin=(sum>>8)%128;m.energy=(unsigned)(sum/n);}
void fft128(const unsigned char*s,float*mag,float*phase){if(!s||!mag||!phase)return;for(int k=0;k<64;k++){float re=0,im=0;for(int n=0;n<128;n++){float a=-6.28318530718f*k*n/128.0f;float x=(float)s[n]-128.0f;re+=x*cosf(a);im+=x*sinf(a);}mag[k]=sqrtf(re*re+im*im)/128.0f;phase[k]=atan2f(im,re);}}
void tick(){m.energy=(m.energy*31+1)>>5;} Metrics metrics(){return m;}
}
