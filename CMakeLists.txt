cmake_minimum_required(VERSION 3.10)

project(Minecraft)

file(GLOB source_files "src/*.c")



add_executable(Minecraft ${source_files})

target_link_libraries(Minecraft -lglfw)
target_link_libraries(Minecraft -lm)
target_link_libraries(Minecraft -ldl)
target_link_libraries(Minecraft -lGL)
