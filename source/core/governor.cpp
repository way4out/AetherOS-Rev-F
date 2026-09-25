#include "governor.h"
namespace { aether::governor::Profile p={3,8,2,1000}; unsigned t=0; }
namespace aether::governor {
void init(){p={3,8,2,1000};t=0;}
void tick(){t++;if((t%300)==0){if(p.audioVoices<16)p.audioVoices++;if(p.visualLevel<3)p.visualLevel++;}}
Profile profile(){return p;}
}