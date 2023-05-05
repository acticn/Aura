#pragma once
#include "../rhi.h"
#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif



namespace Aura {
    class VulkanRHI : public RHI {
        public:
            virtual void initialize();
            virtual void render_test();

            void createInstance();
            bool checkValidationLayerSupport();
            std::vector<const char*> getRequiredExtensions();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        
        private:
            VkInstance instance;
    };
} 