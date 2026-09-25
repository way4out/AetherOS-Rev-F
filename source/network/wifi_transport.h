#pragma once
namespace aether::wifi {
bool init();
bool connected();
unsigned ip();
bool httpGet(const char* host,const char* path,char* out,unsigned cap);
bool tcpExchange(const char* host,unsigned port,const void* tx,unsigned txLen,void* rx,unsigned rxCap,u32& rxLen);
}