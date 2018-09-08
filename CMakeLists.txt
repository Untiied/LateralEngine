project(lateral)
cmake_minimum_required(VERSION 2.6)

file(GLOB_RECURSE lateralsourceFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.c)

file(GLOB_RECURSE lateralheaderFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/Code/Source/*.h)

#add_library(${TARGET_NAME} STATIC ${lateralsourceFiles} ${lateralheaderFiles})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/Code/Source/
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

#glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(External/glfw)

#glm
add_subdirectory(External/glm)

#assimp
add_subdirectory(External/assimpROOT)

include_directories(External External/assimpROOT/include)
find_package(Vulkan REQUIRED)
find_package(Opengl REQUIRED)
add_library(lateral STATIC ${lateralsourceFiles} ${lateralheaderFiles})
target_link_libraries(lateral glfw glm assimp Vulkan::Vulkan opengl32)