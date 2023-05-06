#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace Aura {
    class VulkanInstance {
        public:
            static VulkanInstance& getInstance();
        private :
            VkInstance instance;
        private:

            void createInstance();
            bool checkValidationLayerSupport();
            std::vector<const char*> getRequiredExtensions();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        
    };
}

