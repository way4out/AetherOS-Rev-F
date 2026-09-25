#pragma once
namespace aether::engine {
void init();
void tick();
void saveProject();
bool projectExists();
void resetProject();
unsigned labValue();
unsigned aiValue();
}