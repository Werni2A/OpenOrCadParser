# Add CompoundFileReader dependency
find_path(COMPOUNDFILEREADER_INCLUDE_DIRS "compoundfilereader.h")

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

set(SOURCES
   ${LIB_SRC_DIR}/ContainerExtractor.cpp
   ${LIB_SRC_DIR}/DataStream.cpp
   ${LIB_SRC_DIR}/Parser.cpp
   ${LIB_SRC_DIR}/Parser/parseDirectory.cpp
   ${LIB_SRC_DIR}/Parser/parsePackage.cpp
   ${LIB_SRC_DIR}/Parser/parseSymbolsLibrary.cpp
   ${LIB_SRC_DIR}/Structures/Arc.cpp
   ${LIB_SRC_DIR}/Structures/Bezier.cpp
   ${LIB_SRC_DIR}/Structures/Bitmap.cpp
   ${LIB_SRC_DIR}/Structures/CommentText.cpp
   ${LIB_SRC_DIR}/Structures/Ellipse.cpp
   ${LIB_SRC_DIR}/Structures/Line.cpp
   ${LIB_SRC_DIR}/Structures/Point.cpp
   ${LIB_SRC_DIR}/Structures/Polygon.cpp
   ${LIB_SRC_DIR}/Structures/Polyline.cpp
   ${LIB_SRC_DIR}/Structures/Rect.cpp
   ${LIB_SRC_DIR}/Structures/TextFont.cpp
)

set(HEADERS
   ${LIB_INCLUDE_DIR}/ContainerExtractor.hpp
   ${LIB_INCLUDE_DIR}/DataStream.hpp
   ${LIB_INCLUDE_DIR}/Enums/Color.hpp
   ${LIB_INCLUDE_DIR}/Enums/FillStyle.hpp
   ${LIB_INCLUDE_DIR}/Enums/GeometryStructure.hpp
   ${LIB_INCLUDE_DIR}/Enums/HatchStyle.hpp
   ${LIB_INCLUDE_DIR}/Enums/ImplementationType.hpp
   ${LIB_INCLUDE_DIR}/Enums/LineStyle.hpp
   ${LIB_INCLUDE_DIR}/Enums/LineWidth.hpp
   ${LIB_INCLUDE_DIR}/Enums/PortType.hpp
   ${LIB_INCLUDE_DIR}/Enums/Structure.hpp
   ${LIB_INCLUDE_DIR}/General.hpp
   ${LIB_INCLUDE_DIR}/Parser.hpp
   ${LIB_INCLUDE_DIR}/Structures/Arc.hpp
   ${LIB_INCLUDE_DIR}/Structures/Bezier.hpp
   ${LIB_INCLUDE_DIR}/Structures/Bitmap.hpp
   ${LIB_INCLUDE_DIR}/Structures/CommentText.hpp
   ${LIB_INCLUDE_DIR}/Structures/Ellipse.hpp
   ${LIB_INCLUDE_DIR}/Structures/GeneralProperties.hpp
   ${LIB_INCLUDE_DIR}/Structures/Line.hpp
   ${LIB_INCLUDE_DIR}/Structures/PinIdxMapping.hpp
   ${LIB_INCLUDE_DIR}/Structures/Point.hpp
   ${LIB_INCLUDE_DIR}/Structures/Polygon.hpp
   ${LIB_INCLUDE_DIR}/Structures/Polyline.hpp
   ${LIB_INCLUDE_DIR}/Structures/Properties.hpp
   ${LIB_INCLUDE_DIR}/Structures/Properties2.hpp
   ${LIB_INCLUDE_DIR}/Structures/Rect.hpp
   ${LIB_INCLUDE_DIR}/Structures/SymbolPinScalar.hpp
   ${LIB_INCLUDE_DIR}/Structures/TextFont.hpp
)

# Create library file from sources
add_library(${NAME_LIB} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${NAME_LIB} PRIVATE
                           ${COMPOUNDFILEREADER_INCLUDE_DIRS}
                           ${LIB_INCLUDE_DIR}
)

target_link_libraries(${NAME_LIB} PRIVATE
                      magic_enum::magic_enum
                      nameof::nameof
)