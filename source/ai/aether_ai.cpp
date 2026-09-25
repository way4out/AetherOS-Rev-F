#include "aether_ai.h"
namespace{aether::ai::Result r={0,0,0};unsigned s=7;}
namespace aether::ai{void init(){r={0,0,0};s=7;}void tick(){s=s*1103515245u+12345u;}void generate(){tick();r.pattern=s;r.classId=(s>>28)&7;r.confidence=50+((s>>20)%51);}Result result(){return r;}}