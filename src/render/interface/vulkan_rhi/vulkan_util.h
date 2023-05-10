#pragma once
#include "../rhi_struct.h"
#include "vulkan_rhi_resource.h"

namespace Aura
{
    class VulkanUtil {
        public:
        static void createImage(VkPhysicalDevice      physical_device,
                                          VkDevice              device,
                                          uint32_t              image_width,
                                          uint32_t              image_height,
                                          VkFormat              format,
                                          VkImageTiling         image_tiling,
                                          VkImageUsageFlags     image_usage_flags,
                                          VkMemoryPropertyFlags memory_property_flags,
                                          VkImage&              image,
                                          VkDeviceMemory&       memory,
                                          VkImageCreateFlags    image_create_flags,
                                          uint32_t              array_layers,
                                          uint32_t              miplevels);
        static VkImageView createImageView(VkDevice           device,
                                            VkImage&           image,
                                            VkFormat           format,
                                            VkImageAspectFlags image_aspect_flags,
                                            VkImageViewType    view_type,
                                            uint32_t           layout_count,
                                            uint32_t           miplevels);
        static uint32_t    findMemoryType(VkPhysicalDevice      physical_device,
                                        uint32_t              type_filter,
                                        VkMemoryPropertyFlags properties_flag);
    };
} // namespace Aura
