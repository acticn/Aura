#include "vulkan_rhi.h"

#define LOG_ERROR(msg) std::cout << "LOG:" << msg << std::endl;

namespace Aura {
    void VulkanRHI::initialize() {
        #ifdef NDEBUG
        m_enable_validation_Layers = false;
        #else
        m_enable_validation_Layers = true;
        #endif
        initWindow();
        createInstance();
        initializeDebugMessenger();
        createWindowSurface();
        initializePhysicalDevice();
        createLogicalDevice();
        createCommandPool();
        createCommandBuffers();
        createDescriptorPool();
        createSyncPrimitives();
        createSwapchain();
        createSwapchainImageViews();
        createFramebufferImageAndView();
        createAssetAllocator();
        std::cout << "initialized" << std::endl;

    }
    void VulkanRHI::initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
    }

    void VulkanRHI::createInstance() {
        if (m_enable_validation_Layers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (m_enable_validation_Layers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }
    VkResult VulkanRHI::createDebugUtilsMessengerEXT(VkInstance                                instance,
                                                     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                     const VkAllocationCallbacks*              pAllocator,
                                                     VkDebugUtilsMessengerEXT*                 pDebugMessenger)
    {
        auto func =
            (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }
    void VulkanRHI::initializeDebugMessenger() {
        if (m_enable_validation_Layers)
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            populateDebugMessengerCreateInfo(createInfo);
            if (VK_SUCCESS != createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debug_messenger))
            {
                std::cout << "LOG:" << "failed to set up debug messenger!" << std::endl;
            }
        }
    }
    void VulkanRHI::render_test() {
        std::cout << "rendered" << std::endl;
    }
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }
    void VulkanRHI::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }
    
    std::vector<const char*> VulkanRHI::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (m_enable_validation_Layers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }
    bool VulkanRHI::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }
    void VulkanRHI::createWindowSurface() {

        if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            std::cout << "LOG:" << "glfwCreateWindowSurface failed!" << std::endl;
        }
    } 
    void VulkanRHI::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<VulkanRHI*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
    

    void VulkanRHI::initializePhysicalDevice() {
        uint32_t physical_device_count;
        vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);
        if (physical_device_count == 0)
        {
            LOG_ERROR("enumerate physical devices failed!");
        }
        else
        {
            // find one device that matches our requirement
            // or find which is the best
            std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
            vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());

            std::vector<std::pair<int, VkPhysicalDevice>> ranked_physical_devices;
            for (const auto& device : physical_devices)
            {
                VkPhysicalDeviceProperties physical_device_properties;
                vkGetPhysicalDeviceProperties(device, &physical_device_properties);
                int score = 0;

                if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                {
                    score += 1000;
                }
                else if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
                {
                    score += 100;
                }

                ranked_physical_devices.push_back({score, device});
            }

            std::sort(ranked_physical_devices.begin(),
                      ranked_physical_devices.end(),
                      [](const std::pair<int, VkPhysicalDevice>& p1, const std::pair<int, VkPhysicalDevice>& p2) {
                          return p1 > p2;
                      });

            for (const auto& device : ranked_physical_devices)
            {
                if (isDeviceSuitable(device.second))
                {
                    m_physical_device = device.second;
                    break;
                }
            }

            if (m_physical_device == VK_NULL_HANDLE)
            {
                LOG_ERROR("failed to find suitable physical device");
            }
        }
    }

    bool VulkanRHI::isDeviceSuitable(VkPhysicalDevice physicalm_device) {
        auto queue_indices           = findQueueFamilies(physicalm_device);
        bool is_extensions_supported = checkDeviceExtensionSupport(physicalm_device);
        bool is_swapchain_adequate   = false;
        if (is_extensions_supported)
        {
            SwapChainSupportDetails swapchain_support_details = querySwapChainSupport(physicalm_device);
            is_swapchain_adequate =
                !swapchain_support_details.formats.empty() && !swapchain_support_details.presentModes.empty();
        }

        VkPhysicalDeviceFeatures physicalm_device_features;
        vkGetPhysicalDeviceFeatures(physicalm_device, &physicalm_device_features);

        if (!queue_indices.isComplete() || !is_swapchain_adequate || !physicalm_device_features.samplerAnisotropy)
        {
            return false;
        }

        return true;
    }

    QueueFamilyIndices VulkanRHI::findQueueFamilies(VkPhysicalDevice physicalm_device) // for device and surface
    {
        QueueFamilyIndices indices;
        uint32_t           queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalm_device, &queue_family_count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalm_device, &queue_family_count, queue_families.data());

        for (auto queue_family_properties : queue_families)
        {
            std::cout << "Supported queue types:" << std::endl;

            if (queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
            std::cout << "- Graphics" << std::endl;
            }

            if (queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
            std::cout << "- Compute" << std::endl;
            }

            if (queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
            std::cout << "- Transfer" << std::endl;
            }

            if (queue_family_properties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
            {
            std::cout << "- Sparse Binding" << std::endl;
            } 
        }
        std::cout << "*****"<< std::endl;
        int i = 0;
        for (const auto& queue_family : queue_families)
        {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) // if support graphics command queue
            {
                indices.graphics_family = i;
            }

            if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT) // if support compute command queue
            {
                indices.m_compute_family = i;
            }


            VkBool32 is_present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalm_device,
                                                 i,
                                                 m_surface,
                                                 &is_present_support); // if support surface presentation
            if (is_present_support)
            {
                indices.present_family = i;
            }

            if (indices.isComplete())
            {
                break;
            }
            i++;
        }
        return indices;
    }

    bool VulkanRHI::checkDeviceExtensionSupport(VkPhysicalDevice physicalm_device)
    {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(physicalm_device, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(physicalm_device, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(m_device_extensions.begin(), m_device_extensions.end());
        for (const auto& extension : available_extensions)
        {
            required_extensions.erase(extension.extensionName);
        }

        return required_extensions.empty();
    }

    SwapChainSupportDetails VulkanRHI::querySwapChainSupport(VkPhysicalDevice physicalm_device)
    {
        SwapChainSupportDetails details_result;

        // capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalm_device, m_surface, &details_result.capabilities);

        // formats
        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalm_device, m_surface, &format_count, nullptr);
        if (format_count != 0)
        {
            details_result.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                physicalm_device, m_surface, &format_count, details_result.formats.data());
        }

        // present modes
        uint32_t presentmode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalm_device, m_surface, &presentmode_count, nullptr);
        if (presentmode_count != 0)
        {
            details_result.presentModes.resize(presentmode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                physicalm_device, m_surface, &presentmode_count, details_result.presentModes.data());
        }

        return details_result;
    }

    void VulkanRHI::createLogicalDevice()
    {
        m_queue_indices = findQueueFamilies(m_physical_device);

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos; // all queues that need to be created
        std::set<uint32_t>                   queue_families = {m_queue_indices.graphics_family.value(),
                                             m_queue_indices.present_family.value(),
                                             m_queue_indices.m_compute_family.value()};

        float queue_priority = 1.0f;
        for (uint32_t queue_family : queue_families) // for every queue family
        {
            // queue create info
            VkDeviceQueueCreateInfo queue_create_info {};
            queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount       = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        // physical device features
        VkPhysicalDeviceFeatures physical_device_features = {};

        physical_device_features.samplerAnisotropy = VK_TRUE;

        // support inefficient readback storage buffer
        physical_device_features.fragmentStoresAndAtomics = VK_TRUE;

        // support independent blending
        physical_device_features.independentBlend = VK_TRUE;

        // support geometry shader
        if (m_enable_point_light_shadow)
        {
            physical_device_features.geometryShader = VK_TRUE;
        }

        // device create info
        VkDeviceCreateInfo device_create_info {};
        device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos       = queue_create_infos.data();
        device_create_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size());
        device_create_info.pEnabledFeatures        = &physical_device_features;
        device_create_info.enabledExtensionCount   = static_cast<uint32_t>(m_device_extensions.size());
        device_create_info.ppEnabledExtensionNames = m_device_extensions.data();
        device_create_info.enabledLayerCount       = 0;

        if (vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device) != VK_SUCCESS)
        {
            LOG_ERROR("vk create device");
        }

        // initialize queues of this device
        VkQueue vk_graphics_queue;
        vkGetDeviceQueue(m_device, m_queue_indices.graphics_family.value(), 0, &vk_graphics_queue);
        m_graphics_queue = new VulkanQueue();
        ((VulkanQueue*)m_graphics_queue)->setResource(vk_graphics_queue);

        vkGetDeviceQueue(m_device, m_queue_indices.present_family.value(), 0, &m_present_queue);

        VkQueue vk_compute_queue;
        vkGetDeviceQueue(m_device, m_queue_indices.m_compute_family.value(), 0, &vk_compute_queue);
        m_compute_queue = new VulkanQueue();
        ((VulkanQueue*)m_compute_queue)->setResource(vk_compute_queue);

        std::cout << "queues:" << std::endl;
        std::cout << vk_graphics_queue << std::endl;
        std::cout << m_present_queue << std::endl;
        std::cout << vk_compute_queue << std::endl;
        // more efficient pointer
        _vkResetCommandPool      = (PFN_vkResetCommandPool)vkGetDeviceProcAddr(m_device, "vkResetCommandPool");
        _vkBeginCommandBuffer    = (PFN_vkBeginCommandBuffer)vkGetDeviceProcAddr(m_device, "vkBeginCommandBuffer");
        _vkEndCommandBuffer      = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(m_device, "vkEndCommandBuffer");
        _vkCmdBeginRenderPass    = (PFN_vkCmdBeginRenderPass)vkGetDeviceProcAddr(m_device, "vkCmdBeginRenderPass");
        _vkCmdNextSubpass        = (PFN_vkCmdNextSubpass)vkGetDeviceProcAddr(m_device, "vkCmdNextSubpass");
        _vkCmdEndRenderPass      = (PFN_vkCmdEndRenderPass)vkGetDeviceProcAddr(m_device, "vkCmdEndRenderPass");
        _vkCmdBindPipeline       = (PFN_vkCmdBindPipeline)vkGetDeviceProcAddr(m_device, "vkCmdBindPipeline");
        _vkCmdSetViewport        = (PFN_vkCmdSetViewport)vkGetDeviceProcAddr(m_device, "vkCmdSetViewport");
        _vkCmdSetScissor         = (PFN_vkCmdSetScissor)vkGetDeviceProcAddr(m_device, "vkCmdSetScissor");
        _vkWaitForFences         = (PFN_vkWaitForFences)vkGetDeviceProcAddr(m_device, "vkWaitForFences");
        _vkResetFences           = (PFN_vkResetFences)vkGetDeviceProcAddr(m_device, "vkResetFences");
        _vkCmdDrawIndexed        = (PFN_vkCmdDrawIndexed)vkGetDeviceProcAddr(m_device, "vkCmdDrawIndexed");
        _vkCmdBindVertexBuffers  = (PFN_vkCmdBindVertexBuffers)vkGetDeviceProcAddr(m_device, "vkCmdBindVertexBuffers");
        _vkCmdBindIndexBuffer    = (PFN_vkCmdBindIndexBuffer)vkGetDeviceProcAddr(m_device, "vkCmdBindIndexBuffer");
        _vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)vkGetDeviceProcAddr(m_device, "vkCmdBindDescriptorSets");
        _vkCmdClearAttachments   = (PFN_vkCmdClearAttachments)vkGetDeviceProcAddr(m_device, "vkCmdClearAttachments");

        m_depth_image_format = (RHIFormat)findDepthFormat();
    }
    VkFormat VulkanRHI::findDepthFormat() {
        return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                   VK_IMAGE_TILING_OPTIMAL,
                                   VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
    VkFormat VulkanRHI::findSupportedFormat(const std::vector<VkFormat>& candidates,
                                            VkImageTiling                tiling,
                                            VkFormatFeatureFlags         features)
    {
        for (VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_physical_device, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        LOG_ERROR("findSupportedFormat failed");
        return VkFormat();
    }

    void VulkanRHI::createCommandPool()
    {
        // default graphics command pool
        {
            m_rhi_command_pool = new VulkanCommandPool();
            VkCommandPool vk_command_pool;
            VkCommandPoolCreateInfo command_pool_create_info {};
            command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            command_pool_create_info.pNext            = NULL;
            command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            command_pool_create_info.queueFamilyIndex = m_queue_indices.graphics_family.value();

            if (vkCreateCommandPool(m_device, &command_pool_create_info, nullptr, &vk_command_pool) != VK_SUCCESS)
            {
                LOG_ERROR("vk create command pool");
            }

            ((VulkanCommandPool*)m_rhi_command_pool)->setResource(vk_command_pool);
        }

        // other command pools
        {
            VkCommandPoolCreateInfo command_pool_create_info;
            command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            command_pool_create_info.pNext            = NULL;
            command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            command_pool_create_info.queueFamilyIndex = m_queue_indices.graphics_family.value();

            for (uint32_t i = 0; i < k_max_frames_in_flight; ++i)
            {
                if (vkCreateCommandPool(m_device, &command_pool_create_info, NULL, &m_command_pools[i]) != VK_SUCCESS)
                {
                    LOG_ERROR("vk create command pool");
                }
            }
        }
        LOG_ERROR("SUCCESS:vk create command pool ");
    }

    void VulkanRHI::createCommandBuffers()
    {
        VkCommandBufferAllocateInfo command_buffer_allocate_info {};
        command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = 1U;

        for (uint32_t i = 0; i < k_max_frames_in_flight; ++i)
        {
            command_buffer_allocate_info.commandPool = m_command_pools[i];
            VkCommandBuffer vk_command_buffer;
            if (vkAllocateCommandBuffers(m_device, &command_buffer_allocate_info, &vk_command_buffer) != VK_SUCCESS)
            {
                LOG_ERROR("vk allocate command buffers");
            }
            m_vk_command_buffers[i] = vk_command_buffer;
            m_command_buffers[i] = new VulkanCommandBuffer();
            ((VulkanCommandBuffer*)m_command_buffers[i])->setResource(vk_command_buffer);
        }
    }

    void VulkanRHI::createDescriptorPool()
    {
        // Since DescriptorSet should be treated as asset in Vulkan, DescriptorPool
        // should be big enough, and thus we can sub-allocate DescriptorSet from
        // DescriptorPool merely as we sub-allocate Buffer/Image from DeviceMemory.

        VkDescriptorPoolSize pool_sizes[7];
        pool_sizes[0].type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        pool_sizes[0].descriptorCount = 3 + 2 + 2 + 2 + 1 + 1 + 3 + 3;
        pool_sizes[1].type            = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        pool_sizes[1].descriptorCount = 1 + 1 + 1 * m_max_vertex_blending_mesh_count;
        pool_sizes[2].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pool_sizes[2].descriptorCount = 1 * m_max_material_count;
        pool_sizes[3].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[3].descriptorCount = 3 + 5 * m_max_material_count + 1 + 1; // ImGui_ImplVulkan_CreateDeviceObjects
        pool_sizes[4].type            = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        pool_sizes[4].descriptorCount = 4 + 1 + 1 + 2;
        pool_sizes[5].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        pool_sizes[5].descriptorCount = 3;
        pool_sizes[6].type            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        pool_sizes[6].descriptorCount = 1;

        VkDescriptorPoolCreateInfo pool_info {};
        pool_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.poolSizeCount = sizeof(pool_sizes) / sizeof(pool_sizes[0]);
        pool_info.pPoolSizes    = pool_sizes;
        pool_info.maxSets =
            1 + 1 + 1 + m_max_material_count + m_max_vertex_blending_mesh_count + 1 + 1; // +skybox + axis descriptor set
        pool_info.flags = 0U;

        if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_vk_descriptor_pool) != VK_SUCCESS)
        {
            LOG_ERROR("create descriptor pool");
        }

        m_descriptor_pool = new VulkanDescriptorPool();
        ((VulkanDescriptorPool*)m_descriptor_pool)->setResource(m_vk_descriptor_pool);
    }


    void VulkanRHI::createSyncPrimitives()
    {
        VkSemaphoreCreateInfo semaphore_create_info {};
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_create_info {};
        fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // the fence is initialized as signaled

        for (uint32_t i = 0; i < k_max_frames_in_flight; i++)
        {
            m_image_available_for_texturescopy_semaphores[i] = new VulkanSemaphore();
            if (vkCreateSemaphore(
                    m_device, &semaphore_create_info, nullptr, &m_image_available_for_render_semaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateSemaphore(
                    m_device, &semaphore_create_info, nullptr, &m_image_finished_for_presentation_semaphores[i]) !=
                    VK_SUCCESS ||
                vkCreateSemaphore(
                    m_device, &semaphore_create_info, nullptr, &(((VulkanSemaphore*)m_image_available_for_texturescopy_semaphores[i])->getResource())) !=
                    VK_SUCCESS ||
                vkCreateFence(m_device, &fence_create_info, nullptr, &m_is_frame_in_flight_fences[i]) != VK_SUCCESS)
            {
                LOG_ERROR("vk create semaphore & fence");
            }

            m_rhi_is_frame_in_flight_fences[i] = new VulkanFence();
            ((VulkanFence*)m_rhi_is_frame_in_flight_fences[i])->setResource(m_is_frame_in_flight_fences[i]);
        }
    }

    void VulkanRHI::createSwapchain()
    {
        // query all supports of this physical device
        SwapChainSupportDetails swapchain_support_details = querySwapChainSupport(m_physical_device);

        // choose the best or fitting format
        VkSurfaceFormatKHR chosen_surface_format =
            chooseSwapchainSurfaceFormatFromDetails(swapchain_support_details.formats);
        // choose the best or fitting present mode
        VkPresentModeKHR chosen_presentMode =
            chooseSwapchainPresentModeFromDetails(swapchain_support_details.presentModes);
        // choose the best or fitting extent
        VkExtent2D chosen_extent = chooseSwapchainExtentFromDetails(swapchain_support_details.capabilities);

        uint32_t image_count = swapchain_support_details.capabilities.minImageCount + 1;
        if (swapchain_support_details.capabilities.maxImageCount > 0 &&
            image_count > swapchain_support_details.capabilities.maxImageCount)
        {
            image_count = swapchain_support_details.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo {};
        createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;

        createInfo.minImageCount    = image_count;
        createInfo.imageFormat      = chosen_surface_format.format;
        createInfo.imageColorSpace  = chosen_surface_format.colorSpace;
        createInfo.imageExtent      = chosen_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {m_queue_indices.graphics_family.value(), m_queue_indices.present_family.value()};

        if (m_queue_indices.graphics_family != m_queue_indices.present_family)
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform   = swapchain_support_details.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode    = chosen_presentMode;
        createInfo.clipped        = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
        {
            LOG_ERROR("vk create swapchain khr");
        }

        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
        m_swapchain_images.resize(image_count);
        vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data());

        m_swapchain_image_format = (RHIFormat)chosen_surface_format.format;
        m_swapchain_extent.height = chosen_extent.height;
        m_swapchain_extent.width = chosen_extent.width;

        m_scissor = {{0, 0}, {m_swapchain_extent.width, m_swapchain_extent.height}};
    }

    VkSurfaceFormatKHR
    VulkanRHI::chooseSwapchainSurfaceFormatFromDetails(const std::vector<VkSurfaceFormatKHR>& available_surface_formats)
    {
        for (const auto& surface_format : available_surface_formats)
        {
            // TODO: select the VK_FORMAT_B8G8R8A8_SRGB surface format,
            // there is no need to do gamma correction in the fragment shader
            if (surface_format.format == VK_FORMAT_B8G8R8A8_UNORM &&
                surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return surface_format;
            }
        }
        return available_surface_formats[0];
    }

    VkPresentModeKHR
    VulkanRHI::chooseSwapchainPresentModeFromDetails(const std::vector<VkPresentModeKHR>& available_present_modes)
    {
        for (VkPresentModeKHR present_mode : available_present_modes)
        {
            if (VK_PRESENT_MODE_MAILBOX_KHR == present_mode)
            {
                return VK_PRESENT_MODE_MAILBOX_KHR;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanRHI::chooseSwapchainExtentFromDetails(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);

            VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

            actualExtent.width =
                std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height =
                std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void VulkanRHI::createSwapchainImageViews()
    {
        m_swapchain_imageviews.resize(m_swapchain_images.size());

        // create imageview (one for each this time) for all swapchain images
        for (size_t i = 0; i < m_swapchain_images.size(); i++)
        {
            VkImageView vk_image_view;
            vk_image_view = VulkanUtil::createImageView(m_device,
                                                                   m_swapchain_images[i],
                                                                   (VkFormat)m_swapchain_image_format,
                                                                   VK_IMAGE_ASPECT_COLOR_BIT,
                                                                   VK_IMAGE_VIEW_TYPE_2D,
                                                                   1,
                                                                   1);
            m_swapchain_imageviews[i] = new VulkanImageView();
            ((VulkanImageView*)m_swapchain_imageviews[i])->setResource(vk_image_view);
        }
    }

    void VulkanRHI::createFramebufferImageAndView()
    {
        VulkanUtil::createImage(m_physical_device,
                                m_device,
                                m_swapchain_extent.width,
                                m_swapchain_extent.height,
                                (VkFormat)m_depth_image_format,
                                VK_IMAGE_TILING_OPTIMAL,
                                VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                                VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                ((VulkanImage*)m_depth_image)->getResource(),
                                m_depth_image_memory,
                                0,
                                1,
                                1);

        ((VulkanImageView*)m_depth_image_view)->setResource(
            VulkanUtil::createImageView(m_device, ((VulkanImage*)m_depth_image)->getResource(), (VkFormat)m_depth_image_format, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_VIEW_TYPE_2D, 1, 1));
    }
    
    void VulkanRHI::createAssetAllocator()
    {
        VmaVulkanFunctions vulkanFunctions    = {};
        vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr   = &vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.vulkanApiVersion       = m_vulkan_api_version;
        allocatorCreateInfo.physicalDevice         = m_physical_device;
        allocatorCreateInfo.device                 = m_device;
        allocatorCreateInfo.instance               = m_instance;
        allocatorCreateInfo.pVulkanFunctions       = &vulkanFunctions;

        vmaCreateAllocator(&allocatorCreateInfo, &m_assets_allocator);
    }

    void VulkanRHI::waitForFences() {
        VkResult res_wait_for_fences =
            _vkWaitForFences(m_device, 1, &m_is_frame_in_flight_fences[m_current_frame_index], VK_TRUE, UINT64_MAX);
        if (VK_SUCCESS != res_wait_for_fences)
        {
            LOG_ERROR("failed to synchronize!");
        }
    }

    void VulkanRHI::prepareBeforePass() {
        VkResult acquire_image_result =
            vkAcquireNextImageKHR(m_device,
                                  m_swapchain,
                                  UINT64_MAX,
                                  m_image_available_for_render_semaphores[m_current_frame_index],
                                  VK_NULL_HANDLE,
                                  &m_current_swapchain_image_index);

        if (acquire_image_result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } 
        else if (acquire_image_result != VK_SUCCESS && acquire_image_result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        std::cout << "image success" << std::endl;
        vkResetFences(m_device, 1, &m_is_frame_in_flight_fences[m_current_frame_index]);
        //vkResetCommandBuffer(m_vk_command_buffers[m_current_frame_index], /*VkCommandBufferResetFlagBits*/ 0);
        VkSemaphore signalSemaphores[] = { m_image_finished_for_presentation_semaphores[m_current_frame_index] };

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_swapchain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &m_current_swapchain_image_index;

        // 提交显示
        VkResult result = vkQueuePresentKHR(m_present_queue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
        m_current_frame_index = (m_current_frame_index + 1) % 2;
    }

    void VulkanRHI::recreateSwapChain() {
        int width  = 0;
        int height = 0;
        glfwGetFramebufferSize(m_window, &width, &height);
        while (width == 0 || height == 0) // minimized 0,0, pause for now
        {
            glfwGetFramebufferSize(m_window, &width, &height);
            glfwWaitEvents();
        }

        VkResult res_wait_for_fences =
            _vkWaitForFences(m_device, k_max_frames_in_flight, m_is_frame_in_flight_fences, VK_TRUE, UINT64_MAX);
        if (VK_SUCCESS != res_wait_for_fences)
        {
            LOG_ERROR("_vkWaitForFences failed");
            return;
        }

        destroyImageView(m_depth_image_view);
        vkDestroyImage(m_device, ((VulkanImage*)m_depth_image)->getResource(), NULL);
        vkFreeMemory(m_device, m_depth_image_memory, NULL);

        for (auto imageview : m_swapchain_imageviews)
        {
            vkDestroyImageView(m_device, ((VulkanImageView*)imageview)->getResource(), NULL);
        }
        vkDestroySwapchainKHR(m_device, m_swapchain, NULL);

        createSwapchain();
        createSwapchainImageViews();
        createFramebufferImageAndView();
    }

    void VulkanRHI::destroyImageView(RHIImageView* imageView) {
        vkDestroyImageView(m_device, ((VulkanImageView*)imageView)->getResource(), nullptr);
    }

    bool VulkanRHI::createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass* &pRenderPass)
    {
        // attachment convert
        std::vector<VkAttachmentDescription> vk_attachments(pCreateInfo->attachmentCount);
        for (uint32_t i = 0; i < pCreateInfo->attachmentCount; ++i)
        {
            const auto& rhi_desc = pCreateInfo->pAttachments[i];
            auto& vk_desc = vk_attachments[i];

            vk_desc.flags = (VkAttachmentDescriptionFlags)(rhi_desc).flags;
            vk_desc.format = (VkFormat)(rhi_desc).format;
            vk_desc.samples = (VkSampleCountFlagBits)(rhi_desc).samples;
            vk_desc.loadOp = (VkAttachmentLoadOp)(rhi_desc).loadOp;
            vk_desc.storeOp = (VkAttachmentStoreOp)(rhi_desc).storeOp;
            vk_desc.stencilLoadOp = (VkAttachmentLoadOp)(rhi_desc).stencilLoadOp;
            vk_desc.stencilStoreOp = (VkAttachmentStoreOp)(rhi_desc).stencilStoreOp;
            vk_desc.initialLayout = (VkImageLayout)(rhi_desc).initialLayout;
            vk_desc.finalLayout = (VkImageLayout)(rhi_desc).finalLayout;
        };

        // subpass convert
        int totalAttachmentRefenrence = 0;
        for (uint32_t i = 0; i < pCreateInfo->subpassCount; i++)
        {
            const auto& rhi_desc = pCreateInfo->pSubpasses[i];
            totalAttachmentRefenrence += rhi_desc.inputAttachmentCount; // pInputAttachments
            totalAttachmentRefenrence += rhi_desc.colorAttachmentCount; // pColorAttachments
            if (rhi_desc.pDepthStencilAttachment != nullptr)
            {
                totalAttachmentRefenrence += rhi_desc.colorAttachmentCount; // pDepthStencilAttachment
            }
            if (rhi_desc.pResolveAttachments != nullptr)
            {
                totalAttachmentRefenrence += rhi_desc.colorAttachmentCount; // pResolveAttachments
            }
        }
        std::vector<VkSubpassDescription> vk_subpass_description(pCreateInfo->subpassCount);
        std::vector<VkAttachmentReference> vk_attachment_reference(totalAttachmentRefenrence);
        int currentAttachmentRefence = 0;
        for (uint32_t i = 0; i < pCreateInfo->subpassCount; ++i)
        {
            const auto& rhi_desc = pCreateInfo->pSubpasses[i];
            auto& vk_desc = vk_subpass_description[i];

            vk_desc.flags = (VkSubpassDescriptionFlags)(rhi_desc).flags;
            vk_desc.pipelineBindPoint = (VkPipelineBindPoint)(rhi_desc).pipelineBindPoint;
            vk_desc.preserveAttachmentCount = (rhi_desc).preserveAttachmentCount;
            vk_desc.pPreserveAttachments = (const uint32_t*)(rhi_desc).pPreserveAttachments;

            vk_desc.inputAttachmentCount = (rhi_desc).inputAttachmentCount;
            vk_desc.pInputAttachments = &vk_attachment_reference[currentAttachmentRefence];
            for (uint32_t i = 0; i < (rhi_desc).inputAttachmentCount; i++)
            {
                const auto& rhi_attachment_refence_input = (rhi_desc).pInputAttachments[i];
                auto& vk_attachment_refence_input = vk_attachment_reference[currentAttachmentRefence];

                vk_attachment_refence_input.attachment = rhi_attachment_refence_input.attachment;
                vk_attachment_refence_input.layout = (VkImageLayout)(rhi_attachment_refence_input.layout);

                currentAttachmentRefence += 1;
            };

            vk_desc.colorAttachmentCount = (rhi_desc).colorAttachmentCount;
            vk_desc.pColorAttachments = &vk_attachment_reference[currentAttachmentRefence];
            for (uint32_t i = 0; i < (rhi_desc).colorAttachmentCount; ++i)
            {
                const auto& rhi_attachment_refence_color = (rhi_desc).pColorAttachments[i];
                auto& vk_attachment_refence_color = vk_attachment_reference[currentAttachmentRefence];

                vk_attachment_refence_color.attachment = rhi_attachment_refence_color.attachment;
                vk_attachment_refence_color.layout = (VkImageLayout)(rhi_attachment_refence_color.layout);

                currentAttachmentRefence += 1;
            };

            if (rhi_desc.pResolveAttachments != nullptr)
            {
                vk_desc.pResolveAttachments = &vk_attachment_reference[currentAttachmentRefence];
                for (uint32_t i = 0; i < (rhi_desc).colorAttachmentCount; ++i)
                {
                    const auto& rhi_attachment_refence_resolve = (rhi_desc).pResolveAttachments[i];
                    auto& vk_attachment_refence_resolve = vk_attachment_reference[currentAttachmentRefence];

                    vk_attachment_refence_resolve.attachment = rhi_attachment_refence_resolve.attachment;
                    vk_attachment_refence_resolve.layout = (VkImageLayout)(rhi_attachment_refence_resolve.layout);

                    currentAttachmentRefence += 1;
                };
            }

            if (rhi_desc.pDepthStencilAttachment != nullptr)
            {
                vk_desc.pDepthStencilAttachment = &vk_attachment_reference[currentAttachmentRefence];
                for (uint32_t i = 0; i < (rhi_desc).colorAttachmentCount; ++i)
                {
                    const auto& rhi_attachment_refence_depth = (rhi_desc).pDepthStencilAttachment[i];
                    auto& vk_attachment_refence_depth = vk_attachment_reference[currentAttachmentRefence];

                    vk_attachment_refence_depth.attachment = rhi_attachment_refence_depth.attachment;
                    vk_attachment_refence_depth.layout = (VkImageLayout)(rhi_attachment_refence_depth.layout);

                    currentAttachmentRefence += 1;
                };
            };
        };
        if (currentAttachmentRefence != totalAttachmentRefenrence)
        {
            LOG_ERROR("currentAttachmentRefence != totalAttachmentRefenrence");
            return false;
        }

        std::vector<VkSubpassDependency> vk_subpass_depandecy(pCreateInfo->dependencyCount);
        for (uint32_t i = 0; i < pCreateInfo->dependencyCount; ++i)
        {
            const auto& rhi_desc = pCreateInfo->pDependencies[i];
            auto& vk_desc = vk_subpass_depandecy[i];

            vk_desc.srcSubpass = rhi_desc.srcSubpass;
            vk_desc.dstSubpass = rhi_desc.dstSubpass;
            vk_desc.srcStageMask = (VkPipelineStageFlags)(rhi_desc).srcStageMask;
            vk_desc.dstStageMask = (VkPipelineStageFlags)(rhi_desc).dstStageMask;
            vk_desc.srcAccessMask = (VkAccessFlags)(rhi_desc).srcAccessMask;
            vk_desc.dstAccessMask = (VkAccessFlags)(rhi_desc).dstAccessMask;
            vk_desc.dependencyFlags = (VkDependencyFlags)(rhi_desc).dependencyFlags;
        };

        VkRenderPassCreateInfo create_info{};
        create_info.sType = (VkStructureType)pCreateInfo->sType;
        create_info.pNext = (const void*)pCreateInfo->pNext;
        create_info.flags = (VkRenderPassCreateFlags)pCreateInfo->flags;
        create_info.attachmentCount = pCreateInfo->attachmentCount;
        create_info.pAttachments = vk_attachments.data();
        create_info.subpassCount = pCreateInfo->subpassCount;
        create_info.pSubpasses = vk_subpass_description.data();
        create_info.dependencyCount = pCreateInfo->dependencyCount;
        create_info.pDependencies = vk_subpass_depandecy.data();

        pRenderPass = new VulkanRenderPass();
        VkRenderPass vk_render_pass;
        VkResult result = vkCreateRenderPass(m_device, &create_info, nullptr, &vk_render_pass);
        ((VulkanRenderPass*)pRenderPass)->setResource(vk_render_pass);

        if (result == VK_SUCCESS)
        {
            return RHI_SUCCESS;
        }
        else
        {
            LOG_ERROR("vkCreateRenderPass failed!");
            return false;
        }
    }

    bool VulkanRHI::createFramebuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer* &pFramebuffer)
    {
        //image_view
        int image_view_size = pCreateInfo->attachmentCount;
        std::vector<VkImageView> vk_image_view_list(image_view_size);
        for (int i = 0; i < image_view_size; ++i)
        {
            const auto& rhi_image_view_element = pCreateInfo->pAttachments[i];
            auto& vk_image_view_element = vk_image_view_list[i];

            vk_image_view_element = ((VulkanImageView*)rhi_image_view_element)->getResource();
        };

        VkFramebufferCreateInfo create_info{};
        create_info.sType = (VkStructureType)pCreateInfo->sType;
        create_info.pNext = (const void*)pCreateInfo->pNext;
        create_info.flags = (VkFramebufferCreateFlags)pCreateInfo->flags;
        create_info.renderPass = ((VulkanRenderPass*)pCreateInfo->renderPass)->getResource();
        create_info.attachmentCount = pCreateInfo->attachmentCount;
        create_info.pAttachments = vk_image_view_list.data();
        create_info.width = pCreateInfo->width;
        create_info.height = pCreateInfo->height;
        create_info.layers = pCreateInfo->layers;

        pFramebuffer = new VulkanFramebuffer();
        VkFramebuffer vk_framebuffer;
        VkResult result = vkCreateFramebuffer(m_device, &create_info, nullptr, &vk_framebuffer);
        ((VulkanFramebuffer*)pFramebuffer)->setResource(vk_framebuffer);

        if (result == VK_SUCCESS)
        {
            return RHI_SUCCESS;
        }
        else
        {
            LOG_ERROR("vkCreateFramebuffer failed!");
            return false;
        }
    }

    bool VulkanRHI::createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout* &pSetLayout)
    {
        //descriptor_set_layout_binding
        int descriptor_set_layout_binding_size = pCreateInfo->bindingCount;
        std::vector<VkDescriptorSetLayoutBinding> vk_descriptor_set_layout_binding_list(descriptor_set_layout_binding_size);

        int sampler_count = 0;
        for (int i = 0; i < descriptor_set_layout_binding_size; ++i)
        {
            const auto& rhi_descriptor_set_layout_binding_element = pCreateInfo->pBindings[i];
            if (rhi_descriptor_set_layout_binding_element.pImmutableSamplers != nullptr)
            {
                sampler_count += rhi_descriptor_set_layout_binding_element.descriptorCount;
            }
        }
        std::vector<VkSampler> sampler_list(sampler_count);
        int sampler_current = 0;

        for (int i = 0; i < descriptor_set_layout_binding_size; ++i)
        {
            const auto& rhi_descriptor_set_layout_binding_element = pCreateInfo->pBindings[i];
            auto& vk_descriptor_set_layout_binding_element = vk_descriptor_set_layout_binding_list[i];

            //sampler
            vk_descriptor_set_layout_binding_element.pImmutableSamplers = nullptr;
            if (rhi_descriptor_set_layout_binding_element.pImmutableSamplers)
            {
                vk_descriptor_set_layout_binding_element.pImmutableSamplers = &sampler_list[sampler_current];
                for (uint32_t i = 0; i < rhi_descriptor_set_layout_binding_element.descriptorCount; ++i)
                {
                    const auto& rhi_sampler_element = rhi_descriptor_set_layout_binding_element.pImmutableSamplers[i];
                    auto& vk_sampler_element = sampler_list[sampler_current];

                    vk_sampler_element = ((VulkanSampler*)rhi_sampler_element)->getResource();

                    sampler_current++;
                };
            }
            vk_descriptor_set_layout_binding_element.binding = rhi_descriptor_set_layout_binding_element.binding;
            vk_descriptor_set_layout_binding_element.descriptorType = (VkDescriptorType)rhi_descriptor_set_layout_binding_element.descriptorType;
            vk_descriptor_set_layout_binding_element.descriptorCount = rhi_descriptor_set_layout_binding_element.descriptorCount;
            vk_descriptor_set_layout_binding_element.stageFlags = rhi_descriptor_set_layout_binding_element.stageFlags;
        };
        
        if (sampler_count != sampler_current)
        {
            LOG_ERROR("sampler_count != sampller_current");
            return false;
        }

        VkDescriptorSetLayoutCreateInfo create_info{};
        create_info.sType = (VkStructureType)pCreateInfo->sType;
        create_info.pNext = (const void*)pCreateInfo->pNext;
        create_info.flags = (VkDescriptorSetLayoutCreateFlags)pCreateInfo->flags;
        create_info.bindingCount = pCreateInfo->bindingCount;
        create_info.pBindings = vk_descriptor_set_layout_binding_list.data();

        pSetLayout = new VulkanDescriptorSetLayout();
        VkDescriptorSetLayout vk_descriptorSetLayout;
        VkResult result = vkCreateDescriptorSetLayout(m_device, &create_info, nullptr, &vk_descriptorSetLayout);
        ((VulkanDescriptorSetLayout*)pSetLayout)->setResource(vk_descriptorSetLayout);

        if (result == VK_SUCCESS)
        {
            return RHI_SUCCESS;
        }
        else
        {
            LOG_ERROR("vkCreateDescriptorSetLayout failed!");
            return false;
        }
    }

    bool VulkanRHI::allocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet* &pDescriptorSets)
    {
        //descriptor_set_layout
        int descriptor_set_layout_size = pAllocateInfo->descriptorSetCount;
        std::vector<VkDescriptorSetLayout> vk_descriptor_set_layout_list(descriptor_set_layout_size);
        for (int i = 0; i < descriptor_set_layout_size; ++i)
        {
            const auto& rhi_descriptor_set_layout_element = pAllocateInfo->pSetLayouts[i];
            auto& vk_descriptor_set_layout_element = vk_descriptor_set_layout_list[i];

            vk_descriptor_set_layout_element = ((VulkanDescriptorSetLayout*)rhi_descriptor_set_layout_element)->getResource();

            VulkanDescriptorSetLayout* test = ((VulkanDescriptorSetLayout*)rhi_descriptor_set_layout_element);

            test = nullptr;
        };

        VkDescriptorSetAllocateInfo descriptorset_allocate_info{};
        descriptorset_allocate_info.sType = (VkStructureType)pAllocateInfo->sType;
        descriptorset_allocate_info.pNext = (const void*)pAllocateInfo->pNext;
        descriptorset_allocate_info.descriptorPool = ((VulkanDescriptorPool*)(pAllocateInfo->descriptorPool))->getResource();
        descriptorset_allocate_info.descriptorSetCount = pAllocateInfo->descriptorSetCount;
        descriptorset_allocate_info.pSetLayouts = vk_descriptor_set_layout_list.data();

        VkDescriptorSet vk_descriptor_set;
        pDescriptorSets = new VulkanDescriptorSet;
        VkResult result = vkAllocateDescriptorSets(m_device, &descriptorset_allocate_info, &vk_descriptor_set);
        ((VulkanDescriptorSet*)pDescriptorSets)->setResource(vk_descriptor_set);

        if (result == VK_SUCCESS)
        {
            return true;
        }
        else
        {
            LOG_ERROR("vkAllocateDescriptorSets failed!");
            return false;
        }
    }

    void VulkanRHI::updateDescriptorSets(
        uint32_t descriptorWriteCount,
        const RHIWriteDescriptorSet* pDescriptorWrites,
        uint32_t descriptorCopyCount,
        const RHICopyDescriptorSet* pDescriptorCopies)
    {
        //write_descriptor_set
        int write_descriptor_set_size = descriptorWriteCount;
        std::vector<VkWriteDescriptorSet> vk_write_descriptor_set_list(write_descriptor_set_size);
        int image_info_count = 0;
        int buffer_info_count = 0;
        for (int i = 0; i < write_descriptor_set_size; ++i)
        {
            const auto& rhi_write_descriptor_set_element = pDescriptorWrites[i];
            if (rhi_write_descriptor_set_element.pImageInfo != nullptr)
            {
                image_info_count++;
            }
            if (rhi_write_descriptor_set_element.pBufferInfo != nullptr)
            {
                buffer_info_count++;
            }
        }
        std::vector<VkDescriptorImageInfo> vk_descriptor_image_info_list(image_info_count);
        std::vector<VkDescriptorBufferInfo> vk_descriptor_buffer_info_list(buffer_info_count);
        int image_info_current = 0;
        int buffer_info_current = 0;

        for (int i = 0; i < write_descriptor_set_size; ++i)
        {
            const auto& rhi_write_descriptor_set_element = pDescriptorWrites[i];
            auto& vk_write_descriptor_set_element = vk_write_descriptor_set_list[i];

            const VkDescriptorImageInfo* vk_descriptor_image_info_ptr = nullptr;
            if (rhi_write_descriptor_set_element.pImageInfo != nullptr)
            {
                auto& vk_descriptor_image_info = vk_descriptor_image_info_list[image_info_current];
                if (rhi_write_descriptor_set_element.pImageInfo->sampler == nullptr)
                {
                    vk_descriptor_image_info.sampler = nullptr;
                }
                else
                {
                    vk_descriptor_image_info.sampler = ((VulkanSampler*)rhi_write_descriptor_set_element.pImageInfo->sampler)->getResource();
                }
                vk_descriptor_image_info.imageView = ((VulkanImageView*)rhi_write_descriptor_set_element.pImageInfo->imageView)->getResource();
                vk_descriptor_image_info.imageLayout = (VkImageLayout)rhi_write_descriptor_set_element.pImageInfo->imageLayout;

                vk_descriptor_image_info_ptr = &vk_descriptor_image_info;
                image_info_current++;
            }

            const VkDescriptorBufferInfo* vk_descriptor_buffer_info_ptr = nullptr;
            if (rhi_write_descriptor_set_element.pBufferInfo != nullptr)
            {
                auto& vk_descriptor_buffer_info = vk_descriptor_buffer_info_list[buffer_info_current];
                vk_descriptor_buffer_info.buffer = ((VulkanBuffer*)rhi_write_descriptor_set_element.pBufferInfo->buffer)->getResource();
                vk_descriptor_buffer_info.offset = (VkDeviceSize)rhi_write_descriptor_set_element.pBufferInfo->offset;
                vk_descriptor_buffer_info.range = (VkDeviceSize)rhi_write_descriptor_set_element.pBufferInfo->range;

                vk_descriptor_buffer_info_ptr = &vk_descriptor_buffer_info;
                buffer_info_current++;
            }

            vk_write_descriptor_set_element.sType = (VkStructureType)rhi_write_descriptor_set_element.sType;
            vk_write_descriptor_set_element.pNext = (const void*)rhi_write_descriptor_set_element.pNext;
            vk_write_descriptor_set_element.dstSet = ((VulkanDescriptorSet*)rhi_write_descriptor_set_element.dstSet)->getResource();
            vk_write_descriptor_set_element.dstBinding = rhi_write_descriptor_set_element.dstBinding;
            vk_write_descriptor_set_element.dstArrayElement = rhi_write_descriptor_set_element.dstArrayElement;
            vk_write_descriptor_set_element.descriptorCount = rhi_write_descriptor_set_element.descriptorCount;
            vk_write_descriptor_set_element.descriptorType = (VkDescriptorType)rhi_write_descriptor_set_element.descriptorType;
            vk_write_descriptor_set_element.pImageInfo = vk_descriptor_image_info_ptr;
            vk_write_descriptor_set_element.pBufferInfo = vk_descriptor_buffer_info_ptr;
            //vk_write_descriptor_set_element.pTexelBufferView = &((VulkanBufferView*)rhi_write_descriptor_set_element.pTexelBufferView)->getResource();
        };

        if (image_info_current != image_info_count
            || buffer_info_current != buffer_info_count)
        {
            LOG_ERROR("image_info_current != image_info_count || buffer_info_current != buffer_info_count");
            return;
        }

        //copy_descriptor_set
        int copy_descriptor_set_size = descriptorCopyCount;
        std::vector<VkCopyDescriptorSet> vk_copy_descriptor_set_list(copy_descriptor_set_size);
        for (int i = 0; i < copy_descriptor_set_size; ++i)
        {
            const auto& rhi_copy_descriptor_set_element = pDescriptorCopies[i];
            auto& vk_copy_descriptor_set_element = vk_copy_descriptor_set_list[i];

            vk_copy_descriptor_set_element.sType = (VkStructureType)rhi_copy_descriptor_set_element.sType;
            vk_copy_descriptor_set_element.pNext = (const void*)rhi_copy_descriptor_set_element.pNext;
            vk_copy_descriptor_set_element.srcSet = ((VulkanDescriptorSet*)rhi_copy_descriptor_set_element.srcSet)->getResource();
            vk_copy_descriptor_set_element.srcBinding = rhi_copy_descriptor_set_element.srcBinding;
            vk_copy_descriptor_set_element.srcArrayElement = rhi_copy_descriptor_set_element.srcArrayElement;
            vk_copy_descriptor_set_element.dstSet = ((VulkanDescriptorSet*)rhi_copy_descriptor_set_element.dstSet)->getResource();
            vk_copy_descriptor_set_element.dstBinding = rhi_copy_descriptor_set_element.dstBinding;
            vk_copy_descriptor_set_element.dstArrayElement = rhi_copy_descriptor_set_element.dstArrayElement;
            vk_copy_descriptor_set_element.descriptorCount = rhi_copy_descriptor_set_element.descriptorCount;
        };

        vkUpdateDescriptorSets(m_device, descriptorWriteCount, vk_write_descriptor_set_list.data(), descriptorCopyCount, vk_copy_descriptor_set_list.data());
    }

    void VulkanRHI::createBuffer(RHIDeviceSize size, RHIBufferUsageFlags usage, RHIMemoryPropertyFlags properties, RHIBuffer* & buffer, RHIDeviceMemory* & buffer_memory)
    {
        VkBuffer vk_buffer;
        VkDeviceMemory vk_device_memory;
        
        VulkanUtil::createBuffer(m_physical_device, m_device, size, usage, properties, vk_buffer, vk_device_memory);

        buffer = new VulkanBuffer();
        buffer_memory = new VulkanDeviceMemory();
        ((VulkanBuffer*)buffer)->setResource(vk_buffer);
        ((VulkanDeviceMemory*)buffer_memory)->setResource(vk_device_memory);
    }

    void VulkanRHI::copyBuffer(RHIBuffer* srcBuffer, RHIBuffer* dstBuffer, RHIDeviceSize srcOffset, RHIDeviceSize dstOffset, RHIDeviceSize size)
    {
        VkBuffer vk_src_buffer = ((VulkanBuffer*)srcBuffer)->getResource();
        VkBuffer vk_dst_buffer = ((VulkanBuffer*)dstBuffer)->getResource();
        VulkanUtil::copyBuffer(this, vk_src_buffer, vk_dst_buffer, srcOffset, dstOffset, size);
    }

    RHICommandBuffer* VulkanRHI::beginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo {};
        allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool        = ((VulkanCommandPool*)m_rhi_command_pool)->getResource();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer command_buffer;
        vkAllocateCommandBuffers(m_device, &allocInfo, &command_buffer);

        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        _vkBeginCommandBuffer(command_buffer, &beginInfo);

        RHICommandBuffer* rhi_command_buffer = new VulkanCommandBuffer();
        ((VulkanCommandBuffer*)rhi_command_buffer)->setResource(command_buffer);
        return rhi_command_buffer;
    }

    void VulkanRHI::endSingleTimeCommands(RHICommandBuffer* command_buffer)
    {
        VkCommandBuffer vk_command_buffer = ((VulkanCommandBuffer*)command_buffer)->getResource();
        _vkEndCommandBuffer(vk_command_buffer);

        VkSubmitInfo submitInfo {};
        submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers    = &vk_command_buffer;

        vkQueueSubmit(((VulkanQueue*)m_graphics_queue)->getResource(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(((VulkanQueue*)m_graphics_queue)->getResource());

        vkFreeCommandBuffers(m_device, ((VulkanCommandPool*)m_rhi_command_pool)->getResource(), 1, &vk_command_buffer);
        delete(command_buffer);
    }
}