cmake_minimum_required(VERSION 3.8)

project(OpenOrCadParser)

option(ENABLE_UNIT_TESTING "Enable unit testing" OFF)

if(CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE not defined, 'Debug' will be used")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -flto")
endif()

# Add fmt dependency
find_package(fmt CONFIG REQUIRED)

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

# Add spdlog dependency
find_package(spdlog CONFIG REQUIRED)

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
if(ENABLE_UNIT_TESTING)
    add_subdirectory(test)
endif(ENABLE_UNIT_TESTING)
