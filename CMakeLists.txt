cmake_minimum_required(VERSION 3.10)
project(Aura)

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/src/3rdparty/glfw)

include_directories(${Vulkan_INCLUDE_DIR})
include_directories(${GLFW_DIR}/include)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)