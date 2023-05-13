#include <stdio.h>
#include "render/interface/rhi.h"
#include "render/interface/vulkan_rhi/vulkan_rhi.h"
#include "Aura.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>
int main() {


    Aura::Aura aura;
    aura.run();
    printf("hello world \n");
    return 0;
}