#include "vulkan_rhi.h"



namespace Aura {
    void VulkanRHI::initialize() {

        instance = VulkanInstance::getInstance();
        std::cout << "initialized" << std::endl;

    }
    void VulkanRHI::render_test() {
        std::cout << "rendered" << std::endl;
    }

    
}