#include "gateway_security.h"
namespace { bool g=false; u32 fingerprint=0; }
namespace aether::security {
static u32 mix(const char*s){u32 h=2166136261u;if(!s)return 0;while(*s){h^=(u8)*s++;h*=16777619u;}return h;}
void init(){g=false;fingerprint=0;}u32 tokenFingerprint(const char*t){return mix(t);}
bool pair(const char*t){if(!t||!t[0])return false;fingerprint=mix(t);g=(fingerprint!=0);return g;}bool paired(){return g;}
}