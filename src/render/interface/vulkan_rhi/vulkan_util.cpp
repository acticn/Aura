#include "vulkan_util.h"

#include <iostream>
#define LOG_ERROR(msg) std::cout << "LOG:" << msg << std::endl;
namespace Aura
{
    void VulkanUtil::createImage(VkPhysicalDevice      physical_device,
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
                                 uint32_t              miplevels)
    {
        VkImageCreateInfo image_create_info {};
        image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.flags         = image_create_flags;
        image_create_info.imageType     = VK_IMAGE_TYPE_2D;
        image_create_info.extent.width  = image_width;
        image_create_info.extent.height = image_height;
        image_create_info.extent.depth  = 1;
        image_create_info.mipLevels     = miplevels;
        image_create_info.arrayLayers   = array_layers;
        image_create_info.format        = format;
        image_create_info.tiling        = image_tiling;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.usage         = image_usage_flags;
        image_create_info.samples       = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &image_create_info, nullptr, &image) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create image!");
            return;
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo {};
        allocInfo.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = 
            findMemoryType(physical_device, memRequirements.memoryTypeBits, memory_property_flags);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        {
            LOG_ERROR("failed to allocate image memory!");
            return;
        }

        vkBindImageMemory(device, image, memory, 0);
    }


    VkImageView VulkanUtil::createImageView(VkDevice           device,
                                            VkImage&           image,
                                            VkFormat           format,
                                            VkImageAspectFlags image_aspect_flags,
                                            VkImageViewType    view_type,
                                            uint32_t           layout_count,
                                            uint32_t           miplevels)
    {
        VkImageViewCreateInfo image_view_create_info {};
        image_view_create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image                           = image;
        image_view_create_info.viewType                        = view_type;
        image_view_create_info.format                          = format;
        image_view_create_info.subresourceRange.aspectMask     = image_aspect_flags;
        image_view_create_info.subresourceRange.baseMipLevel   = 0;
        image_view_create_info.subresourceRange.levelCount     = miplevels;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount     = layout_count;

        VkImageView image_view;
        if (vkCreateImageView(device, &image_view_create_info, nullptr, &image_view) != VK_SUCCESS)
        {
            return image_view;
            // todo
        }

        return image_view;
    }

    uint32_t VulkanUtil::findMemoryType(VkPhysicalDevice      physical_device,
                                        uint32_t              type_filter,
                                        VkMemoryPropertyFlags properties_flag)
    {
        VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &physical_device_memory_properties);
        for (uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++)
        {
            if (type_filter & (1 << i) &&
                (physical_device_memory_properties.memoryTypes[i].propertyFlags & properties_flag) == properties_flag)
            {
                return i;
            }
        }
        LOG_ERROR("findMemoryType error");
        return 0;
    }

    void VulkanUtil::createBuffer(VkPhysicalDevice      physical_device,
                                  VkDevice              device,
                                  VkDeviceSize          size,
                                  VkBufferUsageFlags    usage,
                                  VkMemoryPropertyFlags properties,
                                  VkBuffer&             buffer,
                                  VkDeviceMemory&       buffer_memory)
    {
        VkBufferCreateInfo buffer_create_info {};
        buffer_create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_create_info.size        = size;
        buffer_create_info.usage       = usage;                     // use as a vertex/staging/index buffer
        buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // not sharing among queue families

        if (vkCreateBuffer(device, &buffer_create_info, nullptr, &buffer) != VK_SUCCESS)
        {
            LOG_ERROR("vkCreateBuffer failed!");
            return;
        }

        VkMemoryRequirements buffer_memory_requirements; // for allocate_info.allocationSize and
                                                         // allocate_info.memoryTypeIndex
        vkGetBufferMemoryRequirements(device, buffer, &buffer_memory_requirements);

        VkMemoryAllocateInfo buffer_memory_allocate_info {};
        buffer_memory_allocate_info.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        buffer_memory_allocate_info.allocationSize = buffer_memory_requirements.size;
        buffer_memory_allocate_info.memoryTypeIndex =
            VulkanUtil::findMemoryType(physical_device, buffer_memory_requirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &buffer_memory_allocate_info, nullptr, &buffer_memory) != VK_SUCCESS)
        {
            LOG_ERROR("vkAllocateMemory failed!");
            return;
        }

        // bind buffer with buffer memory
        vkBindBufferMemory(device, buffer, buffer_memory, 0); // offset = 0
    }
    
    void VulkanUtil::copyBuffer(RHI*         rhi,
                                VkBuffer     srcBuffer,
                                VkBuffer     dstBuffer,
                                VkDeviceSize srcOffset,
                                VkDeviceSize dstOffset,
                                VkDeviceSize size)
    {
        if (rhi == nullptr)
        {
            LOG_ERROR("rhi is nullptr");
            return;
        }

        RHICommandBuffer* rhi_command_buffer = static_cast<VulkanRHI*>(rhi)->beginSingleTimeCommands();
        VkCommandBuffer command_buffer = ((VulkanCommandBuffer*)rhi_command_buffer)->getResource();

        VkBufferCopy copyRegion = {srcOffset, dstOffset, size};
        vkCmdCopyBuffer(command_buffer, srcBuffer, dstBuffer, 1, &copyRegion);

        static_cast<VulkanRHI*>(rhi)->endSingleTimeCommands(rhi_command_buffer);
    }
} // namespace Aura
