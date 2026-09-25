#include "wifi_transport.h"
#include <nds.h>
#include <dswifi9.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
namespace { bool g=false; }
namespace aether::wifi {
bool init(){g=Wifi_InitDefault(WFC_CONNECT);return g;}
bool connected(){return g&&Wifi_AssocStatus()==ASSOCSTATUS_ASSOCIATED;}
unsigned ip(){return Wifi_GetIP();}
bool tcpExchange(const char*host,unsigned port,const void*tx,unsigned txLen,void*rx,unsigned rxCap,unsigned&rxLen){
rxLen=0;if(!connected()||!host||!tx||!rx||!rxCap)return false;struct hostent*he=gethostbyname(host);if(!he||!he->h_addr_list[0])return false;int s=socket(AF_INET,SOCK_STREAM,0);if(s<0)return false;struct sockaddr_in a;memset(&a,0,sizeof(a));a.sin_family=AF_INET;a.sin_port=htons((u16)port);memcpy(&a.sin_addr,he->h_addr_list[0],sizeof(a.sin_addr));if(connect(s,(struct sockaddr*)&a,sizeof(a))<0){close(s);return false;}int sent=0;while(sent<(int)txLen){int n=send(s,(const char*)tx+sent,txLen-sent,0);if(n<=0){close(s);return false;}sent+=n;}int n=recv(s,(char*)rx,rxCap,0);if(n>0)rxLen=(unsigned)n;close(s);return n>0;}
bool httpGet(const char*host,const char*path,char*out,unsigned cap){if(!out||cap<128)return false;char req[512];snprintf(req,sizeof(req),"GET %s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n",path?path:"/",host?host:"");unsigned n=0;if(!tcpExchange(host,80,req,(unsigned)strlen(req),out,cap-1,n))return false;out[n]=0;return true;}
}