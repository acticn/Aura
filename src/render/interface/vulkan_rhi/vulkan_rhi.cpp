#include "vulkan_rhi.h"



namespace Aura {
    void VulkanRHI::initialize() {
        initWindow();
        instance = VulkanInstance::getInstance();
        std::cout << "initialized" << std::endl;

    }
    void VulkanRHI::render_test() {
        std::cout << "rendered" << std::endl;
    }
    void VulkanRHI::initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    } 
    void VulkanRHI::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<VulkanRHI*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
    
}