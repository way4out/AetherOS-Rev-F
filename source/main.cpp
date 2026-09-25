#include <nds.h>
#include "core/aether_core.h"

int main() {
    aether::SystemState state{};
    aether::init(state);

    while (pmMainLoop()) {
        aether::update(state);
        aether::render(state);
        swiWaitForVBlank();
    }

    aether::shutdown();
    return 0;
}
