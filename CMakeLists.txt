project(lateral)
cmake_minimum_required(VERSION 2.6)

file(GLOB_RECURSE sourceFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.c)

file(GLOB_RECURSE headerFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.h)

add_library(${TARGET_NAME} STATIC ${sourceFiles} ${headerFiles})

#glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(External/glfw)

#glm
add_subdirectory(External/glm)

add_executable(lateral ${sourceFiles} ${headerFiles})
find_package(Vulkan REQUIRED)
find_package(Opengl REQUIRED)
target_link_libraries(lateral glfw glm Vulkan::Vulkan opengl32)