# Add CompoundFileReader dependency
find_path(COMPOUNDFILEREADER_INCLUDE_DIRS "compoundfilereader.h")

# Add fmt dependency
find_package(fmt CONFIG REQUIRED)

# Add Magic Enum dependency
find_package(magic_enum CONFIG REQUIRED)

# Add Nameof dependency
find_package(nameof CONFIG REQUIRED)

# Add spdlog dependency
find_package(spdlog CONFIG REQUIRED)

set(SOURCES
   ${LIB_SRC_DIR}/ContainerExtractor.cpp
   ${LIB_SRC_DIR}/DataStream.cpp
   ${LIB_SRC_DIR}/Files/AdminData.cpp
   ${LIB_SRC_DIR}/Files/CellsDirectory.cpp
   ${LIB_SRC_DIR}/Files/DsnStream.cpp
   ${LIB_SRC_DIR}/Files/ExportBlocksDirectory.cpp
   ${LIB_SRC_DIR}/Files/GraphicsDirectory.cpp
   ${LIB_SRC_DIR}/Files/Hierarchy.cpp
   ${LIB_SRC_DIR}/Files/HSObjects.cpp
   ${LIB_SRC_DIR}/Files/NetBundleMapData.cpp
   ${LIB_SRC_DIR}/Files/Package.cpp
   ${LIB_SRC_DIR}/Files/PackagesDirectory.cpp
   ${LIB_SRC_DIR}/Files/Page.cpp
   ${LIB_SRC_DIR}/Files/PartsDirectory.cpp
   ${LIB_SRC_DIR}/Files/Schematic.cpp
   ${LIB_SRC_DIR}/Files/Symbol.cpp
   ${LIB_SRC_DIR}/Files/SymbolsDirectory.cpp
   ${LIB_SRC_DIR}/Files/SymbolsLibrary.cpp
   ${LIB_SRC_DIR}/Files/Type.cpp
   ${LIB_SRC_DIR}/Files/ViewsDirectory.cpp
   ${LIB_SRC_DIR}/Misc.cpp
   ${LIB_SRC_DIR}/Parser.cpp
   ${LIB_SRC_DIR}/Structures/Arc.cpp
   ${LIB_SRC_DIR}/Structures/Bezier.cpp
   ${LIB_SRC_DIR}/Structures/Bitmap.cpp
   ${LIB_SRC_DIR}/Structures/CommentText.cpp
   ${LIB_SRC_DIR}/Structures/Ellipse.cpp
   ${LIB_SRC_DIR}/Structures/GeneralProperties.cpp
   ${LIB_SRC_DIR}/Structures/GeometrySpecification.cpp
   ${LIB_SRC_DIR}/Structures/Line.cpp
   ${LIB_SRC_DIR}/Structures/PartInst.cpp
   ${LIB_SRC_DIR}/Structures/PinIdxMapping.cpp
   ${LIB_SRC_DIR}/Structures/Point.cpp
   ${LIB_SRC_DIR}/Structures/Polygon.cpp
   ${LIB_SRC_DIR}/Structures/Polyline.cpp
   ${LIB_SRC_DIR}/Structures/Properties.cpp
   ${LIB_SRC_DIR}/Structures/Properties2.cpp
   ${LIB_SRC_DIR}/Structures/PropertiesTrailing.cpp
   ${LIB_SRC_DIR}/Structures/Rect.cpp
   ${LIB_SRC_DIR}/Structures/SymbolBBox.cpp
   ${LIB_SRC_DIR}/Structures/SymbolDisplayProp.cpp
   ${LIB_SRC_DIR}/Structures/SymbolPinBus.cpp
   ${LIB_SRC_DIR}/Structures/SymbolPinScalar.cpp
   ${LIB_SRC_DIR}/Structures/SymbolVector.cpp
   ${LIB_SRC_DIR}/Structures/T0x1f.cpp
   ${LIB_SRC_DIR}/Structures/TextFont.cpp
   ${LIB_SRC_DIR}/Structures/WireScalar.cpp
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
   ${LIB_INCLUDE_DIR}/Files/AdminData.hpp
   ${LIB_INCLUDE_DIR}/Files/DsnStream.hpp
   ${LIB_INCLUDE_DIR}/Files/HSObjects.hpp
   ${LIB_INCLUDE_DIR}/Files/NetBundleMapData.hpp
   ${LIB_INCLUDE_DIR}/Files/Package.hpp
   ${LIB_INCLUDE_DIR}/Files/Symbol.hpp
   ${LIB_INCLUDE_DIR}/Files/SymbolsLibrary.hpp
   ${LIB_INCLUDE_DIR}/Files/Type.hpp
   ${LIB_INCLUDE_DIR}/FutureData.hpp
   ${LIB_INCLUDE_DIR}/General.hpp
   ${LIB_INCLUDE_DIR}/Parser.hpp
   ${LIB_INCLUDE_DIR}/Structures/Arc.hpp
   ${LIB_INCLUDE_DIR}/Structures/Bezier.hpp
   ${LIB_INCLUDE_DIR}/Structures/Bitmap.hpp
   ${LIB_INCLUDE_DIR}/Structures/CommentText.hpp
   ${LIB_INCLUDE_DIR}/Structures/Ellipse.hpp
   ${LIB_INCLUDE_DIR}/Structures/GeneralProperties.hpp
   ${LIB_INCLUDE_DIR}/Structures/GeometrySpecification.hpp
   ${LIB_INCLUDE_DIR}/Structures/Line.hpp
   ${LIB_INCLUDE_DIR}/Structures/PinIdxMapping.hpp
   ${LIB_INCLUDE_DIR}/Structures/Point.hpp
   ${LIB_INCLUDE_DIR}/Structures/Polygon.hpp
   ${LIB_INCLUDE_DIR}/Structures/Polyline.hpp
   ${LIB_INCLUDE_DIR}/Structures/Properties.hpp
   ${LIB_INCLUDE_DIR}/Structures/Properties2.hpp
   ${LIB_INCLUDE_DIR}/Structures/PropertiesTrailing.hpp
   ${LIB_INCLUDE_DIR}/Structures/Rect.hpp
   ${LIB_INCLUDE_DIR}/Structures/SymbolPinScalar.hpp
   ${LIB_INCLUDE_DIR}/Structures/T0x1f.hpp
   ${LIB_INCLUDE_DIR}/Structures/TextFont.hpp
   ${LIB_INCLUDE_DIR}/Structures/WireScalar.hpp
)

# Create library file from sources
add_library(${NAME_LIB} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${NAME_LIB} PRIVATE
                           ${COMPOUNDFILEREADER_INCLUDE_DIRS}
                           ${LIB_INCLUDE_DIR}
)

target_link_libraries(${NAME_LIB} PRIVATE
                      fmt::fmt
                      magic_enum::magic_enum
                      nameof::nameof
                      spdlog::spdlog
                      spdlog::spdlog_header_only
)