#include "network_fabric.h"
#include "wifi_transport.h"
#include <stdio.h>
namespace { unsigned ticks=0; bool wifiUp=false; }
namespace aether::network {
void init(){ticks=0;wifiUp=wifi::init();}
LinkStatus status(LinkType t){
 LinkStatus s={t,false,false,0,0};
 if(t==LINK_WIFI){s.available=wifi::connected();s.configured=wifiUp;if(s.available){s.latencyMs=1;s.bandwidthKbps=1000;}}
 else if(t==LINK_PHONE_5G){s.configured=true;s.available=wifi::connected();}
 else if(t==LINK_SATELLITE||t==LINK_BLUETOOTH){s.configured=false;s.available=false;}
 else if(t==LINK_REMOTE_COMPUTE){s.configured=wifi::connected();s.available=false;}
 return s;
}
bool queue(const char*kind,const char*payload){FILE*f=fopen("REVF/NETWORK/QUEUE.TXT","a");if(!f)return false;fprintf(f,"%s|%s\n",kind?kind:"EVENT",payload?payload:"");fclose(f);return true;}
bool remoteAI(const char*request,char*out,unsigned cap){(void)request;(void)out;(void)cap;return false;}
bool remoteQPU(const char*request,char*out,unsigned cap){(void)request;(void)out;(void)cap;return false;}
void tick(){ticks++;}
}