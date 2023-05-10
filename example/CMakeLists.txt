cmake_minimum_required(VERSION 3.14)

# set the project name
project(calendar-example VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.1.0)
        message(FATAL_ERROR "GCC version must be at least 8.1.0!  You are on (${CMAKE_CXX_COMPILER_ID}) v(${CMAKE_CXX_COMPILER_VERSION})")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.0.0)
        message(FATAL_ERROR "Clang version must be at least 13.0.0!  You are on (${CMAKE_CXX_COMPILER_ID}) v(${CMAKE_CXX_COMPILER_VERSION})")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()

include_directories("${CMAKE_SOURCE_DIR}/../lib/inc")
include_directories("inc")

link_directories("${CMAKE_SOURCE_DIR}/../lib/_build")

file(
    GLOB_RECURSE
    example_src
    src/**/*.cpp
)

# add the executable
add_executable("${PROJECT_NAME}" calendarExample.cpp ${example_src})

# add compile options
if(UNIX)
    add_compile_options(
        -g,
    )
    set(CMAKE_EXE_LINKER_FLAGS "-lxml2 -lxparser")
endif()

macro(print_all_variables)
    message(STATUS "example_src=${example_src}")
    message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
    message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}")
endmacro()

print_all_variables()
