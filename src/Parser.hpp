#ifndef PARSER_H
#define PARSER_H


#include <any>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

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
#include "Structures/Type.hpp"


namespace fs = std::filesystem;


// Forward declaration
struct Library;


#include <optional>
#include <functional>

struct FileStructure
{
    // All known standard files

    std::optional<fs::path> AdminData;

    fs::path Cache;

    fs::path Cells;
    std::optional<fs::path> CellsDir;

    fs::path DsnStream;

    fs::path ExportBlocks;
    fs::path ExportBlocksDir;

    fs::path HSObjects;

    fs::path Graphics;
    fs::path GraphicsDir;
    fs::path GraphicsTypes;

    fs::path Library;

    std::optional<fs::path> NetBundleMapData;

    fs::path Packages;
    std::vector<fs::path> PackagesPackages;
    fs::path PackagesDir;

    fs::path Parts;
    fs::path PartsDir;

    fs::path Symbols;
    fs::path SymbolsERC;
    fs::path SymbolsTypes;
    std::vector<fs::path> SymbolsSymbols;
    fs::path SymbolsDir;

    fs::path Views;
    fs::path ViewsDir;

    // struct View
    // {
    //     fs::path ViewsViewsSchematics;
    //     fs::path ViewsViewsHierarchyHierarchy;
    //     fs::path ViewsViewsPages;

            // struct Page
            // {
            //     std::string name;
            //     fs::path    path;
            // };

    //     std::vector<Page> ViewsViewsPagesPages;
    // };

    // std::vector<View> ViewsViews;


    std::vector<fs::path> ViewsSchematics;

    std::vector<fs::path> ViewsSchematicsSchematic;

    std::vector<std::optional<fs::path>> ViewsSchematicsHierarchyHierarchy;

    std::vector<std::optional<fs::path>> ViewsSchematicsPages;

    std::vector<std::vector<fs::path>> ViewsSchematicsPagesPages;
};


class Parser
{
public:

    Parser(const fs::path& aFile, FileFormatVersion aFileFormatVersion = FileFormatVersion::C);

    ~Parser();

    size_t getFileErrCtr() const
    {
        return mFileErrCtr;
    }


    Library parseLibrary();


    void readAdminData(const fs::path& aFilePath);
    void readNetBundleMapData(const fs::path& aFilePath);

    // @todo adapt to pass variadic arguments into aParseFct
    template<typename T>
    T parseFile(const fs::path& aFilePath, const std::function<T()>& aParseFct)
    {
        T t;

        ++mFileCtr;
        try
        {
            openFile(aFilePath);
            t = aParseFct();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }

        spdlog::info("\n----------------------------------------------------------------------------------\n");

        return t;
    };


    // @todo adapt to pass variadic arguments into aParseFct
    // template<typename T>
    void parseFile(const fs::path& aFilePath, const std::function<void()>& aParseFct)
    {
        ++mFileCtr;
        try
        {
            openFile(aFilePath);
            aParseFct();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }

        spdlog::info("\n----------------------------------------------------------------------------------\n");
    };


private:

    void populateFilePaths(const fs::path& aPathLib, FileStructure& aFileStruct);


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

    std::vector<fs::path> mRemainingFiles; //!< Files that have not yet been parsed

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