#include <stdio.h>
#include "interface/rhi.h"
#include "interface/vulkan/vulkan_rhi.h"
int main() {
    Aura::RHI* rhi = new Aura::VulkanRHI();
    rhi->initialize();
    rhi->render_test();
    printf("hello world \n");
    return 0;
}