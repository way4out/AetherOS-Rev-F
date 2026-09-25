#include "aether_studio.h"
#include "../audio/aether_audio.h"
namespace { aether::studio::State s={120,0,0,60,100,0}; unsigned div=0; }
namespace aether::studio {
void init(){s={120,0,0,60,100,0};div=0;}
void reset(){init();}
void setBpm(unsigned b){if(b<40)b=40;if(b>240)b=240;s.bpm=b;}
void trigger(unsigned note,unsigned velocity){s.note=note;s.velocity=velocity;s.voices++;unsigned hz=220u+(note>57?(note-57u)*7u:0u);if(hz>2000)hz=2000;audio::tone(hz,70);}
void tick(){if(++div>=30){div=0;s.step=(s.step+1)&15;if((s.step&3)==0)trigger(60+(s.pattern&7),96);}}
State state(){return s;}
}
