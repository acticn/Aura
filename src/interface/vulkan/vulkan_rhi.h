#pragma once
#include "../rhi.h"
namespace Aura {
    class VulkanRHI : public RHI {
        virtual void initialize();
        virtual void render_test();
    };
} 