#include "gateway_protocol.h"
#include <string.h>
namespace aether::protocol {
static const u32 MAGIC=0x41544852;
u32 crc32(const void* data,u32 n){const u8*p=(const u8*)data;u32 c=0xFFFFFFFFu;for(u32 i=0;i<n;i++){c^=p[i];for(int j=0;j<8;j++)c=(c>>1)^((c&1)?0xEDB88320u:0);}return c^0xFFFFFFFFu;}
bool encode(Kind k,u32 id,const void* payload,u32 n,void* out,u32 cap,u32& written){if(!out||n>65536||cap<n+20)return false;u8*b=(u8*)out;u32 m=MAGIC;u16 v=1,kk=(u16)k;memcpy(b,&m,4);memcpy(b+4,&v,2);memcpy(b+6,&kk,2);memcpy(b+8,&id,4);memcpy(b+12,&n,4);u32 c=crc32(payload,n);memcpy(b+16,&c,4);if(n&&payload)memcpy(b+20,payload,n);written=n+20;return true;}
bool decode(const void* in,u32 size,Frame& f,const void*& payload){if(!in||size<20)return false;const u8*b=(const u8*)in;u32 m;memcpy(&m,b,4);if(m!=MAGIC)return false;memcpy(&f.version,b+4,2);memcpy(&f.kind,b+6,2);memcpy(&f.id,b+8,4);memcpy(&f.length,b+12,4);memcpy(&f.crc,b+16,4);if(f.version!=1||f.length>size-20)return false;payload=b+20;return crc32(payload,f.length)==f.crc;}
}