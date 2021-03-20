cmake_minimum_required(VERSION 3.10)

project(Minecraft)

find_package(OpenGL REQUIRED)


file(GLOB source_files "src/*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories(ThirdPartyIncludes)
include_directories("glfw/include")

add_subdirectory("glfw")
add_executable(Minecraft ${source_files} "src/glad.c")


target_link_libraries(Minecraft ${OPENGL_LIBRARY})
if (WIN32)
target_link_libraries(Minecraft "opengl32.lib")
endif (WIN32)
if (UNIX)
target_link_libraries(Minecraft "-lGL")
endif (UNIX)
target_link_libraries(Minecraft glfw ${GLFW_LIBRARIES})

if (UNIX)
    #target_compile_options(Minecraft PRIVATE "-w")
endif()
if ( MSVC )
    target_compile_options(Minecraft PRIVATE "/W4")
endif() 