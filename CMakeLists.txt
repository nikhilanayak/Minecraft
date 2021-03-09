cmake_minimum_required(VERSION 3.10)

project(Minecraft)

find_package(OpenGL REQUIRED)


file(GLOB source_files "src/*.c")



include_directories(ThirdPartyIncludes)
include_directories("glfw/include")

add_subdirectory("glfw")
add_executable(Minecraft ${source_files} "src/glad.c")

target_link_libraries(Minecraft ${OPENGL_LIBRARY})
target_link_libraries(Minecraft "opengl32.lib")
target_link_libraries(Minecraft glfw ${GLFW_LIBRARIES})
