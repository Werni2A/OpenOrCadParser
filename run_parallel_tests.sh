#!/bin/bash

test=build/test/test

echo
echo ---------------------------------------------------------
echo
echo Running Unit Tests...
echo
echo ---------------------------------------------------------
echo

# $test --list-tests --verbosity quiet |
#     awk '{print "\042" $0 "\042"}' |
#     parallel --halt now,fail=1 -j $(nproc) $test "{}" ">>" "test.log"

# $test --list-tests --verbosity quiet |
#     awk '{print "\042" $0 "\042"}' |
#     parallel --bar $test "{}" ">>" "test.log"

./build/test/test 2>&1 > test.log

if [ $? -eq 0 ]; then

    echo
    echo ---------------------------------------------------------
    echo
    echo Unit Tests Completed Successfully
    echo
    echo ---------------------------------------------------------
    echo

    exit 0

else

    echo
    echo ---------------------------------------------------------
    echo
    echo Unit Tests Failed
    echo
    echo ---------------------------------------------------------
    echo

    exit 1

fi