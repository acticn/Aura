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
            std::vector<RHIFramebuffer*> framebuffers;
            void mainLoop();
            void drawFrame();
            void initialize();
            void setupRenderPass();
            void setupFrameBuffers();
    };
}