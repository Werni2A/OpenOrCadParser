
cmake_minimum_required(VERSION 2.8.12)

project(OpenOrCadParser)

if(CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE not defined, 'Debug' will be used")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto")
endif()

set(LIB_SRC_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CLI_SRC_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CLI_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/test/src)
set(TEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/src)

set(NAME_LIB  OpenOrCadParser)
set(NAME_CLI  OpenOrCadParser-cli)
set(NAME_TEST test)

add_subdirectory(lib)
add_subdirectory(cli)
add_subdirectory(test)