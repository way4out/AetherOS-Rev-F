#pragma once
#include <nds.h>
namespace aether::animal {
enum Species : u8 { UNKNOWN=0,HORSE,DOG,CAT,COW,BISON,CAMEL,ZEBRA,OSTRICH,ELEPHANT,DELPHIN,ORCA,BELUGA,WHale,BIRD,CROW,RAVEN,PARROT,FINCH,BEE,FROG,BAT,WOLF,COYOTE,FOX,BEAR,PRIMATE,CHIMP,PIG,GOAT,SHEEP,DONKEY,CHICKEN,SPECIES_COUNT };
enum Direction : u8 { ANIMAL_TO_HUMAN=0,HUMAN_TO_ANIMAL,BIDIRECTIONAL };
struct Report { Species species; Direction direction; u16 confidence; u16 signalScore; u16 intent; u32 samples; bool gatewayReady; bool modelBacked; };
void init(); void tick(); void analyze(); void synthesize(); void setSpecies(Species); void setDirection(Direction);
void ingestAudio(u16 level,u16 pitch,u16 rhythm); const Report& report(); const char* speciesName(); const char* directionName();
const char* humanText(); const char* animalSignal(); bool gatewayRequired(); bool safetyReady();
}