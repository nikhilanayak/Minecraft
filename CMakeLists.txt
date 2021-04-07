cmake_minimum_required(VERSION 3.10)

project(Minecraft)

find_package(OpenGL REQUIRED)


file(GLOB source_files "src/*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

add_executable(Minecraft ${source_files} "src/glad.c")


target_link_libraries(Minecraft "glfw")


if (UNIX)
    target_link_libraries(Minecraft "m")
    target_link_libraries(Minecraft "dl")
    target_compile_options(Minecraft PRIVATE "-w")
endif()
if ( MSVC )
    target_compile_options(Minecraft PRIVATE "/W4")
endif() 