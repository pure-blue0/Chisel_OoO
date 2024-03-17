
cmake_minimum_required(VERSION 3.10)

project(main)

option(TRACE_ON "Debug Trace On" ON)
if (TRACE_ON)
    add_definitions( -DTRACE_ON )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 

SET(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Use some expat option")

SET(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Use some expat option") 

SET(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Use some expat option")

add_subdirectory(src)
add_subdirectory(thirdParty/ELFIO)
add_subdirectory(thirdParty/yaml-cpp)
add_subdirectory(thirdParty/spdlog)
add_dependencies(model elfio yaml-cpp spdlog)

set(CMAKE_C_FLAGS_Release "${CMAKE_C_FLAGS_Release} -O3")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

