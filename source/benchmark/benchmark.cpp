#include "benchmark.h"
namespace aether::benchmark {
void runQuick(bool& complete) {
    volatile unsigned long x = 0;
    for (unsigned long i=0;i<50000;++i) x = x * 1664525UL + 1013904223UL;
    (void)x;
    complete = true;
}
}
