#pragma once
#include "../rhi.h"
#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "vulkan_instance.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

namespace Aura {
    class VulkanRHI : public RHI {
        public:
            virtual void initialize();
            virtual void render_test();
            void initWindow();
            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
            bool framebufferResized = false;
        private:
            VulkanInstance instance;
            GLFWwindow* window;
    };
} 