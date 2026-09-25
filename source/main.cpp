#include <nds.h>
#include "core/aether_core.h"
int main() {
    aether::SystemState state{};
    aether::init(state);
    while (1) {
        aether::update(state);
        aether::render(state);
        swiWaitForVBlank();
    }
}
