#include "aether_lab.h"
namespace{aether::lab::Metrics m={0,0,0,0};unsigned s=0x13579BDF;}
namespace aether::lab{void init(){m={0,0,0,0};s=0x13579BDF;}void tick(){s=s*1664525u+1013904223u;m.monteCarlo=(s>>24)&255;m.entropy=((s>>16)^(s>>8))&255;m.automata=(m.automata<<1)^((s>>29)&1);m.physics=(m.physics+((s>>20)&31))&65535;}Metrics metrics(){return m;}}