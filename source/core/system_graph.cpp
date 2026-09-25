#include "system_graph.h"
namespace { aether::graph::State s; }
namespace aether::graph {
void init(){for(int i=0;i<NODE_COUNT;i++)s.enabled[i]=true;s.ticks=0;}
void tick(){s.ticks++;}
State state(){return s;}
}