
# set(SOURCE_FILES main.cpp render/interface/vulkan/vulkan_rhi.cpp Aura.cpp)

add_executable(Aura 
main.cpp 
Aura.cpp 
${PROJECT_SOURCE_DIR}/src/render/interface/vulkan_rhi/vulkan_rhi.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})

find_library(GLFW_LIBRARY glfw3 PATHS ${GLFW_DIR}/lib-vc2022)

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARY} ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY})