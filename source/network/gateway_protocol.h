#pragma once
#include <nds.h>
namespace aether::protocol {
static const u16 VERSION=1;
enum Kind { HELLO=1, HELLO_ACK=2, PING=3, PONG=4, AUTH=5, JOB=6, RESULT=7, TELEMETRY=8, QUEUE=9, ERROR=10 };
struct Frame { u16 version; u16 kind; u32 id; u32 length; u32 crc; };
u32 crc32(const void*, u32);
bool encode(Kind, u32, const void*, u32, void*, u32, u32&);
bool decode(const void*, u32, Frame&, const void*&);
bool selfTest();
}
