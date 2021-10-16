
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
endif()

set(THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# Add Boost dependency
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Add CompoundFileReader dependency
set(compoundfilereader_INCLUDE_DIR ${THIRDPARTY}/compoundfilereader/src/include)

set(HEADERS
   ${HEADERS}
   ${compoundfilereader_INCLUDE_DIR}/compoundfilereader.h
   ${compoundfilereader_INCLUDE_DIR}/utf.h
)

set(SOURCES
   ${SOURCES}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ContainerExtractor.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStream.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ContainerExtractor.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStream.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/General.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser.hpp
)

# Create executable file from sources
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
                           ${Boost_INCLUDE_DIR}
                           ${compoundfilereader_INCLUDE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
                      ${Boost_LIBRARIES}
)