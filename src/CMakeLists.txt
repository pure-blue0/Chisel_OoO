
cmake_minimum_required(VERSION 3.10)
add_definitions(-std=c++11)

project(model)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SRC_LIST "*.cc")

add_executable(model ${SRC_LIST} ../thirdParty/cmdline/cmdline.h)



target_link_libraries(
            model
            elfio
            yaml-cpp
            spdlog
)

target_include_directories(model PRIVATE ../thirdParty/cmdline)   
