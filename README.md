# Xparser
## Overview
A modern wrapper around that which is libxml2.  Created mostly at a time when nothing out there seemed easy.  This makes use of modern thing, such as lambdas.  So enjoy.

## Dependencies
1. libmxl2,  *see [www.xmlsoft.org](http://www.xmlsoft.org/)

1. Install Cmake 3.14 (or later) *see [cmake.org](https://cmake.org/)

1. Install GCC 8.1 (or later) *see [gcc.gnu.org](https://gcc.gnu.org/)

## Setup
1. Install all dependecies on your machine. 

1. Build the Makefile
    ```
    cd lib && cmake -S . -B ./_build
    ```

    OR, if your environment does not automatically pick up the libxml or the g++, then consider running like this:

    ```
    cd lib && cmake -S . -B ./_build -DCMAKE_CXX_FLAGS="-isystem <path to libxml2>/include/libxml2" -DCMAKE_CXX_COMPILER=<path to g++>/bin/g++
    ```

1. Build the library
    ```
    cd _build && make && cd ..
    ```

This is all that is required to build the Shared library.  The SO can be found in `_build/`.


## Example
You can build a test example

1. Build the example code Makefile. (*these instructions assume the `_build/` is the build directory for the SO file from the previous steps.*)
    ```
    cd example && cmake -S . -B ./_build
    ```

    OR, if your environment does not automatically pick up the libxml or the g++, then consider running like this:

    ```
    cd example && cmake -S . -B ./_build -DCMAKE_CXX_FLAGS="-isystem <path to libxml2>/include/libxml2" -DCMAKE_CXX_COMPILER=<path to g++>/bin/g++
    ```

1. Build the example executable
    ```
    cd _build && make && cd ..
    ```

1. Run the example (*from the example directory*)
    ```
    LD_LIBRARY_PATH=../lib/_build:${LD_LIBRARY_PATH} ./_build/calendar-example ../test/etc/calendar.xml
    ```
