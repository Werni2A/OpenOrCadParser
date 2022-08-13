#!/bin/bash

test=build/test/test

echo
echo ---------------------------------------------------------
echo
echo Running Unit Tests...
echo
echo ---------------------------------------------------------
echo

$test --list-tests --verbosity quiet |
    awk '{print "\042" $0 "\042"}' |
    parallel --halt now,fail=1 -j $(nproc) $test "{}" ">" "test.log"

# $test --list-tests --verbosity quiet |
#     awk '{print "\042" $0 "\042"}' |
#     parallel --bar -j $(nproc) $test "{}" ">>" "test.log"

echo
echo ---------------------------------------------------------
echo
echo Unit Test Log
echo
echo ---------------------------------------------------------
echo

cat test.log