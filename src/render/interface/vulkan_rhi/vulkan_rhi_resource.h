#pragma once

#include "../rhi_struct.h"
#include <vulkan/vulkan.h>
namespace Aura
{
    class VulkanQueue : public RHIQueue
    {
    public:
        void setResource(VkQueue res)
        {
            m_resource = res;
        }
        VkQueue getResource() const
        {
            return m_resource;
        }
    private:
        VkQueue m_resource;
    };

    class VulkanCommandPool : public RHICommandPool
    {
    public:
        void setResource(VkCommandPool res)
        {
            m_resource = res;
        }
        VkCommandPool getResource() const
        {
            return m_resource;
        }
    private:
        VkCommandPool m_resource;
    };

    class VulkanCommandBuffer : public RHICommandBuffer
    {
    public:
        void setResource(VkCommandBuffer res)
        {
            m_resource = res;
        }
        const VkCommandBuffer getResource() const
        {
            return m_resource;
        }
    private:
        VkCommandBuffer m_resource;
    };

    class VulkanDescriptorPool : public RHIDescriptorPool
    {
    public:
        void setResource(VkDescriptorPool res)
        {
            m_resource = res;
        }
        VkDescriptorPool getResource() const
        {
            return m_resource;
        }
    private:
        VkDescriptorPool m_resource;
    };

    class VulkanSemaphore : public RHISemaphore
    {
    public:
        void setResource(VkSemaphore res)
        {
            m_resource = res;
        }
        VkSemaphore &getResource()
        {
            return m_resource;
        }
    private:
        VkSemaphore m_resource;
    };

    class VulkanFence : public RHIFence
    {
    public:
        void setResource(VkFence res)
        {
            m_resource = res;
        }
        VkFence getResource() const
        {
            return m_resource;
        }
    private:
        VkFence m_resource;
    };

    class VulkanImageView : public RHIImageView
    {
    public:
        void setResource(VkImageView res)
        {
            m_resource = res;
        }
        VkImageView getResource() const
        {
            return m_resource;
        }
    private:
        VkImageView m_resource;
    };

    class VulkanImage : public RHIImage
    {
    public:
        void setResource(VkImage res)
        {
            m_resource = res;
        }
        VkImage &getResource()
        {
            return m_resource;
        }
    private:
        VkImage m_resource;
    };
} // namespace Aura
