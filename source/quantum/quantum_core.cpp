#include "quantum_core.h"
#include <math.h>
#include <stdlib.h>

namespace {
static float re[256], im[256];
static int nstates=2;
static void normalize(){float s=0;for(int i=0;i<nstates;++i)s+=re[i]*re[i]+im[i]*im[i];if(s<=0)return;float k=1.0f/sqrtf(s);for(int i=0;i<nstates;++i){re[i]*=k;im[i]*=k;}}
static void collapse(int state){for(int i=0;i<nstates;++i){re[i]=0;im[i]=0;}re[state]=1;}
static int sample(){float r=(float)(rand()%10000)/10000.0f,a=0;for(int i=0;i<nstates;++i){a+=re[i]*re[i]+im[i]*im[i];if(r<=a)return i;}return nstates-1;}
}
namespace aether::quantum {
BackendInfo backend(){return {"Aether Local State Vector",true,true};}
void init(Simulator&q){q.qubits=3;q.shots=0;q.lastMeasurement=0;q.bellState=false;reset(q);}
void reset(Simulator&q){nstates=1<<q.qubits;for(int i=0;i<256;++i){re[i]=0;im[i]=0;}re[0]=1;for(int i=0;i<8;++i)q.probability[i]=0;q.lastMeasurement=0;q.shots=0;q.bellState=false;}
void hadamard(Simulator&q,int bit){if(bit<0||bit>=q.qubits)return;int mask=1<<bit;float k=.70710678f;for(int base=0;base<nstates;base+=(mask<<1))for(int j=0;j<mask;++j){int a=base+j,b=a+mask;float ar=re[a],ai=im[a],br=re[b],bi=im[b];re[a]=(ar+br)*k;im[a]=(ai+bi)*k;re[b]=(ar-br)*k;im[b]=(ai-bi)*k;}normalize();}
void pauliX(Simulator&,int bit){int mask=1<<bit;for(int i=0;i<nstates;++i)if(!(i&mask)){int j=i|mask;float tr=re[i],ti=im[i];re[i]=re[j];im[i]=im[j];re[j]=tr;im[j]=ti;}}
void cnot(Simulator&,int control,int target){if(control==target)return;int cm=1<<control,tm=1<<target;for(int i=0;i<nstates;++i)if((i&cm)&&!(i&tm)){int j=i|tm;float tr=re[i],ti=im[i];re[i]=re[j];im[i]=im[j];re[j]=tr;im[j]=ti;}}
void runBell(Simulator&q){reset(q);hadamard(q,0);cnot(q,0,1);q.bellState=true;tick(q);}
void runGrover2(Simulator&q){q.qubits=2;reset(q);hadamard(q,0);hadamard(q,1);for(int i=0;i<nstates;++i)if(i==3){re[i]*=-1;im[i]*=-1;}float mean=0;for(int i=0;i<nstates;++i)mean+=re[i];mean/=nstates;for(int i=0;i<nstates;++i)re[i]=2*mean-re[i];normalize();tick(q);}
int measure(Simulator&q){int s=sample();q.lastMeasurement=s;q.shots++;collapse(s);return s;}
void tick(Simulator&q){for(int i=0;i<8;++i)q.probability[i]=(i<nstates)?re[i]*re[i]+im[i]*im[i]:0;}
}