#pragma once
namespace aether::compute {
enum Backend { BACKEND_NONE, BACKEND_QPU, BACKEND_AI };
struct Job { Backend backend; const char* payload; unsigned length; };
void init(); bool submit(const Job&); bool available(Backend);
}