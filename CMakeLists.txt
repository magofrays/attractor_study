cmake_minimum_required(VERSION 3.25)

project(Attractor)                      
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

add_executable(Attractor)


include_directories(Attractor src ${SDL2_INCLUDE_DIRS})
add_subdirectory(src)
target_link_libraries(Attractor ${SDL2_LIBRARIES})