#pragma once
#include "../rhi.h"
#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>


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
            void createInstance();
                bool checkValidationLayerSupport();
                std::vector<const char*> getRequiredExtensions();
                void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        private:
            VkInstance instance;
            GLFWwindow* window;
            bool enableValidationLayers = true;
            const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };
    };
} 