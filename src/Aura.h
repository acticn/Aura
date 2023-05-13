#pragma once

#include "render/interface/vulkan_rhi/vulkan_rhi.h"
#include "render/interface/rhi.h"

namespace Aura {
    class Aura {
        public:
            void run();
        private:
            VulkanRHI* rhi;
            RHIRenderPass* renderpass;
            void mainLoop();
            void drawFrame();
            void initialize();
            void createRenderPass();
    };
}