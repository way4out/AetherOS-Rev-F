#pragma once
namespace aether::network {
enum LinkType { LINK_NONE, LINK_WIFI, LINK_PHONE_5G, LINK_SATELLITE, LINK_BLUETOOTH, LINK_REMOTE_COMPUTE };
struct LinkStatus { LinkType type; bool available; bool configured; unsigned latencyMs; unsigned bandwidthKbps; };
void init(); void tick(); LinkStatus status(LinkType); bool queue(const char*,const char*); bool remoteAI(const char*,char*,unsigned); bool remoteQPU(const char*,char*,unsigned);
}