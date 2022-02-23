# Xparser
## Overview
A modern wrapper around that which is libxml.  Created mostly at a time when nothing out there seemed easy.  This makes use of modern thing, such as lambdas.  So enjoy.

## Dependencies
1. libmxl2,  *see [www.xmlsoft.org](http://www.xmlsoft.org/)

## Setup
1. Install all dependecies on your machine.  If you're planning to localize the shared libraries, then please note that you're going to need to update the bazel `BUILD` files.

1. Install Bazel [bazel.build](https://bazel.build/)

1. Run
    ```
    bazel build //lib/xparser
    ```

## Test
1. Run
    ```
    bazel test test:xparser-test
    ```


## Example
You can build a test example

1. Run
    ```
    bazel build //main:calendar-example
    ```

2. Run
    ```
    ./bazel-bin/main/calendar-example ./test/etc/calendar.xml
    ```
