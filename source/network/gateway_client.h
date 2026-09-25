#pragma once
namespace aether::gateway {
bool ping(const char* host,unsigned port,unsigned& latencyMs);
bool submitAI(const char* host,unsigned port,const char* request,char* result,unsigned cap);
bool submitQPU(const char* host,unsigned port,const char* circuit,char* result,unsigned cap);
}