# Add CompoundFileReader dependency
find_path(COMPOUNDFILEREADER_INCLUDE_DIRS "compoundfilereader.h")

# Add tinyxml2 dependency
find_package(tinyxml2 CONFIG REQUIRED)

set(SOURCES
   ${LIB_SRC_DIR}/Container.cpp
   ${LIB_SRC_DIR}/ContainerExtractor.cpp
   ${LIB_SRC_DIR}/DataStream.cpp
   ${LIB_SRC_DIR}/GenericParser.cpp
   ${LIB_SRC_DIR}/PageSettings.cpp
   ${LIB_SRC_DIR}/Primitives/Point.cpp
   ${LIB_SRC_DIR}/Primitives/PrimArc.cpp
   ${LIB_SRC_DIR}/Primitives/PrimBezier.cpp
   ${LIB_SRC_DIR}/Primitives/PrimBitmap.cpp
   ${LIB_SRC_DIR}/Primitives/PrimCommentText.cpp
   ${LIB_SRC_DIR}/Primitives/PrimEllipse.cpp
   ${LIB_SRC_DIR}/Primitives/PrimLine.cpp
   ${LIB_SRC_DIR}/Primitives/PrimPolygon.cpp
   ${LIB_SRC_DIR}/Primitives/PrimPolyline.cpp
   ${LIB_SRC_DIR}/Primitives/PrimRect.cpp
   ${LIB_SRC_DIR}/Primitives/PrimSymbolVector.cpp
   ${LIB_SRC_DIR}/RecordFactory.cpp
   ${LIB_SRC_DIR}/StreamFactory.cpp
   ${LIB_SRC_DIR}/Streams/StreamAdminData.cpp
   ${LIB_SRC_DIR}/Streams/StreamBOMDataStream.cpp
   ${LIB_SRC_DIR}/Streams/StreamCache.cpp
   ${LIB_SRC_DIR}/Streams/StreamDirectoryStruct.cpp
   ${LIB_SRC_DIR}/Streams/StreamDsnStream.cpp
   ${LIB_SRC_DIR}/Streams/StreamDTypeD.cpp
   ${LIB_SRC_DIR}/Streams/StreamERC.cpp
   ${LIB_SRC_DIR}/Streams/StreamHierarchy.cpp
   ${LIB_SRC_DIR}/Streams/StreamHSObjects.cpp
   ${LIB_SRC_DIR}/Streams/StreamLibrary.cpp
   ${LIB_SRC_DIR}/Streams/StreamNetBundleMapData.cpp
   ${LIB_SRC_DIR}/Streams/StreamPackage.cpp
   ${LIB_SRC_DIR}/Streams/StreamPage.cpp
   ${LIB_SRC_DIR}/Streams/StreamSchematic.cpp
   ${LIB_SRC_DIR}/Streams/StreamSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructAlias.cpp
   ${LIB_SRC_DIR}/Structures/StructBookMarkSymbolInst.cpp
   ${LIB_SRC_DIR}/Structures/StructBusEntry.cpp
   ${LIB_SRC_DIR}/Structures/StructDevice.cpp
   ${LIB_SRC_DIR}/Structures/StructERCObject.cpp
   ${LIB_SRC_DIR}/Structures/StructERCSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructGeneralProperties.cpp
   ${LIB_SRC_DIR}/Structures/StructGlobal.cpp
   ${LIB_SRC_DIR}/Structures/StructGlobalSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicArcInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicBezierInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicBitMapInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicBoxInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicCommentTextInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicEllipseInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicLineInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicOleEmbedInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicPolygonInst.cpp
   ${LIB_SRC_DIR}/Structures/StructGraphicPolylineInst.cpp
   ${LIB_SRC_DIR}/Structures/StructLibraryPart.cpp
   ${LIB_SRC_DIR}/Structures/StructNetDbIdMapping.cpp
   ${LIB_SRC_DIR}/Structures/StructOffPageConnector.cpp
   ${LIB_SRC_DIR}/Structures/StructOffPageSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructPackage.cpp
   ${LIB_SRC_DIR}/Structures/StructPartCell.cpp
   ${LIB_SRC_DIR}/Structures/StructPinShapeSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructPlacedInstance.cpp
   ${LIB_SRC_DIR}/Structures/StructPort.cpp
   ${LIB_SRC_DIR}/Structures/StructPortSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructSthInHierarchy1.cpp
   ${LIB_SRC_DIR}/Structures/StructSthInHierarchy2.cpp
   ${LIB_SRC_DIR}/Structures/StructSthInHierarchy3.cpp
   ${LIB_SRC_DIR}/Structures/StructSthInPages0.cpp
   ${LIB_SRC_DIR}/Structures/StructSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructSymbolBBox.cpp
   ${LIB_SRC_DIR}/Structures/StructSymbolDisplayProp.cpp
   ${LIB_SRC_DIR}/Structures/StructSymbolPin.cpp
   ${LIB_SRC_DIR}/Structures/StructSymbolPinBus.cpp
   ${LIB_SRC_DIR}/Structures/StructSymbolPinScalar.cpp
   ${LIB_SRC_DIR}/Structures/StructT0x10.cpp
   ${LIB_SRC_DIR}/Structures/StructT0x34.cpp
   ${LIB_SRC_DIR}/Structures/StructT0x35.cpp
   ${LIB_SRC_DIR}/Structures/StructT0x45.cpp
   ${LIB_SRC_DIR}/Structures/StructT0x5b.cpp
   ${LIB_SRC_DIR}/Structures/StructTitleBlock.cpp
   ${LIB_SRC_DIR}/Structures/StructTitleBlockSymbol.cpp
   ${LIB_SRC_DIR}/Structures/StructWire.cpp
   ${LIB_SRC_DIR}/Structures/StructWireBus.cpp
   ${LIB_SRC_DIR}/Structures/StructWireScalar.cpp
#    ${LIB_SRC_DIR}/XmlExporter.cpp
)

# Create library file from sources
add_library(${NAME_LIB} SHARED ${SOURCES})

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
                      tinyxml2::tinyxml2
)
