#ifndef PARSER_H
#define PARSER_H


#include <any>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "DataStream.hpp"
#include "Enums/GeometryStructure.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/Arc.hpp"
#include "Structures/Bezier.hpp"
#include "Structures/Bitmap.hpp"
#include "Structures/CommentText.hpp"
#include "Structures/DirectoryStruct.hpp"
#include "Structures/Ellipse.hpp"
#include "Structures/GeneralProperties.hpp"
#include "Structures/GeometrySpecification.hpp"
#include "Structures/Library.hpp"
#include "Structures/Line.hpp"
#include "Structures/Package.hpp"
#include "Structures/PinIdxMapping.hpp"
#include "Structures/Point.hpp"
#include "Structures/Polygon.hpp"
#include "Structures/Polyline.hpp"
#include "Structures/Properties.hpp"
#include "Structures/Properties2.hpp"
#include "Structures/Rect.hpp"
#include "Structures/SymbolBBox.hpp"
#include "Structures/SymbolDisplayProp.hpp"
#include "Structures/SymbolPinBus.hpp"
#include "Structures/SymbolPinScalar.hpp"
#include "Structures/SymbolsLibrary.hpp"
#include "Structures/SymbolVector.hpp"
#include "Structures/T0x1f.hpp"
#include "Structures/TextFont.hpp"


namespace fs = std::filesystem;


// Forward declaration
struct Library;


class Parser
{
public:

    static uint16_t storedVersion;

    Parser(const fs::path& aFile, FileFormatVersion aFileFormatVersion = FileFormatVersion::C);

public:

    size_t getFileErrCtr() const
    {
        return mFileErrCtr;
    }


    Library parseLibrary();

private:


    void checkInterpretedDataLen(const std::string& aFuncName, size_t aStartOffset, size_t aEndOffset, size_t aExpectedLen);


    void readTitleBlockSymbol();


    GeometrySpecification parseGlobalSymbol();


    GeometrySpecification parseSymbolHierarchic();


    GeometrySpecification parseOffPageSymbol();


    GeometrySpecification readPinShapeSymbol();


    Package parseSymbol();


    Package parsePackage();


    SymbolsLibrary parseSymbolsLibrary();


    void parseSchematic();


    void parseHierarchy();


    void parseSymbolsERC();


    void parsePage();


    void readPartInst();


    void readT0x10();


    TextFont readTextFont();


    DirectoryStruct parseDirectory();

public:
    /**
     * @brief Extract container.
     *
     * @param aFile Path to container file.
     * @param aOutDir Output directory.
     * @return fs::path Path to the extraction directory.
     */
    fs::path extractContainer(const fs::path& aFile, const fs::path& aOutDir) const;

    /**
     * @brief Extract currently used container.
     *
     * @param aOutDir Output directory.
     * @return fs::path Path to the extraction directory.
     */
    fs::path extractContainer(const fs::path& aOutDir) const;

    /**
     * @brief Print container tree structure to console.
     */
    void printContainerTree() const;
private:

    /**
     * @brief Get the file type from file extension.
     *
     * @param aFile File name including its extension.
     * @return FileType Type of the file.
     */
    FileType getFileTypeByExtension(const fs::path& aFile) const;


    void discard_until_preamble();


    std::pair<Structure, std::any> parseStructure(Structure structure);


    void pushStructure(const std::pair<Structure, std::any>& structure, Package& container);


    Structure read_type_prefix_long();


    Structure read_type_prefix();


    Structure read_type_prefix_short();


    uint32_t readPreamble(bool readOptionalLen = true);


    uint32_t readConditionalPreamble(Structure structure, bool readOptionalLen = true);


    void readGeometryStructure(GeometryStructure geometryStructure, GeometrySpecification* geometrySpecification = nullptr);


    void readSthInPages0();


    void readGraphicCommentTextInst();


    void readWireScalar();


    void readAlias();


    void readGraphicBoxInst();


    void readDevHelper();


    Line readLine();


    Point readPoint();


    Ellipse readEllipse();


    Polygon readPolygon();


    Polyline readPolyline();


    Bezier readBezier();


    Rect readRect();


    SymbolVector readSymbolVector();


    Bitmap readBitmap();


    CommentText readCommentText();


    PinIdxMapping readPinIdxMapping();


    Arc readArc();


    SymbolPinScalar readSymbolPinScalar();


    SymbolPinBus readSymbolPinBus();


    SymbolDisplayProp readSymbolDisplayProp();


    void readERCSymbol();


    SymbolBBox readSymbolBBox();


    T0x1f readT0x1f();


    GeneralProperties readGeneralProperties();


    Properties readProperties();


    Properties2 readProperties2();


    std::vector<Type> parseTypes();


    GeometrySpecification readSymbolProperties();


    GeometrySpecification parseGeometrySpecification();

public:

    // Public data


    Library mLibrary; //!< This stores the content of the parsed library file

private:

    void openFile(const fs::path& aFile);
    void closeFile();
    void exceptionHandling();

    FileType mFileType;
    FileFormatVersion mFileFormatVersion;

    fs::path mInputFile;
    size_t   mInputFileSize;

    fs::path mCurrOpenFile;
    size_t   mCurrOpenFileSize;

    fs::path mExtractedPath;

    size_t mFileCtr;    //!< Counts all files that were opend for parsing
    size_t mFileErrCtr; //!< Counts all files that failed somewhere

    DataStream mDs;

    uint32_t mByteOffset;
};


#endif // PARSER_H