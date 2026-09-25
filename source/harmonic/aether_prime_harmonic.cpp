#include "aether_prime_harmonic.h"
namespace { aether::harmonic::Node n{2,1,0,1000,1000,432000}; const u16 primes[]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47}; unsigned pi=0; u32 base=432000; u32 clamp(u32 x){return x>4000000000UL?4000000000UL:x;} }
namespace aether::harmonic {
void init(){n={2,1,0,1000,1000,base};}
void tick(){n.frequencyMilliHz=outputMilliHz();}
void reset(){pi=0;n.prime=2;n.harmonic=1;n.voidVector=0;n.dampener=1000;n.amplifier=1000;n.frequencyMilliHz=base;}
void nextPrime(){pi=(pi+1)%15;n.prime=primes[pi];}
void setBase(u32 f){base=f?f:432000;n.frequencyMilliHz=outputMilliHz();}
void setDampener(u16 d){n.dampener=d>4000?4000:d;}
void setAmplifier(u16 a){n.amplifier=a>4000?4000:a;}
void setVoid(s16 v){n.voidVector=v;}
const Node& node(){return n;}
u32 outputMilliHz(){s32 v=(s32)n.prime*(s32)n.harmonic + (s32)n.voidVector; if(v<1)v=1; u32 f=clamp(base*(u32)v); f=(f*(u32)n.amplifier)/1000; f=(f*(u32)n.dampener)/1000; return f;}
u32 stackHash(){return (u32)n.prime*73856093UL+(u32)n.harmonic*19349663UL+(u32)(n.voidVector+32768)*83492791UL+(u32)n.dampener*31UL+(u32)n.amplifier*17UL;}
const char* status(){return "PRIME STACK / VOID VECTOR / DAMPENER / AMPLIFIER";}
}