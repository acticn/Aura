#pragma once
#include <optional>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
#include "../render_type.h"
namespace Aura {
    class RHIQueue { };
    class RHICommandPool { };
    class RHICommandBuffer { };
    class RHIDescriptorPool { };
    class RHISemaphore { };
    class RHIFence { };
    class RHIImageView { };
    class RHIImage { };
    class RHIRenderPass { };
    class RHIFramebuffer { };
    class RHIDescriptorSetLayout { };
    class RHISampler { };
    struct RHIDescriptorSetLayoutBinding
    {
        uint32_t binding;
        RHIDescriptorType descriptorType;
        uint32_t descriptorCount;
        RHIShaderStageFlags stageFlags;
        RHISampler* const* pImmutableSamplers = nullptr;
    };
    struct RHIDescriptorSetLayoutCreateInfo
    {
        RHIStructureType sType;
        const void* pNext;
        RHIDescriptorSetLayoutCreateFlags flags;
        uint32_t bindingCount;
        const RHIDescriptorSetLayoutBinding* pBindings;
    };
    struct RHIAttachmentReference
    {
        uint32_t attachment;
        RHIImageLayout layout;
    };
    struct RHIAttachmentDescription
    {
        RHIAttachmentDescriptionFlags flags;
        RHIFormat format;
        RHISampleCountFlagBits samples;
        RHIAttachmentLoadOp loadOp;
        RHIAttachmentStoreOp storeOp;
        RHIAttachmentLoadOp stencilLoadOp;
        RHIAttachmentStoreOp stencilStoreOp;
        RHIImageLayout initialLayout;
        RHIImageLayout finalLayout;
    };
    struct RHISubpassDescription
    {
        RHISubpassDescriptionFlags flags;
        RHIPipelineBindPoint pipelineBindPoint;
        uint32_t inputAttachmentCount;
        const RHIAttachmentReference* pInputAttachments;
        uint32_t colorAttachmentCount;
        const RHIAttachmentReference* pColorAttachments;
        const RHIAttachmentReference* pResolveAttachments;
        const RHIAttachmentReference* pDepthStencilAttachment;
        uint32_t preserveAttachmentCount;
        const uint32_t* pPreserveAttachments;
    };
    struct RHISubpassDependency
    {
        uint32_t srcSubpass;
        uint32_t dstSubpass;
        RHIPipelineStageFlags srcStageMask;
        RHIPipelineStageFlags dstStageMask;
        RHIAccessFlags srcAccessMask;
        RHIAccessFlags dstAccessMask;
        RHIDependencyFlags dependencyFlags;
    };
    struct RHIFramebufferCreateInfo
    {
        RHIStructureType sType;
        const void* pNext;
        RHIFramebufferCreateFlags flags;
        RHIRenderPass* renderPass;
        uint32_t attachmentCount;
        RHIImageView* const* pAttachments;
        uint32_t width;
        uint32_t height;
        uint32_t layers;
    };
    struct RHIRenderPassCreateInfo
    {
        RHIStructureType sType;
        const void* pNext;
        RHIRenderPassCreateFlags flags;
        uint32_t attachmentCount;
        const RHIAttachmentDescription* pAttachments;
        uint32_t subpassCount;
        const RHISubpassDescription* pSubpasses;
        uint32_t dependencyCount;
        const RHISubpassDependency* pDependencies;
    };
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