#ifndef PARSER_HPP
#define PARSER_HPP


#include <any>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "DataStream.hpp"
#include "DataVariants.hpp"
#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Library.hpp"
#include "Primitives/Point.hpp"
#include "Primitives/PrimArc.hpp"
#include "Primitives/PrimBezier.hpp"
#include "Primitives/PrimBitmap.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Primitives/PrimEllipse.hpp"
#include "Primitives/PrimLine.hpp"
#include "Primitives/PrimPolygon.hpp"
#include "Primitives/PrimPolyline.hpp"
#include "Primitives/PrimRect.hpp"
#include "Primitives/PrimSymbolVector.hpp"
#include "Streams/StreamAdminData.hpp"
#include "Streams/StreamDirectoryStruct.hpp"
#include "Streams/StreamDsnStream.hpp"
#include "Streams/StreamHSObjects.hpp"
#include "Streams/StreamLibrary.hpp"
#include "Streams/StreamNetBundleMapData.hpp"
#include "Streams/StreamPackage.hpp"
#include "Streams/StreamSymbol.hpp"
#include "Streams/StreamType.hpp"
#include "Structures/StructAlias.hpp"
#include "Structures/StructERCSymbol.hpp"
#include "Structures/StructGeneralProperties.hpp"
#include "Structures/StructGraphicBoxInst.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"
#include "Structures/StructPartInst.hpp"
#include "Structures/StructPinIdxMapping.hpp"
#include "Structures/StructPrimitives.hpp"
#include "Structures/StructProperties.hpp"
#include "Structures/StructProperties2.hpp"
#include "Structures/StructSthInHierarchy1.hpp"
#include "Structures/StructSthInHierarchy2.hpp"
#include "Structures/StructSthInHierarchy3.hpp"
#include "Structures/StructSthInPages0.hpp"
#include "Structures/StructSymbolBBox.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPinBus.hpp"
#include "Structures/StructSymbolPinScalar.hpp"
#include "Structures/StructT0x1f.hpp"
#include "Structures/StructT0x10.hpp"
#include "Structures/StructTitleBlockSymbol.hpp"
#include "Structures/StructWireScalar.hpp"
#include "Structures/TextFont.hpp"
#include "Structures/TrailingProperties.hpp"


namespace fs = std::filesystem;


// Forward declaration
struct Library;


class Parser
{
public:

    Parser(const fs::path& aFile, FileFormatVersion aFileFormatVersion = FileFormatVersion::C);

    ~Parser();

    size_t getFileErrCtr() const
    {
        return mFileErrCtr;
    }

    // private:

    void checkInterpretedDataLen(const std::string &aFuncName, size_t aStartOffset, size_t aEndOffset, size_t aExpectedLen);

    template <typename T>
    T parseFile(const fs::path &aFilePath, std::function<T()> aParseFunc)
    {
        T parsed_obj;

        ++mFileCtr;
        mFutureDataLst.clear();
        try
        {
            openFile(aFilePath.string());
            parsed_obj = aParseFunc();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }

        spdlog::info("----------------------------------------------------------------------------------\n");

        return parsed_obj;
    }

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
// private:

    /**
     * @brief Get the file type from file extension.
     *
     * @param aFile File name including its extension.
     * @return FileType Type of the file.
     */
    FileType getFileTypeByExtension(const fs::path& aFile) const;

    void discard_until_preamble();

    Structure auto_read_prefixes();

    Structure read_prefixes(size_t aNumber, bool aPrediction = false);

    std::pair<Structure, uint32_t> read_single_prefix();

    std::pair<Structure, uint32_t> read_single_prefix_short();

    uint32_t readPreamble(bool readOptionalLen = true);

    uint32_t readConditionalPreamble(Structure structure, bool readOptionalLen = true);

    std::optional<FutureData> getFutureData();

    void sanitizeThisFutureSize(std::optional<FutureData> aThisFuture);

    std::optional<FutureData> checkTrailingFuture();

    // ---------------------------------------------
    // -------------- Read Container ---------------
    // ---------------------------------------------

    Library parseLibrary();

    // ---------------------------------------------
    // --------------- Read Stream -----------------
    // ---------------------------------------------

