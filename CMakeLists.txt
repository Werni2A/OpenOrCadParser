cmake_minimum_required(VERSION 3.8)

project(OpenOrCadParser VERSION 0.1.0)

option(ENABLE_UNIT_TESTING "Enable unit testing" OFF)

if(CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE not defined, 'Debug' will be used")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wall>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Werror>)
add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wextra>)

# @todo enable /WX
# add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/Wall>)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-g3>)
else()
    add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-O3>)
    add_compile_options($<$<COMPILE_LANG_AND_ID:CXX,GNU>:-flto>)
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
