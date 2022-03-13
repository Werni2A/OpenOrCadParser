#!/bin/bash

echo Downloading/Setting Up Unit Test Repositories...

# Download repositories for unit tests
python3 test/py/testing.py -r True -d repos.yaml -t thirdparty

echo Generating Unit Tests...

# Generate C++ unit test files
python3 test/py/testing.py -g 1 -d repos.yaml -u test/src/Misc -t thirdparty

echo Building Applications...

# Build applications
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build

echo Running Unit Tests...

# Run unit tests
./build/test/test > OpenOrCadParser.log 2>&1 && echo OK

echo Writing Back Error Counter Log into Database...

# Write back the error counter results from the log
python3 test/py/testing.py -w 1 -d repos.yaml -u test/src/Misc -t thirdparty -e test/test_err_cnt.log