    bool readStreamERC();
    bool readStreamHierarchy();
    bool readStreamPage();
    bool readStreamSchematic();
    std::vector<Type> readStreamType();
    StreamAdminData readStreamAdminData();
    StreamDirectoryStruct readStreamCellsDirectory();
    StreamDirectoryStruct readStreamExportBlocksDirectory();
    StreamDirectoryStruct readStreamGraphicsDirectory();
    StreamDirectoryStruct readStreamPackagesDirectory();
    StreamDirectoryStruct readStreamPartsDirectory();
    StreamDirectoryStruct readStreamSymbolsDirectory();
    StreamDirectoryStruct readStreamViewsDirectory();
    StreamDsnStream readStreamDsnStream();
    StreamHSObjects readStreamHSObjects();
    StreamLibrary readStreamLibrary();
    StreamNetBundleMapData readStreamNetBundleMapData();
    StreamPackage readStreamPackage(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    StreamSymbol readStreamSymbol();

    // ---------------------------------------------
    // -------------- Read Structure ---------------
    // ---------------------------------------------

    StructAlias readStructAlias();
    StructERCSymbol readStructERCSymbol();
    StructGeneralProperties readStructGeneralProperties();
    StructGraphicBoxInst readStructGraphicBoxInst();
    StructGraphicCommentTextInst readStructGraphicCommentTextInst();
    StructPartInst readStructPartInst();
    StructPinIdxMapping readStructPinIdxMapping();
    StructPrimitives readStructGlobalSymbol();
    StructPrimitives readStructHierarchicSymbol();
    StructPrimitives readStructOffPageSymbol();
    StructPrimitives readStructPinShapeSymbol();
    StructPrimitives readStructPrimitives(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    StructProperties readStructProperties();
    StructProperties2 readStructProperties2();
    StructSthInHierarchy1 readStructSthInHierarchy1();
    StructSthInHierarchy2 readStructSthInHierarchy2();
    StructSthInHierarchy3 readStructSthInHierarchy3();
    StructSthInPages0 readStructSthInPages0();
    StructSymbolBBox readStructSymbolBBox();
    StructSymbolDisplayProp readStructSymbolDisplayProp();
    StructSymbolPinBus readStructSymbolPinBus();
    StructSymbolPinScalar readStructSymbolPinScalar();
    StructT0x1f readStructT0x1f();
    StructT0x10 readStructT0x10();
    StructTitleBlockSymbol readStructTitleBlockSymbol();
    StructWireScalar readStructWireScalar();

    // ---------------------------------------------
    // -------------- Read Primitive ---------------
    // ---------------------------------------------

    PrimArc readPrimArc(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimBezier readPrimBezier(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimBitmap readPrimBitmap();
    PrimCommentText readPrimCommentText(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimEllipse readPrimEllipse(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimLine readPrimLine(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimPolygon readPrimPolygon(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimPolyline readPrimPolyline(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimRect readPrimRect(FileFormatVersion aVersion = FileFormatVersion::Unknown);
    PrimSymbolVector readPrimSymbolVector();

    // ---------------------------------------------
    // ---------------- Read Misc ------------------
    // ---------------------------------------------

    Point readPoint();
    TextFont readTextFont();

    VariantPrimitive readPrimitive(Primitive aPrimitive);
    VariantStructure readStructure(Structure aStructure);
    TrailingProperties readTrailingProperties();

    void readTitleBlockSymbol();

public:
    // Public data

    Library mLibrary; //!< This stores the content of the parsed library file

private:
    void openFile(const fs::path& aFile);
    void closeFile();
    void exceptionHandling();

    FileType mFileType;
    FileFormatVersion mFileFormatVersion;

    std::vector<fs::path> mRemainingFiles; //!< Streams that have not yet been parsed

    fs::path mInputFile;
    size_t   mInputFileSize;

    fs::path mCurrOpenFile;
    size_t   mCurrOpenFileSize;

    fs::path mExtractedPath;

    size_t mFileCtr;    //!< Counts all files that were opend for parsing
    size_t mFileErrCtr; //!< Counts all files that failed somewhere

    DataStream mDs;

    uint32_t mByteOffset;

    FutureDataLst mFutureDataLst;
};


#endif // PARSER_HPP