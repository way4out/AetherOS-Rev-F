#pragma once
namespace aether::quantum {
struct BackendInfo { const char* name; bool local; bool externalCapable; };
BackendInfo backend();
}
