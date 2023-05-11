
# set(SOURCE_FILES main.cpp render/interface/vulkan/vulkan_rhi.cpp Aura.cpp)

add_executable(Aura 
main.cpp 
Aura.cpp 
${PROJECT_SOURCE_DIR}/src/render/interface/vulkan_rhi/vulkan_rhi.cpp 
${PROJECT_SOURCE_DIR}/src/render/interface/vulkan_rhi/vulkan_util.cpp 
${PROJECT_SOURCE_DIR}/src/render/interface/vulkan_rhi/vulkan_vma.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC 
${Vulkan_INCLUDE_DIR} 
${PROJECT_SOURCE_DIR}/src/render/interface/vulkan_rhi
${PROJECT_SOURCE_DIR}/src/render/interface) 

target_include_directories(${PROJECT_NAME} PUBLIC 
${PROJECT_SOURCE_DIR}/src/3rdparty/vulkanmemoryallocator/include) 

target_include_directories(${PROJECT_NAME} PUBLIC 
${PROJECT_SOURCE_DIR}/src/3rdparty/tinyobjloader) 

find_library(GLFW_LIBRARY glfw3 PATHS ${GLFW_DIR}/lib-vc2022)

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARY} ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} tinyobjloader)
