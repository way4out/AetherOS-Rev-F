#pragma once
#include <nds.h>
namespace aether::diag {
struct Report { unsigned frame; unsigned faults; unsigned graphTicks; unsigned gatewayOnline; unsigned score; };
void init();
void tick(unsigned frame);
void fault();
Report report();
}
