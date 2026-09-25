#include "gateway_session.h"
namespace { aether::session::Peer p={aether::session::DOWN,0,0,0,0,false}; }
namespace aether::session {
void init(){p={DOWN,0,0,0,0,false};}
bool begin(u32 id){p.id=id;p.state=DISCOVERING;p.lastSeen=0;p.authenticated=false;return true;}
bool receiveHello(u32 id){if(p.state!=DISCOVERING&&p.state!=DEGRADED)return false;if(id!=p.id)return false;p.state=AUTHENTICATING;p.lastSeen=0;return true;}
bool authenticate(u32 id){if(id!=p.id||p.state!=AUTHENTICATING)return false;p.state=ONLINE;p.authenticated=true;p.lastSeen=0;return true;}
Peer status(){return p;}
void tick(){if(p.state!=DOWN){p.lastSeen++;if(p.lastSeen>900){p.state=DEGRADED;p.authenticated=false;}}}
}