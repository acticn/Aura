#pragma once
#include "../rhi.h"
#include <iostream>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <set>
#include "../rhi_struct.h"
#include "vulkan_rhi_resource.h"
#include "../../render_type.h"
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

            void createLogicalDevice();


            static uint8_t const k_max_frames_in_flight {3};
            uint32_t m_max_vertex_blending_mesh_count{ 256 };
            uint32_t m_max_material_count{ 256 };
            bool m_enable_validation_Layers{true};
            bool m_enable_point_light_shadow{true};
            VkDebugUtilsMessengerEXT m_debug_messenger = nullptr;
            const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
            };
            std::vector<char const*> m_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        public:
            GLFWwindow*        m_window {nullptr};
            VkSurfaceKHR       m_surface {nullptr};
            RHIQueue* m_graphics_queue{ nullptr };
            RHIQueue* m_compute_queue{ nullptr };
            VkDevice           m_device {nullptr};
            VkQueue            m_present_queue {nullptr};
            RHIFormat m_depth_image_format{ RHI_FORMAT_UNDEFINED };
            RHIDescriptorPool* m_descriptor_pool = new VulkanDescriptorPool();

            VkDescriptorPool m_vk_descriptor_pool;
                // function pointers
            PFN_vkCmdBeginDebugUtilsLabelEXT _vkCmdBeginDebugUtilsLabelEXT;
            PFN_vkCmdEndDebugUtilsLabelEXT   _vkCmdEndDebugUtilsLabelEXT;
            PFN_vkWaitForFences         _vkWaitForFences;
            PFN_vkResetFences           _vkResetFences;
            PFN_vkResetCommandPool      _vkResetCommandPool;
            PFN_vkBeginCommandBuffer    _vkBeginCommandBuffer;
            PFN_vkEndCommandBuffer      _vkEndCommandBuffer;
            PFN_vkCmdBeginRenderPass    _vkCmdBeginRenderPass;
            PFN_vkCmdNextSubpass        _vkCmdNextSubpass;
            PFN_vkCmdEndRenderPass      _vkCmdEndRenderPass;
            PFN_vkCmdBindPipeline       _vkCmdBindPipeline;
            PFN_vkCmdSetViewport        _vkCmdSetViewport;
            PFN_vkCmdSetScissor         _vkCmdSetScissor;
            PFN_vkCmdBindVertexBuffers  _vkCmdBindVertexBuffers;
            PFN_vkCmdBindIndexBuffer    _vkCmdBindIndexBuffer;
            PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets;
            PFN_vkCmdDrawIndexed        _vkCmdDrawIndexed;
            PFN_vkCmdClearAttachments   _vkCmdClearAttachments;

            VkSemaphore          m_image_available_for_render_semaphores[k_max_frames_in_flight];
            VkSemaphore          m_image_finished_for_presentation_semaphores[k_max_frames_in_flight];
            RHISemaphore*        m_image_available_for_texturescopy_semaphores[k_max_frames_in_flight];
            VkFence              m_is_frame_in_flight_fences[k_max_frames_in_flight];
            RHIFence* m_rhi_is_frame_in_flight_fences[k_max_frames_in_flight];

            VkSwapchainKHR           m_swapchain {nullptr};
            std::vector<VkImage>     m_swapchain_images;
            RHIFormat m_swapchain_image_format{ RHI_FORMAT_UNDEFINED };
            RHIExtent2D m_swapchain_extent;
            RHIRect2D m_scissor;
        private:
            VkInstance m_instance;
            VkPhysicalDevice m_physical_device;
            QueueFamilyIndices m_queue_indices;
            VulkanCommandPool* m_rhi_command_pool;
            VkCommandPool        m_command_pools[k_max_frames_in_flight];
            VkCommandBuffer      m_vk_command_buffers[k_max_frames_in_flight];
            RHICommandBuffer* m_command_buffers[k_max_frames_in_flight];

            void initWindow();
            void createWindowSurface();
            VkFormat findDepthFormat();
            VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
                                     VkImageTiling                tiling,
                                     VkFormatFeatureFlags         features);

            void createCommandPool();
            void createCommandBuffers();
            void createDescriptorPool();
            void createSyncPrimitives();
            void createSwapchain();
            VkSurfaceFormatKHR chooseSwapchainSurfaceFormatFromDetails(const std::vector<VkSurfaceFormatKHR>& available_surface_formats);
            VkPresentModeKHR VulkanRHI::chooseSwapchainPresentModeFromDetails(const std::vector<VkPresentModeKHR>& available_present_modes);
            VkExtent2D chooseSwapchainExtentFromDetails(const VkSurfaceCapabilitiesKHR& capabilities);
    };
} 