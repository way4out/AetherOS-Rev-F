#include "aether_animal.h"
namespace {
aether::animal::Report r{aether::animal::UNKNOWN,aether::animal::BIDIRECTIONAL,0,0,0,0,false,false};
u16 level=0,pitch=0,rhythm=0;
const char* text="Awaiting signal";
const char* signal="Awaiting species signal";
const char* sp[]={"Unknown","Horse","Dog","Cat","Cow","Bison","Camel","Zebra","Ostrich","Elephant","Dolphin","Orca","Beluga","Whale","Bird","Crow","Raven","Parrot","Finch","Bee","Frog","Bat","Wolf","Coyote","Fox","Bear","Primate","Chimpanzee","Pig","Goat","Sheep","Donkey","Chicken"};
}
namespace aether::animal {
void init(){r={UNKNOWN,BIDIRECTIONAL,0,0,0,0,false,false};}
void tick(){r.gatewayReady=false;}
void setSpecies(Species s){r.species=s;}
void setDirection(Direction d){r.direction=d;}
void ingestAudio(u16 l,u16 p,u16 y){level=l;pitch=p;rhythm=y;++r.samples;}
void analyze(){r.signalScore=(u16)((level+pitch+rhythm)/3);r.confidence=(u16)((r.signalScore>100?100:r.signalScore)*10);r.intent=(u16)((pitch/32+rhythm/32)%16);r.gatewayReady=false;r.modelBacked=false;text="Signal analyzed; semantic decoding requires a trained model gateway";}
void synthesize(){signal="Species-conditioned signal profile; acoustic rendering requires a trained model gateway";}
const Report& report(){return r;}
const char* speciesName(){return sp[r.species<SPECIES_COUNT?r.species:0];}
const char* directionName(){return r.direction==ANIMAL_TO_HUMAN?"ANIMAL > HUMAN":r.direction==HUMAN_TO_ANIMAL?"HUMAN > ANIMAL":"ANIMAL < > HUMAN";}
const char* humanText(){return text;}
const char* animalSignal(){return signal;}
bool gatewayRequired(){return !r.modelBacked;}
bool safetyReady(){return true;}
}