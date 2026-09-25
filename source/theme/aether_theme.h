#pragma once
#include "../settings/aether_settings.h"
namespace aether::theme {
void init();
settings::Theme active();
const char* name();
const char* sky();
const char* ground();
const char* accent();
const char* icon();
void cycle();
}
