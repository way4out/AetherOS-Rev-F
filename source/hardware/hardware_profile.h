#pragma once
#include <nds.h>

namespace aether::hardware {
bool sdAvailable();
bool ensureDirectories();
bool writeBootMarker();
}
