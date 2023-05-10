#pragma once
#include "../rhi.h"
#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <set>
#include "../rhi_struct.h"
#define LOG_ERROR(msg) std::cout << "LOG:" << msg << std::endl;
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;



namespace Aura {
    class VulkanRHI : public RHI {
        public:
            virtual void initialize();
            virtual void render_test();
            
            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
            bool framebufferResized = false;

        private:
            void createInstance();
                bool checkValidationLayerSupport();
                std::vector<const char*> getRequiredExtensions();
                void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            void initializeDebugMessenger();
                VkResult VulkanRHI::createDebugUtilsMessengerEXT(VkInstance                                instance,
                                                     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                     const VkAllocationCallbacks*              pAllocator,
                                                     VkDebugUtilsMessengerEXT*                 pDebugMessenger);
            void initializePhysicalDevice();
            bool isDeviceSuitable(VkPhysicalDevice physicalm_device);
            QueueFamilyIndices VulkanRHI::findQueueFamilies(VkPhysicalDevice physicalm_device);
            bool checkDeviceExtensionSupport(VkPhysicalDevice physicalm_device);
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalm_device);
        public:
            GLFWwindow*        m_window {nullptr};
            VkSurfaceKHR       m_surface {nullptr};
        private:
            VkInstance m_instance;
            VkPhysicalDevice m_physical_device;
            bool m_enable_validation_Layers = true;
            VkDebugUtilsMessengerEXT m_debug_messenger = nullptr;
            const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };
            std::vector<char const*> m_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
            void initWindow();
            void createWindowSurface();
    };
} 