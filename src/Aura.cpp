#include "Aura.h"

namespace Aura {
    void Aura::run() {
        RHI* rhi = new VulkanRHI();
        rhi->initialize();
        rhi->render_test();
    }
} 