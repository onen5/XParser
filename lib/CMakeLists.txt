cmake_minimum_required(VERSION 3.14)

# set the project name
project(XParser VERSION 0.0.3)

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

include_directories("inc")

file(
    GLOB_RECURSE
    xparser_src
    src/**/*.cpp
)

file(
    GLOB_RECURSE
    xparser_hdr
    inc/**/*.h
)

set(OPT0FLAG "-O0")
set(OPT1FLAG "-O1")
set(OPT2FLAG "-O2")
set(OPT3FLAG "-O3")
set(OPTNFLAG ${OPT3FLAG})

set(SONAME, string(TOLOWER, ${PROJECT_NAME}))

# add compile options
if(UNIX)
    add_compile_options(
        -Wall
        -Werror
        -Wextra
        -Wunused
        -Wunreachable-code
        -fPIC
        -momit-leaf-frame-pointer
        -g
        -v
        ${OPTNFLAG}
    )
    set(CMAKE_SHARED_LINKER_FLAGS "-lxml2")

# -Wl,-soname,${SONAME}
else()
    add_compile_options(/W4 /WX)
endif()

# add libary
add_library(xparser SHARED ${xparser_src})

# install target
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    install(TARGETS xparser DESTINATION lib64)
elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    install(TARGETS xparser DESTINATION lib64)
else()
    install(TARGETS xparser DESTINATION lib)
endif()

install(FILES ${xparser_hdr} DESTINATION include/xparser)

macro(print_all_variables)
    message(STATUS "xparser_src=${xparser_src}")
    message(STATUS "xparser_hdr=${xparser_hdr}")
    message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
    message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")
endmacro()

print_all_variables()
