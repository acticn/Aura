#pragma once
#include <optional>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
namespace Aura {
    class RHIQueue { };
    class RHICommandPool { };
    class RHICommandBuffer { };
    class RHIDescriptorPool { };
    class RHISemaphore { };
    class RHIFence { };
    class RHIImageView { };
    struct RHIExtent2D {
        uint32_t width;
        uint32_t height;
    };
    struct RHIOffset2D {
        int32_t x;
        int32_t y;
    };
    struct RHIRect2D
    {
        RHIOffset2D offset;
        RHIExtent2D extent;
    };
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        std::optional<uint32_t> m_compute_family;
        
        bool isComplete() { 
            return graphics_family.has_value() && present_family.has_value() && m_compute_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };
}