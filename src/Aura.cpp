#include "Aura.h"

namespace Aura {
    void Aura::run() {
        
        rhi = new VulkanRHI();
        rhi->initialize();
        rhi->render_test();
        initialize();
        mainLoop();

        
    }

    void Aura::initialize() {
        
        setupRenderPass();
        setupFrameBuffers();
        setupDescriptorSetLayout();
    }
    void Aura::setupRenderPass() {
        
        RHIAttachmentDescription colorAttachment{};
        
        colorAttachment.format = rhi->m_swapchain_image_format;
        
        colorAttachment.samples = RHI_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = RHI_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = RHI_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = RHI_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = RHI_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = RHI_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        RHIAttachmentDescription depthAttachment{};
        depthAttachment.format = rhi->m_depth_image_format;
        depthAttachment.samples = RHI_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = RHI_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = RHI_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = RHI_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = RHI_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = RHI_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
        RHIAttachmentDescription attachments[] = {colorAttachment, depthAttachment};
        


        RHIAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        RHIAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;



        RHISubpassDescription subpass{};
        subpass.pipelineBindPoint = RHI_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        RHISubpassDescription subpasses[] = {subpass};

        RHISubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = RHI_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | RHI_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        RHISubpassDependency dependencies[] = {dependency};
        
        RHIRenderPassCreateInfo renderpass_create_info {};
        renderpass_create_info.sType = RHI_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderpass_create_info.attachmentCount = (sizeof(attachments) / sizeof(attachments[0]));
        renderpass_create_info.pAttachments    = attachments;
        renderpass_create_info.subpassCount    = (sizeof(subpasses) / sizeof(subpasses[0]));
        renderpass_create_info.pSubpasses      = subpasses;
        renderpass_create_info.dependencyCount = (sizeof(dependencies) / sizeof(dependencies[0]));
        renderpass_create_info.pDependencies   = dependencies;
        
        if (rhi->createRenderPass(&renderpass_create_info, renderpass) != RHI_SUCCESS) {
            throw std::runtime_error("failed to create render pass");
        }
    }
    void Aura::mainLoop() {
        while (!glfwWindowShouldClose(rhi->m_window)) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(rhi->m_device);
    }

    void Aura::drawFrame() {
            rhi->waitForFences();
            rhi->prepareBeforePass();


    }

    void Aura::setupFrameBuffers() {
        const std::vector<RHIImageView*>& imageViews = rhi->m_swapchain_imageviews;
        framebuffers.resize(imageViews.size());
        for (size_t i = 0; i < framebuffers.size(); i++) { 
            RHIImageView* attachments[2] = { imageViews[i], rhi->m_depth_image_view};

            RHIFramebufferCreateInfo framebuffer_create_info{};
            framebuffer_create_info.sType = RHI_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_create_info.renderPass = renderpass;
            framebuffer_create_info.attachmentCount = sizeof(attachments) / sizeof(attachments[0]);
            framebuffer_create_info.pAttachments = attachments;
            framebuffer_create_info.width = rhi->m_swapchain_extent.width;
            framebuffer_create_info.height = rhi->m_swapchain_extent.height;
            framebuffer_create_info.layers = 1;

            if (rhi->createFramebuffer(&framebuffer_create_info, framebuffers[i]) !=RHI_SUCCESS) {
                throw std::runtime_error("create inefficient pick framebuffer");
            }
        }
    }

    void Aura::setupDescriptorSetLayout() {
        RHIDescriptorSetLayoutBinding layoutBinding[2];
        layoutBinding[0].binding = 0;
        layoutBinding[0].descriptorType = RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding[0].descriptorCount = 1;
        layoutBinding[0].stageFlags = RHI_SHADER_STAGE_VERTEX_BIT;
        layoutBinding[0].pImmutableSamplers = nullptr;


        layoutBinding[1].binding = 2;
        layoutBinding[1].descriptorType = RHI_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layoutBinding[1].descriptorCount = 1;
        layoutBinding[1].stageFlags = RHI_SHADER_STAGE_FRAGMENT_BIT;
        layoutBinding[1].pImmutableSamplers = nullptr;

        RHIDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = RHI_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 2;
        layoutInfo.pBindings = layoutBinding;

        if (rhi->createDescriptorSetLayout(&layoutInfo, layout) != RHI_SUCCESS)
        {
            throw std::runtime_error("create debug draw layout");
        }
    }

    void Aura::setupDescriptorSet() {
        // std::vector<RHIDescriptorSetLayout> layouts(2, *layout);

        // RHIDescriptorSetAllocateInfo allocateInfo;
        // if (RHI_SUCCESS != rhi->allocateDescriptorSets(&allocateInfo, ))
        // {
        //     throw std::runtime_error("allocate post process global descriptor set");
        // }
    }

    void Aura::setupVertexBuffer() {
        
    }
} 