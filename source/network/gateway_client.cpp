#include "gateway_client.h"
#include "wifi_transport.h"
#include "gateway_protocol.h"
#include <string.h>
#include <nds.h>
namespace aether::gateway {
static bool exchange(const char*h,unsigned port,protocol::Kind k,const char*payload,char*out,unsigned cap){unsigned char tx[768],rx[1024];u32 tw=0,rw=0;if(!payload||!out||cap<2)return false;unsigned len=(unsigned)strlen(payload);if(len>500)return false;if(!protocol::encode(k,1,payload,len,tx,sizeof(tx),tw))return false;if(!wifi::tcpExchange(h,port,tx,tw,rx,sizeof(rx),rw))return false;protocol::Frame f;const void*p;if(!protocol::decode(rx,rw,f,p))return false;if(f.kind!=protocol::RESULT&&f.kind!=protocol::PONG&&f.kind!=protocol::HELLO_ACK)return false;unsigned n=f.length<cap-1?f.length:cap-1;memcpy(out,p,n);out[n]=0;return true;}
bool ping(const char*h,unsigned p,unsigned&latencyMs){char out[32];u32 start=REG_VCOUNT;bool ok=exchange(h,p,protocol::PING,"ping",out,sizeof(out));latencyMs=(REG_VCOUNT-start);return ok;}
bool submitAI(const char*h,unsigned p,const char*r,char*o,unsigned c){return exchange(h,p,protocol::JOB,r,o,c);}
bool submitQPU(const char*h,unsigned p,const char*r,char*o,unsigned c){return exchange(h,p,protocol::JOB,r,o,c);}
bool submit(const char*h,unsigned p,unsigned k,const char*r,char*o,unsigned c){return exchange(h,p,(protocol::Kind)k,r,o,c);}
}
