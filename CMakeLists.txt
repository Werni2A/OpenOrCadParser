
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

# Add Boost dependency
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Add CompoundFileReader dependency
find_path(COMPOUNDFILEREADER_INCLUDE_DIRS "compoundfilereader.h")

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

set(SOURCES
   ${SOURCES}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ContainerExtractor.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStream.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser/parseDirectory.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser/parsePackage.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser/parseSymbolsLibrary.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Arc.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Bezier.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Bitmap.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/CommentText.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Ellipse.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Line.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Point.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Polygon.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Polyline.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Rect.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/TextFont.cpp
)

set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ContainerExtractor.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStream.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/Color.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/FillStyle.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/GeometryStructure.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/HatchStyle.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/ImplementationType.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/LineStyle.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/LineWidth.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/PortType.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums/Structure.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/General.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Arc.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Bezier.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Bitmap.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/CommentText.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Ellipse.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/GeneralProperties.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Line.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/PinIdxMapping.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Point.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Polygon.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Polyline.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Properties.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Properties2.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Rect.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/SymbolPinScalar.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/TextFont.hpp
)

# Create executable file from sources
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
                           ${Boost_INCLUDE_DIR}
                           ${COMPOUNDFILEREADER_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
                      ${Boost_LIBRARIES}
                      magic_enum::magic_enum
                      nameof::nameof
)