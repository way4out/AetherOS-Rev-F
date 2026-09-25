#include "gateway_protocol.h"
#include <string.h>
namespace aether::protocol {
static const u32 MAGIC=0x41544852u;
u32 crc32(const void* data,u32 n){
    const u8*p=(const u8*)data; u32 c=0xFFFFFFFFu;
    for(u32 i=0;i<n;i++){c^=p[i];for(int j=0;j<8;j++)c=(c>>1)^((c&1)?0xEDB88320u:0);}
    return c^0xFFFFFFFFu;
}
bool encode(Kind k,u32 id,const void* payload,u32 n,void* out,u32 cap,u32& written){
    if(!out||n>65536u||cap<n+20u||(n&& !payload))return false;
    u8*b=(u8*)out; u32 m=MAGIC; u16 v=VERSION,kk=(u16)k;
    memcpy(b,&m,4);memcpy(b+4,&v,2);memcpy(b+6,&kk,2);memcpy(b+8,&id,4);memcpy(b+12,&n,4);
    u32 c=crc32(payload,n);memcpy(b+16,&c,4);if(n)memcpy(b+20,payload,n);written=n+20;return true;
}
bool decode(const void* in,u32 size,Frame& f,const void*& payload){
    if(!in||size<20u)return false;const u8*b=(const u8*)in;u32 m;
    memcpy(&m,b,4);if(m!=MAGIC)return false;memcpy(&f.version,b+4,2);memcpy(&f.kind,b+6,2);
    memcpy(&f.id,b+8,4);memcpy(&f.length,b+12,4);memcpy(&f.crc,b+16,4);
    if(f.version!=VERSION||f.length>65536u||f.length!=size-20u)return false;
    payload=b+20;return crc32(payload,f.length)==f.crc;
}
bool selfTest(){
    char payload[]="AETHER-HIL"; u8 buf[64];u32 n=0;Frame f;const void*p=0;
    if(!encode(PING,42,payload,10,buf,sizeof(buf),n))return false;
    if(!decode(buf,n,f,p))return false;
    if(f.kind!=PING||f.id!=42||f.length!=10||memcmp(p,payload,10)!=0)return false;
    buf[20]^=1; if(decode(buf,n,f,p))return false;
    return true;
}
}
