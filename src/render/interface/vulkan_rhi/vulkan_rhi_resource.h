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
} // namespace Aura
