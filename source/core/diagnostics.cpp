#include "diagnostics.h"
#include "system_graph.h"
#include "../network/gateway_manager.h"
namespace { aether::diag::Report r={0,0,0,0,0}; }
namespace aether::diag {
void init(){r={0,0,0,0,0};}
void tick(unsigned frame){
 r.frame=frame; r.graphTicks=aether::graph::state().ticks; r.gatewayOnline=aether::gate::onlineCount();
 unsigned base=100; if(r.graphTicks<frame/2)base-=10; if(r.faults)base-= (r.faults>50?50:r.faults);
 r.score=base;
}
void fault(){++r.faults;}
Report report(){return r;}
}
