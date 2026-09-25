#pragma once
#include <nds.h>
namespace aether::security {
void init(); bool pair(const char*); bool paired(); u32 tokenFingerprint(const char*);
}