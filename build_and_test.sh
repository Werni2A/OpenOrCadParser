#!/bin/bash

echo Downloading/Setting Up Unit Test Repositories...

# Download repositories for unit tests
python3 test/py/testing.py -r True -d repos.yaml -t test/designs

echo Generating Unit Tests...

# Generate C++ unit test files
python3 test/py/testing.py -g 1 -d repos.yaml -u test/src/generated_tests -t test/designs

echo Building Applications...

# Build applications
cmake --preset release -DENABLE_UNIT_TESTING=ON
cmake --build --preset release

echo Running Unit Tests...

# Run unit tests
python3 run_tests.py

echo Writing Back Error Counter Log into Database...

# Write back the error counter results from the log
python3 test/py/testing.py -w 1 -d repos.yaml -u test/src/generated_tests -t test/designs -e test_err_cnt.log