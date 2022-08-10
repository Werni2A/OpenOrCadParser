#include <algorithm>
#include <any>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "ContainerExtractor.hpp"
#include "DataStream.hpp"
#include "Enums/FillStyle.hpp"
#include "Enums/GeometryStructure.hpp"
#include "Enums/HatchStyle.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/PortType.hpp"
#include "Enums/Rotation.hpp"
#include "Enums/Structure.hpp"
#include "Exception.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Parser.hpp"
#include "PinShape.hpp"
#include "Structures/Arc.hpp"
#include "Structures/Ellipse.hpp"
#include "Structures/Line.hpp"
#include "Structures/Point.hpp"
#include "Structures/Polygon.hpp"
#include "Structures/Rect.hpp"
#include "Structures/SymbolDisplayProp.hpp"
#include "Structures/SymbolPinBus.hpp"
#include "Structures/SymbolPinScalar.hpp"
#include "Structures/SymbolsLibrary.hpp"
#include "Structures/T0x1f.hpp"
#include "Structures/Type.hpp"


namespace fs = std::filesystem;


uint16_t Parser::storedVersion;


Parser::Parser(const fs::path& aFile, FileFormatVersion aFileFormatVersion) :
    mFileCtr{0u}, mFileErrCtr{0u}, mFileFormatVersion{aFileFormatVersion}
{
    mFileType      = getFileTypeByExtension(aFile);
    mInputFile     = aFile;
    mInputFileSize = fs::file_size(aFile);

    const fs::path extractTo = fs::temp_directory_path() / "OpenOrCadParser";
    mExtractedPath = extractContainer(aFile, extractTo);

    // @todo Figure out the file format version and set it here
    // mFileFormatVersion = FileFormatVersion::Unknown;

    // @todo Figure out the file format version and set it here instead of an argument in
    //       the constructor
    storedVersion = 0;
}


bool requiresPreamble(Structure structure)
{
    bool required;

    switch(structure)
    {
        case Structure::Properties:        required = true;  break;
        case Structure::GeoDefinition:     required = false; break;
        case Structure::SymbolPinScalar:   required = true;  break;
        case Structure::SymbolPinBus:      required = false; break;
        case Structure::T0x1f:             required = true;  break;
        case Structure::PinIdxMapping:     required = true;  break;
        case Structure::GlobalSymbol:      required = false; break;
        case Structure::PortSymbol:        required = false; break;
        case Structure::OffPageSymbol:     required = true;  break;
        case Structure::SymbolDisplayProp: required = true;  break;
        case Structure::SymbolVector:      required = false; break;
        case Structure::TitleBlockSymbol:  required = false; break;
        case Structure::ERCSymbol:         required = false; break;
        case Structure::PinShapeSymbol:    required = false; break;
            break;
        default:
            throw std::runtime_error(std::string(__func__) + " does not implement structure " + to_string(structure));
            break;
    }

    return required;
}


/**
 * @brief Parse the whole library.
 */
Library Parser::parseLibrary()
{
    const std::string aLibPath = mExtractedPath;

    std::clog << "Start parsing library " << aLibPath << std::endl;

    fs::path pathLib = fs::path(aLibPath);

    if(!fs::exists(pathLib))
    {
        throw std::invalid_argument("Folder could not be found! " + aLibPath);
    }

    if(!fs::is_directory(pathLib))
    {
        throw std::invalid_argument("Path does not point to directory! " + aLibPath);
    }

    // All known standard files

    fs::path pathAdminData        = pathLib / "AdminData.bin";

    fs::path pathCache            = pathLib / "Cache.bin";

    fs::path pathCells            = pathLib / "Cells";
    fs::path pathCellsDir         = pathLib / "Cells Directory.bin";

    fs::path pathDsnStream        = pathLib / "DsnStream.bin";

    fs::path pathExportBlocks     = pathLib / "ExportBlocks";
    fs::path pathExportBlocksDir  = pathLib / "ExportBlocks Directory.bin";

    fs::path pathHSObjects        = pathLib / "HSObjects.bin";

    fs::path pathGraphics         = pathLib / "Graphics";
    fs::path pathGraphicsDir      = pathLib / "Graphics Directory.bin";
    fs::path pathGraphicsTypes    = pathGraphics / "$Types$.bin";

    fs::path pathLibrary          = pathLib / "Library.bin";

    fs::path pathNetBundleMapData = pathLib / "NetBundleMapData.bin"; // @todo Only in newer file format versions. Document this somewhere

    fs::path pathPackages         = pathLib / "Packages";
    fs::path pathPackagesDir      = pathLib / "Packages Directory.bin";

    fs::path pathParts            = pathLib / "Parts";
    fs::path pathPartsDir         = pathLib / "Parts Directory.bin";

    fs::path pathSymbols          = pathLib / "Symbols";
    fs::path pathSymbolsDir       = pathLib / "Symbols Directory.bin";
    fs::path pathSymbolsTypes     = pathSymbols / "$Types$.bin";
    fs::path pathSymbolsERC       = pathSymbols / "ERC.bin";

    fs::path pathViews            = pathLib / "Views";
    fs::path pathViewsDir         = pathLib / "Views Directory.bin";

    std::vector<fs::path> pathViewsSchematics;

    for(const auto& dir: std::filesystem::directory_iterator{pathViews})
    {
        if(dir.is_directory())
        {
            pathViewsSchematics.push_back(dir);
        }
        else
        {
            throw std::runtime_error("Did not expect to find files here!");
        }
    }

    std::vector<fs::path> pathViewsSchematicsSchematic;

    for(const auto& dir : pathViewsSchematics)
    {
        pathViewsSchematicsSchematic.push_back(dir / "Schematic.bin");
    }

    std::vector<fs::path> pathViewsSchematicsHierarchyHierarchy;

    for(const auto& dir : pathViewsSchematics)
    {
        pathViewsSchematicsHierarchyHierarchy.push_back(dir / "Hierarchy" / "Hierarchy.bin");
    }

    std::vector<fs::path> pathViewsSchematicsPages;

    for(const auto& dir : pathViewsSchematics)
    {
        pathViewsSchematicsPages.push_back(dir / "Pages");
    }

    std::vector<std::vector<fs::path>> pathViewsSchematicsPagesPages;

    for(const auto& pagesDir: pathViewsSchematicsPages)
    {
        std::vector<fs::path> schematicPages;

        for(const auto& page : std::filesystem::directory_iterator{pagesDir})
        {
            if(page.is_regular_file())
            {
                schematicPages.push_back(page);
            }
            else
            {
                throw std::runtime_error("Did not expect to find anything else than files here!");
            }
        }

        pathViewsSchematicsPagesPages.push_back(schematicPages);
    }

    const auto parseDirectoryFile = [this](const fs::path& aFilePath) -> DirectoryStruct
    {
        DirectoryStruct directoryStruct;

        ++mFileCtr;
        try
        {
            openFile(aFilePath.string());
            directoryStruct = parseDirectory();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }

        std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;

        return directoryStruct;
    };

    if(fs::exists(pathExportBlocksDir))
    {
        mLibrary.exportBlocksDir = parseDirectoryFile(pathExportBlocksDir);
    }
    else
    {
        std::clog << "File does not exist: " << pathExportBlocksDir.string() << std::endl;
    }

    if(fs::exists(pathGraphicsDir))
    {
        mLibrary.graphicsDir     = parseDirectoryFile(pathGraphicsDir);
    }
    else
    {
        std::clog << "File does not exist: " << pathGraphicsDir.string() << std::endl;
    }

    if(fs::exists(pathPackagesDir))
    {
        mLibrary.packagesDir     = parseDirectoryFile(pathPackagesDir);
    }
    else
    {
        std::clog << "File does not exist: " << pathPackagesDir.string() << std::endl;
    }

    if(fs::exists(pathPartsDir))
    {
        mLibrary.partsDir        = parseDirectoryFile(pathPartsDir);
    }
    else
    {
        std::clog << "File does not exist: " << pathPartsDir.string() << std::endl;
    }

    if(fs::exists(pathSymbolsDir))
    {
        mLibrary.symbolsDir      = parseDirectoryFile(pathSymbolsDir);
    }
    else
    {
        std::clog << "File does not exist: " << pathSymbolsDir.string() << std::endl;
    }

    if(fs::exists(pathCellsDir))
    {
        mLibrary.cellsDir        = parseDirectoryFile(pathCellsDir);
    }
    else
    {
        std::clog << "File does not exist: " << pathCellsDir.string() << std::endl;
    }

    if(fs::exists(pathViewsDir))
    {
        mLibrary.viewsDir        = parseDirectoryFile(pathViewsDir);
    }
    else
    {
        std::clog << "File does not exist: " << pathViewsDir.string() << std::endl;
    }

    std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;

    if(fs::exists(pathGraphicsTypes))
    {
        ++mFileCtr;
        try
        {
            openFile(pathGraphicsTypes.string());
            mLibrary.graphicsTypes = parseTypes();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }
    }
    else
    {
        std::clog << "File does not exist: " << pathGraphicsTypes.string() << std::endl;
    }


    std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;

    if(fs::exists(pathSymbolsTypes))
    {
        ++mFileCtr;
        try
        {
            openFile(pathSymbolsTypes.string());
            mLibrary.symbolsTypes = parseTypes();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }
    }
    else
    {
        std::clog << "File does not exist: " << pathSymbolsTypes.string() << std::endl;
    }

    std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;

    if(fs::exists(pathLibrary))
    {
        ++mFileCtr;
        try
        {
            openFile(pathLibrary.string());
            mLibrary.symbolsLibrary = parseSymbolsLibrary();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }
    }
    else
    {
        std::clog << "File does not exist: " << pathLibrary.string() << std::endl;
    }

    std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;

    if(fs::exists(pathSymbolsERC))
    {
        ++mFileCtr;
        try
        {
            openFile(pathSymbolsERC.string());
            // @todo write results into mLibrary
            /* mLibrary.symbolsERC = */ parseSymbolsERC();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }
    }
    else
    {
        std::clog << "File does not exist: " << pathSymbolsERC.string() << std::endl;
    }

    std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;

    if(fs::exists(pathPackages))
    {
        for(const auto& file : fs::directory_iterator(pathPackages))
        {
            fs::path pathPackage = file.path();

            bool hasError = false;

            ++mFileCtr;
            try
            {
                openFile(pathPackage.string());
                mLibrary.packages.push_back(parsePackage());
                closeFile();
            }
            catch(...)
            {
                hasError = true;
                exceptionHandling();
            }

            if(!hasError)
            {
                std::cout << printGreen("Package " + mCurrOpenFile.string() + " parsed successfuly.") << std::endl;
            }

            std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;
        }
    }
    else
    {
        std::clog << "Directory does not exist: " << pathPackages.string() << std::endl;
    }

    if(fs::exists(pathSymbols))
    {
        for(const auto& file : fs::directory_iterator(pathSymbols))
        {
            fs::path pathSymbol = file.path();

            // Skip the 'ERC' and '$Types$' stream as they are additional
            // information but no symbols.
            if(pathSymbol.filename() == "ERC.bin"     ||
               pathSymbol.filename() == "$Types$.bin")
            {
                continue;
            }

            bool hasError = false;

            ++mFileCtr;
            try
            {
                openFile(pathSymbol.string());
                // @todo Results are only stored in packages for testing purposes
                //       Replace with symbols later on.
                mLibrary.packages.push_back(parseSymbol()); // @todo implement symbol parsing
                closeFile();
            }
            catch(...)
            {
                hasError = true;
                exceptionHandling();
            }

            if(!hasError)
            {
                std::cout << printGreen("Symbol " + mCurrOpenFile.string() + " parsed successfuly.") << std::endl;
            }

            std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;
        }
    }
    else
    {
        std::clog << "Directory does not exist: " << pathSymbols.string() << std::endl;
    }

    for(const auto& schematic : pathViewsSchematicsSchematic)
    {
        if(fs::exists(schematic))
        {
            ++mFileCtr;
            try
            {
                openFile(schematic.string());
                // @todo write results into mLibrary
                /* mLibrary.symbolsERC = */ parseSchematic();
                closeFile();
            }
            catch(...)
            {
                exceptionHandling();
            }
        }
        else
        {
            std::clog << "File does not exist: " << schematic.string() << std::endl;
        }

        std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;
    }

    for(const auto& hierarchy : pathViewsSchematicsHierarchyHierarchy)
    {
        if(fs::exists(hierarchy))
        {
            ++mFileCtr;
            try
            {
                openFile(hierarchy.string());
                // @todo write results into mLibrary
                /* mLibrary.symbolsERC = */ parseHierarchy();
                closeFile();
            }
            catch(...)
            {
                exceptionHandling();
            }
        }
        else
        {
            std::clog << "File does not exist: " << hierarchy.string() << std::endl;
        }

        std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;
    }

    for(const auto& pages : pathViewsSchematicsPagesPages)
    {
        for(const auto& page : pages)
        {
            if(fs::exists(page))
            {
                ++mFileCtr;
                try
                {
                    openFile(page.string());
                    // @todo write results into mLibrary
                    /* mLibrary.symbolsERC = */ parsePage();
                    closeFile();
                }
                catch(...)
                {
                    exceptionHandling();
                }
            }
            else
            {
                std::clog << "File does not exist: " << page.string() << std::endl;
            }

            std::cout << newLine() << "----------------------------------------------------------------------------------" << newLine() << std::endl;
        }
    }

    std::string errCtrStr = "Errors in " + std::to_string(mFileErrCtr) + "/" + std::to_string(mFileCtr) + " files!";

    errCtrStr = (mFileErrCtr == 0u) ? printGreen(errCtrStr) : printRed(errCtrStr);

    std::cout << errCtrStr << std::endl;

    // std::cout << "Print parsed library" << std::endl;

    // for(const auto& package : mLibrary.packages)
    // {
    //     if(package.properties.at(0).name == "VDD")
    //     {
    //         std::cout << package << std::endl;
    //     }
    // }

    // std::cout << mLibrary.symbolsLibrary << std::endl;

    // std::cout << mLibrary << std::endl;

    return mLibrary;
}


void Parser::exceptionHandling()
{
    try
    {
        throw;
    }
    catch(const std::exception& e)
    {
        ++mFileErrCtr;

        spdlog::error(printRed("--------ERROR REPORT--------"));
        spdlog::error(printRed("File: " + mCurrOpenFile.string()));
        mDs.printCurrentOffset(std::cerr);
        spdlog::error("");
        spdlog::error(printRed("Error Message: " + newLine() + std::string(e.what())));
        spdlog::error("");
    }
    catch(...)
    {
        spdlog::error(printRed("--------ERROR REPORT--------"));
        spdlog::error(printRed("Unknown exception caugt!"));
        spdlog::error("");
    }
}


void Parser::parsePage()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    // readDevHelper();
    // return;

    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    mDs.printUnknownData(21, std::string(__func__) + " - 0");
    readPreamble();

    std::string name = mDs.readStringLenZeroTerm();

    std::string PageSize = mDs.readStringLenZeroTerm();

    time_t createDateTime = static_cast<time_t>(mDs.readUint32());
    time_t modifyDateTime = static_cast<time_t>(mDs.readUint32());

    std::cout << "createDateTime = " << DateTimeToStr(createDateTime) << " | modifyDateTime = " << DateTimeToStr(modifyDateTime) << std::endl;

    mDs.printUnknownData(16, std::string(__func__) + " - 1");

    uint32_t width  = mDs.readUint32();
    uint32_t height = mDs.readUint32();

    std::cout << "width = " << std::to_string(width) << " | height = " << std::to_string(height) << std::endl;

    uint32_t pinToPin = mDs.readUint32(); //!< Pin-to-pin spacing

    std::cout << "pinToPin = " << std::to_string(pinToPin) << std::endl;

    mDs.printUnknownData(2, std::string(__func__) + " - 2");

    uint16_t horizontalCount = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint16_t verticalCount   = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    std::cout << "horizontalCount = " << std::to_string(horizontalCount) << " | verticalCount = " << std::to_string(verticalCount) << std::endl;

    mDs.printUnknownData(2, std::string(__func__) + " - 3");

    uint32_t horizontalWidth = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint32_t verticalWidth   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    std::cout << "horizontalWidth = " << std::to_string(horizontalWidth) << " | verticalWidth = " << std::to_string(verticalWidth) << std::endl;

    mDs.printUnknownData(48, std::string(__func__) + " - 4");

    uint32_t horizontalChar = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                             //             'Horizontal' -> 'Alphabetic' = 1
                                             //                             'Numeric'    = 0

    std::cout << "horizontalChar = " << std::to_string(horizontalChar) << std::endl;

    mDs.printUnknownData(4, std::string(__func__) + " - 5");

    uint32_t horizontalAscending = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                                  //         'Horizontal'
                                                  //         Select 'ascending' (1) or 'decending' (0).

    std::cout << "horizontalAscending = " << std::to_string(horizontalAscending) << std::endl;

    uint32_t verticalChar   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference' ->
                                             //       'Vertical' -> 'Alphabetic' = 1
                                             //                     'Numeric'    = 0

    std::cout << "verticalChar = " << std::to_string(verticalChar) << std::endl;

    mDs.printUnknownData(4, std::string(__func__) + " - 6");


    uint32_t verticalAscending = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                                  //         'Vertical'
                                                  //         Select 'ascending' (1) or 'decending' (0).

    std::cout << "verticalAscending = " << std::to_string(verticalAscending) << std::endl;

    uint32_t isMetric            = mDs.readUint32();
    uint32_t borderDisplayed     = mDs.readUint32();
    uint32_t borderPrinted       = mDs.readUint32();
    uint32_t gridRefDisplayed    = mDs.readUint32();
    uint32_t gridRefPrinted      = mDs.readUint32();
    uint32_t titleblockDisplayed = mDs.readUint32();
    uint32_t titleblockPrinted   = mDs.readUint32();
    // @todo XML is always 1, maybe a bug in OrCad?
    uint32_t ansiGridRefs = mDs.readUint32(); //!< Use ANSI grid references.
                                              //   See 'Schematic Page Properties' -> 'Grid Reference'

    spdlog::debug("isMetric            = {}", isMetric);
    spdlog::debug("borderDisplayed     = {}", borderDisplayed);
    spdlog::debug("borderPrinted       = {}", borderPrinted);
    spdlog::debug("gridRefDisplayed    = {}", gridRefDisplayed);
    spdlog::debug("gridRefPrinted      = {}", gridRefPrinted);
    spdlog::debug("titleblockDisplayed = {}", titleblockDisplayed);
    spdlog::debug("titleblockPrinted   = {}", titleblockPrinted);
    spdlog::debug("ansiGridRefs        = {}", ansiGridRefs);

    const uint16_t lenA = mDs.readUint16();

    std::cout << "lenA = " << std::to_string(lenA) << std::endl;

    for(size_t i = 0u; i < lenA; ++i)
    {
        mDs.printUnknownData(8, std::string(__func__) + " - a");
    }

    const uint16_t len0 = mDs.readUint16();

    std::cout << "len0 = " << std::to_string(len0) << std::endl;

    for(size_t i = 0u; i < len0; ++i)
    {
        mDs.printUnknownData(32, std::string(__func__) + " - 8");
    }

    // mDs.printUnknownData(30, std::string(__func__) + " - 7");


    // mDs.printUnknownData(38, std::string(__func__) + " - 1.5");

    // @todo required for CONTENT page but not for the others? This offset must be somehow
    //       dynamic
    // mDs.printUnknownData(14, std::string(__func__) + " - 1.6");





    mDs.printUnknownData(2, std::string(__func__) + " - 9");

    const uint16_t len1 = mDs.readUint16();

    std::cout << "len1 = " << std::to_string(len1) << std::endl;

    for(size_t i = 0u; i < len1; ++i)
    {
        std::string name = mDs.readStringLenZeroTerm();
        mDs.printUnknownData(4, std::string(__func__) + " - 10");
    }

    const uint16_t len2 = mDs.readUint16();

    std::cout << "len2 = " << std::to_string(len2) << std::endl;

    for(size_t i = 0u; i < len2; ++i)
    {
        // if(i > 0u)
        // {
        //     readPreamble();
        //     mDs.printUnknownData(8, std::string(__func__) + " - 11");
        // }

        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push structure

        // structure = read_type_prefix();
        // readPreamble();
        // parseStructure(structure); // @todo push structure
    }

    const uint16_t len3 = mDs.readUint16();

    std::cout << "len3 = " << std::to_string(len3) << std::endl;

    for(size_t i = 0u; i < len3; ++i)
    {
        // if(i > 0u)
        // {
        //     readPreamble();
        //     mDs.printUnknownData(8, std::string(__func__) + " - 11");
        // }

        Structure structure;

        if(i == 0u)
        {
            // @todo this is type_prefix_very_long()
            mDs.printUnknownData(47, std::string(__func__) + " - 11");
            structure = ToStructure(0x0d); // Parse package instance for now until type_prefix_very_long is implemented
        }
        else
        {
            structure = read_type_prefix();
        }


        readPreamble();
        parseStructure(structure); // @todo push structure

        // structure = read_type_prefix();
        // readPreamble();
        // parseStructure(structure); // @todo push structure
    }

    // readPreamble();
    // mDs.printUnknownData(12, std::string(__func__) + " - 12");

    mDs.printUnknownData(10, std::string(__func__) + " - 10");

    const uint16_t lenX = mDs.readUint16();

    std::cout << "lenX = " << std::to_string(lenX) << std::endl;

    for(size_t i = 0u; i < lenX; ++i)
    {
        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push structure
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readPartInst()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    mDs.printUnknownData(8, std::string(__func__) + " - 0");

    std::string pkgName = mDs.readStringLenZeroTerm();

    uint32_t dbId = mDs.readUint32();

    mDs.printUnknownData(8, std::string(__func__) + " - 1");

    int16_t locX = mDs.readInt16();
    int16_t locY = mDs.readInt16();

    Color color = ToColor(mDs.readUint16()); // @todo educated guess

    mDs.printUnknownData(2, std::string(__func__) + " - 2");

    uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push struct
    }

    mDs.printUnknownData(1, std::string(__func__) + " - 3");

    std::string reference = mDs.readStringLenZeroTerm();

    mDs.printUnknownData(14, std::string(__func__) + " - 4");

    uint16_t len2 = mDs.readUint16();

    for(size_t i = 0u; i < len2; ++i)
    {
        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push struct
    }

    std::string sth1 = mDs.readStringLenZeroTerm(); // @todo needs verification

    mDs.printUnknownData(2, std::string(__func__) + " - 5");

    // @todo implement type_prefix_very_long
    mDs.printUnknownData(18, std::string(__func__) + " - 6");
    Structure structure = read_type_prefix_long();
    readPreamble();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readT0x10()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    mDs.printUnknownData(16, std::string(__func__) + " - 0");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


std::pair<Structure, std::any> Parser::parseStructure(Structure structure)
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::cout << "Parsing " << structure << std::endl;

    std::any parseStruct;

    switch(structure)
    {
        case Structure::SthInPages0:                            /*parseStruct =*/ readSthInPages0();            break;
        case Structure::Properties:                             parseStruct = readProperties();                 break;
        case Structure::PartInst:                               /*parseStruct =*/ readPartInst();               break;
        case Structure::T0x10:                                  /*parseStruct =*/ readT0x10();                  break;
        case Structure::WireScalar:                             /*parseStruct =*/ readWireScalar();             break;
        case Structure::GeoDefinition:          readPreamble(); parseStruct = parseGeometrySpecification();     break;
        case Structure::SymbolPinScalar:                        parseStruct = readSymbolPinScalar();            break;
        case Structure::SymbolPinBus:                           parseStruct = readSymbolPinBus();               break;
        case Structure::T0x1f:                                  parseStruct = readT0x1f();                      break;
        case Structure::PinIdxMapping:                          parseStruct = readPinIdxMapping();              break;
        case Structure::GlobalSymbol:           readPreamble(); parseStruct = parseGlobalSymbol();              break;
        case Structure::PortSymbol:             readPreamble(); parseStruct = parseSymbolHierarchic();          break;
        case Structure::OffPageSymbol:                          parseStruct = parseOffPageSymbol();             break;
        case Structure::SymbolDisplayProp:                      parseStruct = readSymbolDisplayProp();          break;
        case Structure::Alias:                                  /*parseStruct =*/ readAlias();                  break;
        case Structure::GraphicBoxInst:                         /*parseStruct =*/ readGraphicBoxInst();         break;
        case Structure::GraphicCommentTextInst:                 /*parseStruct =*/ readGraphicCommentTextInst(); break;
        case Structure::ERCSymbol:              readPreamble(); /*parseStruct =*/ readERCSymbol();              break;
        case Structure::PinShapeSymbol:         readPreamble(); parseStruct = readPinShapeSymbol();             break;
        default:
            std::string errorMsg = "Structure with value 0x" + ToHex(structure, 2)
                                + " is not implemented!";
            throw std::invalid_argument(errorMsg);
            break;
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    return std::make_pair(structure, parseStruct);
}


void Parser::discard_until_preamble()
{
    const int patternSize = 4;
    std::array<uint8_t, patternSize> buffer = {0};

    // Magic number specifying the beginning of a struct
    const std::array<uint8_t, patternSize> preamble = {0xff, 0xe4, 0x5c, 0x39};

    auto shift_left = [](std::array<uint8_t, patternSize>& buffer)
    {
        for(size_t i = 0u; i < buffer.size() - 1; ++i)
        {
            buffer[i] = buffer[i + 1];
        }
    };

    while(buffer != preamble)
    {
        shift_left(buffer);
        mDs.read(reinterpret_cast<char*>(buffer.data()) + buffer.size() - 1, 1);
    }

    // Put back the preamble such that it can be parsed in the next step
    for(size_t i = 0u; i < preamble.size(); ++i)
    {
        mDs.putback(preamble[preamble.size() - 1 - i]);
    }
}


Structure Parser::read_type_prefix_long()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const Structure typeId = ToStructure(mDs.readUint8());

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

    const Structure typeIdRep = read_type_prefix();

    if(typeId != typeIdRep)
    {
        throw std::runtime_error("typeId (" + std::to_string(static_cast<size_t>(typeId))
                                    + ") should be equal to typeIdRep ("
                                    + std::to_string(static_cast<size_t>(typeIdRep)) + ")!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    return typeId;
}


Structure Parser::read_type_prefix()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const Structure typeId = ToStructure(mDs.readUint8());

    // @todo In most cases this is the offset from the following typeIdRep to
    //       the beginning of the next read_type_prefix() where all the previous
    //       structures belong to this one.
    mByteOffset = mDs.readUint32();
    std::cout << std::string(__func__) << " - 0 | mByteOffset = " << std::to_string(mByteOffset) << std::endl;
    // mDs.printUnknownData(4, std::string(__func__) + " - 0");

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

    const Structure typeIdRep = read_type_prefix_short();

    if(typeId != typeIdRep)
    {
        throw std::runtime_error("typeId (" + std::to_string(static_cast<size_t>(typeId))
                                    + ") should be equal to typeIdRep ("
                                    + std::to_string(static_cast<size_t>(typeIdRep)) + ")!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    return typeId;
}


Structure Parser::read_type_prefix_short()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const Structure typeId = ToStructure(mDs.readUint8());

    // const size_t startOffset = mDs.getCurrentOffset();

    // @todo this is probably not a lenght but specifies some attribute e.g. locked/not-locked
    //       0x0b = not-locked
    //       0x1e = locked
    const uint32_t byteLength = mDs.readUint32();
    if(byteLength != 0x0b && byteLength != 0x1e)
    {
        // throw std::runtime_error("Unexpected lock value 0x" + ToHex(byteLength, 2));
    }

    mDs.printUnknownData(4, std::string(__func__) + " - 0");
    // mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    const Structure typeIdRep = ToStructure(mDs.readUint8());

    const int16_t size = mDs.readInt16();
    std::cout << std::string(__func__) << " - 1 | typeId = " << to_string(typeId) << std::endl;
    std::cout << std::string(__func__) << " - 2 | size   = " << std::to_string(size) << std::endl;

    if(size >= 0)
    {
        // @todo This vector needs to be stored in the package, therefore return it to caller
        std::vector<std::pair<uint32_t, uint32_t> > nameValueMapping; //!< .first = Name Index, .second = Value Index

        for(int i = 0; i < size; ++i)
        {
            uint32_t strLstIdxName  = mDs.readUint32();
            uint32_t strLstIdxValue = mDs.readUint32();

            nameValueMapping.push_back(std::make_pair(strLstIdxName, strLstIdxValue));
        }

        for(size_t i = 0u; i < nameValueMapping.size(); ++i)
        {
            try
            {
                const auto getStr = [&, this](uint32_t idx) -> std::string
                    {
                        int64_t newIdx = static_cast<int64_t>(idx) - 1;
                        return newIdx >= 0 ? mLibrary.symbolsLibrary.strLst.at(newIdx) : "";
                    };

                std::cout << "  " << std::to_string(i) << ": " << getStr(nameValueMapping.at(i).first) << " <- " << getStr(nameValueMapping.at(i).second) << std::endl;
            }
            catch(const std::exception& e)
            {
                throw std::out_of_range("Tried to access strLst out of range!" + newLine() + std::string(e.what()));
            }
        }
    }
    else // size < 0
    {
        // @todo Why is -1 used? The value 0 would also suffice...
        // Until now I only saw it for PinIdxMapping, Properties and SymbolDisplayProp
        std::cout << to_string(typeId) + ": What does " + std::to_string(size) + " mean?" << std::endl; // @todo Figure out
    }

    // if(mDs.getCurrentOffset() != startOffset + byteLength)
    // {
    //     throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    // }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    return typeId;
}


uint32_t Parser::readPreamble(bool readOptionalLen)
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    // Magic number specifying the beginning of a struct
    mDs.assumeData({0xff, 0xe4, 0x5c, 0x39}, std::string(__func__) + " - 0");

    const uint32_t optionalLen = readOptionalLen ? mDs.readUint32() : 0u;

    mDs.printUnknownData(optionalLen, std::string(__func__) + " - 1 | Correlates to locks");

    if(optionalLen > 0u)
    {
        // @todo Looks like this correlates to setting a lock for an object.
        std::cout << std::string(__func__) << ": Figure out when optionalLen is used! Currently it's 0x" << ToHex(optionalLen, 4) << std::endl;
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    return optionalLen;
}


// Looks like some structures require a preceeding preamble but not all.
uint32_t Parser::readConditionalPreamble(Structure structure, bool readOptionalLen)
{
    uint32_t optionalLen = 0u;

    if(requiresPreamble(structure))
    {
        optionalLen = readPreamble(readOptionalLen);
    }

    return optionalLen;
}


// @todo needs some way to push the results into the final object
void Parser::readGeometryStructure(GeometryStructure geometryStructure, GeometrySpecification* geometrySpecification)
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeometrySpecification container;

    switch(geometryStructure)
    {
        case GeometryStructure::Rect:         container.rects.push_back(readRect());                 break;
        case GeometryStructure::Line:         container.lines.push_back(readLine());                 break;
        case GeometryStructure::Arc:          container.arcs.push_back(readArc());                   break;
        case GeometryStructure::Ellipse:      container.ellipses.push_back(readEllipse());           break;
        case GeometryStructure::Polygon:      container.polygons.push_back(readPolygon());           break;
        case GeometryStructure::Polyline:     container.polylines.push_back(readPolyline());         break;
        case GeometryStructure::CommentText:  container.commentTexts.push_back(readCommentText());   break;
        case GeometryStructure::Bitmap:       container.bitmaps.push_back(readBitmap());             break;
        case GeometryStructure::SymbolVector: container.symbolVectors.push_back(readSymbolVector()); break;
        case GeometryStructure::Bezier:       container.beziers.push_back(readBezier());             break;
        default:
            throw std::runtime_error("GeometryStructure not yet implemented value 0x"
                                    + ToHex(geometryStructure, 4));
            break;
    }

    if(geometrySpecification != nullptr)
    {
        // @todo do not overwrite geometrySpecification but only push_back
        //       the entries from container
        *geometrySpecification = container;
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


// @todo Probably a wrapper for Inst (Instances)
void Parser::readSthInPages0()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    mDs.printUnknownData(6, std::string(__func__) + " - 0");
    mDs.printUnknownData(4, std::string(__func__) + " - 1");

    const uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        GeometryStructure geometryStructure1 = ToGeometryStructure(mDs.readUint8());
        GeometryStructure geometryStructure2 = ToGeometryStructure(mDs.readUint8());

        if(geometryStructure1 != geometryStructure2)
        {
            throw std::runtime_error("Geometry structures should be equal!");
        }

        readGeometryStructure(geometryStructure1, nullptr); // @todo write output to structure
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readGraphicCommentTextInst()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    mDs.printUnknownData(34, std::string(__func__) + " - 0");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readWireScalar()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    uint32_t dbId = mDs.readUint32();

    std::cout << "dbId = " << std::to_string(dbId) << std::endl;

    mDs.printUnknownData(4, std::string(__func__) + " - 0");

    Color wireColor = ToColor(mDs.readUint32());

    int32_t startX = mDs.readInt32();
    int32_t startY = mDs.readInt32();
    int32_t endX   = mDs.readInt32();
    int32_t endY   = mDs.readInt32();

    std::cout << "startX = " << std::to_string(startX) << " | "
              << "startY = " << std::to_string(startY) << " | "
              << "endX = " << std::to_string(endX) << " | "
              << "endY = " << std::to_string(endY) << " | " << std::endl;

    mDs.printUnknownData(1, std::string(__func__) + " - 1");

    std::cout << "mByteOffset = " << mByteOffset << std::endl;

    if(mByteOffset == 0x3d)
    {
        mDs.printUnknownData(2, std::string(__func__) + " - 2");
    }
    else if(mByteOffset > 0x3d)
    {
        const uint16_t len = mDs.readUint16();

        std::cout << "len = " << std::to_string(len) << std::endl;

        for(size_t i = 0u; i < len; ++i)
        {
            // @todo len should always be 1 and the read structure should be 'Alias'
            Structure structure = read_type_prefix();
            readPreamble();
            parseStructure(structure); // @todo push
        }
    }

    mDs.printUnknownData(2, std::string(__func__) + " - 3");

    LineWidth wireLineWidth = ToLineWidth(mDs.readUint32());
    LineStyle wireLineStyle = ToLineStyle(mDs.readUint32());

    std::cout << "wireLineWidth = " << wireLineWidth << " | "
              << "wireLineStyle = " << wireLineStyle << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readAlias()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    int32_t locX = mDs.readInt32();
    int32_t locY = mDs.readInt32();

    std::cout << "locX = " << std::to_string(locX) << " | "
              << "locY = " << std::to_string(locY) << std::endl;

    Color color = ToColor(mDs.readUint32());

    std::cout << "color = " << color << std::endl;

    Rotation rotation = ToRotation(mDs.readUint32()); // @todo Why is it 4 byte? Probably increase Rotation size

    std::cout << "rotation = " << rotation << std::endl;

    uint16_t textFontIdx = mDs.readUint16(); // @todo educated guess
    std::cout << "Alias fontIdx = " << std::to_string(textFontIdx) << std::endl;

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    std::string name = mDs.readStringLenZeroTerm();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


// @todo is this a specialized instance for Rects or general for all types?
void Parser::readGraphicBoxInst()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    mDs.printUnknownData(11, std::string(__func__) + " - 0");

    uint32_t dbId = mDs.readUint32();

    int16_t locY = mDs.readInt16();
    int16_t locX = mDs.readInt16();

    int16_t y2 = mDs.readInt16();
    int16_t x2 = mDs.readInt16();

    int16_t x1 = mDs.readInt16();
    int16_t y1 = mDs.readInt16();

    Color color = ToColor(mDs.readUint16()); // @todo is it really not a 4 byte value?

    mDs.printUnknownData(5, std::string(__func__) + " - 1");

    // @todo Only Rect as a shape would make sense here. Maybe this should be passed
    //       as a parameter to readSthInPages0 to check this condition. Further,
    //       parseStructure should always call readSthInPages0.
    Structure structure = read_type_prefix_long();
    readPreamble();
    parseStructure(structure);

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readDevHelper()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    mDs.discardBytes(0x2a1);

    read_type_prefix();
    readPreamble();

    readGraphicBoxInst();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::checkInterpretedDataLen(const std::string& aFuncName, size_t aStartOffset, size_t aEndOffset, size_t aExpectedLen)
{
    if(aStartOffset > aEndOffset)
    {
        throw std::invalid_argument("Start offset musst be smaller or equal to end offset!");
    }

    const size_t actuallLen = aEndOffset - aStartOffset;

    if(actuallLen != aExpectedLen)
    {
        throw MisinterpretedData(aFuncName.c_str(), aStartOffset, aExpectedLen, aEndOffset);
    }
}


SymbolVector Parser::readSymbolVector()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolVector symbolVector;

    const auto readSmallTypePrefix = [&, this]() -> GeometryStructure
        {
            GeometryStructure structure = ToGeometryStructure(mDs.readUint8());
            mDs.assumeData({0x00}, std::string(__func__) + " - 0");
            mDs.assumeData({static_cast<uint8_t>(structure)}, std::string(__func__) + " - 1");

            return structure;
        };

    // mDs.printUnknownData(20, std::string(__func__) + " - x");
    // read_type_prefix();

    discard_until_preamble();
    readPreamble();

    symbolVector.locX = mDs.readInt16();
    symbolVector.locY = mDs.readInt16();

    uint16_t repetition = mDs.readUint16();

    for(size_t i = 0u; i < repetition; ++i)
    {
        if(i > 0u)
        {
            readPreamble();
        }

        readGeometryStructure(readSmallTypePrefix());
    }

    readPreamble();
    symbolVector.name = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, std::string(__func__) + " - 2");
    // mDs.printUnknownData(12, std::string(__func__) + " - 2");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolVector << std::endl;

    return symbolVector;
}


fs::path Parser::extractContainer(const fs::path& aFile, const fs::path& aOutDir) const
{
    ContainerExtractor extractor{aFile};

    return extractor.extract(aOutDir);
}


fs::path Parser::extractContainer(const fs::path& aOutDir) const
{
    return extractContainer(mInputFile, aOutDir);
}


void Parser::printContainerTree() const
{
    ContainerExtractor extractor{mInputFile};
    extractor.printContainerTree();
}


FileType Parser::getFileTypeByExtension(const fs::path& aFile) const
{
    std::string extension = aFile.extension().string();

    // Ignore case of extension
    std::transform(extension.begin(), extension.end(), extension.begin(),
        [] (unsigned char c) { return std::toupper(c); });

    const std::map<std::string, FileType> extensionFileTypeMap =
        {
            {".OLB", FileType::Library},
            {".OBK", FileType::Library},
            {".DSN", FileType::Schematic},
            {".DBK", FileType::Schematic}
        };

    FileType fileType;

    try
    {
        fileType = extensionFileTypeMap.at(extension);
    }
    catch(...)
    {
        throw std::runtime_error("Unknown file extension `" + extension + "`");
    }

    return fileType;
}


void Parser::openFile(const fs::path& aFile)
{
    std::cout << "Opening file: " << aFile << std::endl;

    mDs = DataStream{aFile};
    if(!mDs)
    {
        throw std::runtime_error("Could not open file: " + aFile.string());
    }

    mCurrOpenFile     = aFile;
    mCurrOpenFileSize = fs::file_size(aFile);

    std::cout << "File contains " << mCurrOpenFileSize << " byte." << std::endl;
}


void Parser::closeFile()
{
    std::cout << "Closing file: " << mCurrOpenFile << std::endl;

    mDs.close();

    mCurrOpenFile.clear();
    mCurrOpenFileSize = 0u;
}


PinIdxMapping Parser::readPinIdxMapping()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    PinIdxMapping pinIdxMapping;

    pinIdxMapping.unitRef = mDs.readStringLenZeroTerm();
    pinIdxMapping.refDes  = mDs.readStringLenZeroTerm();

    const uint16_t pinCount = mDs.readUint16();

    // @todo Add to kaitai file i = 'Order' of pin
    // See OrCAD: 'Pin Properties' -> 'Order'
    for(size_t i = 0u; i < pinCount; ++i)
    {
        pinIdxMapping.pinMap.push_back(mDs.readStringLenZeroTerm());

        const uint8_t separator = mDs.readUint8();

        // std::cout << "Sep = 0x" << ToHex(separator, 2) << std::endl;

        // @todo maybe this is not a separator but the additional property of the pin?
        // As soon as I add a property like NET_SHORT the separator changes from 0x7f to 0xaa
        // This is probably also affected by units and convert view.
        if(separator != 0x7f && separator != 0xaa && separator != 0xff)
        {
            throw std::runtime_error("Separator should be 0x" + ToHex(0x7f, 2) + " or 0x" + ToHex(0xaa, 2) + " or 0x" + ToHex(0xff, 2)
                                     + " but got 0x" + ToHex(separator, 2) + "!");
        }
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << pinIdxMapping << std::endl;

    return pinIdxMapping;
}


SymbolPinScalar Parser::readSymbolPinScalar()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolPinScalar symbolPinScalar;

    symbolPinScalar.name = mDs.readStringLenZeroTerm();

    symbolPinScalar.startX = mDs.readInt32();
    symbolPinScalar.startY = mDs.readInt32();
    symbolPinScalar.hotptX = mDs.readInt32();
    symbolPinScalar.hotptY = mDs.readInt32();

    symbolPinScalar.pinShape = ToPinShape(mDs.readUint16());

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    symbolPinScalar.portType = ToPortType(mDs.readUint32());

    mDs.printUnknownData(6, std::string(__func__) + " - 1");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolPinScalar << std::endl;

    return symbolPinScalar;
}


SymbolPinBus Parser::readSymbolPinBus()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolPinBus symbolPinBus;

    symbolPinBus.name = mDs.readStringLenZeroTerm();

    symbolPinBus.startX = mDs.readInt32();
    symbolPinBus.startY = mDs.readInt32();
    symbolPinBus.hotptX = mDs.readInt32();
    symbolPinBus.hotptY = mDs.readInt32();

    symbolPinBus.pinShape = ToPinShape(mDs.readUint16());

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    symbolPinBus.portType = ToPortType(mDs.readUint32());

    mDs.printUnknownData(6, std::string(__func__) + " - 1");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolPinBus << std::endl;

    return symbolPinBus;
}


SymbolDisplayProp Parser::readSymbolDisplayProp()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolDisplayProp symbolDisplayProp;

    symbolDisplayProp.nameIdx = mDs.readUint32();

    // @todo move to left shift operator
    // @bug The required string is not this one but the value of the associated property!!!! This is just the name of the property!!
    std::cout << "strLst Item = " << mLibrary.symbolsLibrary.strLst.at(symbolDisplayProp.nameIdx - 1) << std::endl;

    symbolDisplayProp.x = mDs.readInt16();
    symbolDisplayProp.y = mDs.readInt16();

    // @todo maybe using a bitmap is a cleaner solution than shifting bits
    const uint16_t packedStruct = mDs.readUint16();

    symbolDisplayProp.textFontIdx = packedStruct & 0xff; // Bit  7 downto  0

    if(symbolDisplayProp.textFontIdx > mLibrary.symbolsLibrary.textFonts.size())
    {
        throw std::out_of_range(std::string(__func__) + ": textFontIdx is out of range! Expected " +
            std::to_string(symbolDisplayProp.textFontIdx) + " <= " +
            std::to_string(mLibrary.symbolsLibrary.textFonts.size()) + "!");
    }

    // @todo The meaning of the bits in between is unknown
    std::clog << "Unknown bits in bitmap: " << std::to_string((packedStruct >> 8u) & 0x3f) << std::endl; // Bit 13 downto  8
    if(((packedStruct >> 8u) & 0x3f) != 0x00)
    {
        throw std::runtime_error("Some bits in the bitmap are used but what is the meaning of them?");
    }

    symbolDisplayProp.rotation = ToRotation(packedStruct >> 14u); // Bit 15 downto 14

    symbolDisplayProp.propColor = ToColor(mDs.readUint8());

    // Somehow relates to the visiblity of text. See show "Value if Value exist" and the other options
    //        Do not display
    // cc 01  Value only
    // 00 02  Name and value
    // 00 03  Name only
    // 00 04  Both if value exist
    //        Value if value exist
    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    mDs.assumeData({0x00}, std::string(__func__) + " - 1");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolDisplayProp << std::endl;

    return symbolDisplayProp;
}


// @todo implement return type and return it
void Parser::readERCSymbol()
{
    std::string name = mDs.readStringLenZeroTerm();

    // @todo Probably 'sourceLibName' which is a string but I'm not sure. Could also be the
    //       last part of the next unknown block
    mDs.printUnknownData(3, std::string(__func__) + " - 0");

    mDs.printUnknownData(4, std::string(__func__) + " - 1");

    uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        GeometryStructure geometryStructure1 = ToGeometryStructure(mDs.readUint8());
        GeometryStructure geometryStructure2 = ToGeometryStructure(mDs.readUint8());

        if(geometryStructure1 != geometryStructure2)
        {
            throw std::runtime_error("Geometry structures should be equal!");
        }

        readGeometryStructure(geometryStructure1, nullptr); // @todo push structure
    }

    // @todo not sure if this belongs into this structure and how do we know whether it
    //       is used or not? (BBox should be optional according to XSD)
    readPreamble();
    readSymbolBBox(); // @todo push structure
}


SymbolBBox Parser::readSymbolBBox()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolBBox symbolBBox;

    symbolBBox.x1 = mDs.readInt16();
    symbolBBox.y1 = mDs.readInt16();
    symbolBBox.x2 = mDs.readInt16();
    symbolBBox.y2 = mDs.readInt16();

    // @todo not sure weather this belongs to the structure or should be outside of it
    mDs.printUnknownData(4, std::string(__func__) + " - 0");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolBBox << std::endl;

    return symbolBBox;
}


// @todo Probably specifies the 'Package Properties'
T0x1f Parser::readT0x1f()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    T0x1f t0x1f;

    t0x1f.name = mDs.readStringLenZeroTerm();

    std::string unknownStr0 = mDs.readStringLenZeroTerm(); // @todo figure out
    std::cout << "readT0x1f unknownStr0 = " << unknownStr0 << std::endl;

    t0x1f.refDes = mDs.readStringLenZeroTerm();

    std::string unknownStr1 = mDs.readStringLenZeroTerm(); // @todo figure out
    std::cout << "readT0x1f unknownStr1 = " << unknownStr1 << std::endl;

    t0x1f.pcbFootprint = mDs.readStringLenZeroTerm();

    // Maybe the last two bytes specify the amount of units the symbols has?
    // Also called "Section Count"
    mDs.printUnknownData(2, std::string(__func__) + " - 0 - Prob. Unit Count");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << t0x1f << std::endl;

    return t0x1f;
}


// @todo create/update Kaitai file
GeneralProperties Parser::readGeneralProperties()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeneralProperties obj;

    // @todo move to kaitai file
    // doc: |
    //   Implementation path of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation Path'
    obj.implementationPath = mDs.readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Implementation of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation'
    obj.implementation = mDs.readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Reference descriptor for the symbol. E.g. 'R' for resistor.
    //   See OrCAD: 'Package Properties' -> 'Part Reference Prefix'
    obj.refDes = mDs.readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Value of the symbol. E.g. '10k' for a resistor.
    //   See OrCAD: 'Part Properties' -> 'Value'
    obj.partValue = mDs.readStringLenZeroTerm();

    const uint8_t properties = mDs.readUint8();

    // Expect that upper bits are unused => 00xx xxxxb
    if((properties & 0xc0) != 0x00)
    {
        throw std::runtime_error("Expected 00xx xxxxb but got 0x" + ToHex(properties & 0xc0, 2));
    }

    const uint8_t pinProperties      =  properties       & 0x07; // Get bits 2 down to 0
    const uint8_t implementationType = (properties >> 3) & 0x07; // Get bits 5 down to 3

    obj.pinNameVisible   =  static_cast<bool>((pinProperties & 0x01) >> 0); // Bit 0
    obj.pinNameRotate    =  static_cast<bool>((pinProperties & 0x02) >> 1); // Bit 1
    obj.pinNumberVisible = !static_cast<bool>((pinProperties & 0x04) >> 2); // Bit 2 - Note that this bit is inverted

    obj.implementationType = ToImplementationType(implementationType);

    mDs.printUnknownData(1, std::string(__func__) + " - 0");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}


Properties Parser::readProperties()
{
    // @todo this structure contains somehow .Normal and .Convert variants

    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Properties obj;

    obj.ref = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably string

    // @todo use enum for the view (normal/convert)
    const uint16_t viewNumber = mDs.readUint16(); // @todo I assume that this is the amount of views
                                               // the symbol has. Typically 1 (.Normal) or maybe
                                               // 2 with (.Normal and .Convert)
                                               // Add to obj

    switch(viewNumber)
    {
        case 1: // ".Normal"
            // Nothing to do
            break;

        case 2: // ".Convert"
            // @todo how to handle optional attributes in my structures?
            obj.convertName = mDs.readStringLenZeroTerm(); // @todo include into Kaitai file
            break;

        default:
            throw std::runtime_error("viewNumber is " + std::to_string(viewNumber) +
                " but it was expected that this can only take the value 1 or 2!");
            break;
    }

    obj.name = mDs.readStringLenZeroTerm();

    // This really looks like an TypePrefix! Maybe this property can be split up?
    mDs.printUnknownData(29, std::string(__func__) + " - 1");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}


Properties2 Parser::readProperties2()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Properties2 obj;

    obj.name = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably string

    obj.refDes = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 1"); // Unknown but probably string

    obj.footprint = mDs.readStringLenZeroTerm();

    obj.sectionCount = mDs.readUint16(); // @todo has this something to do with units? Or was this just bad naming from myself?

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}


std::vector<Type> Parser::parseTypes()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::vector<Type> types;

    // File can be completely empty (size of 0 byte)
    while(!mDs.isEoF())
    {
        Type type;

        type.name = mDs.readStringLenZeroTerm();
        type.componentType = ToComponentType(mDs.readUint16());

        types.push_back(type);
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    return types;
}


GeometrySpecification Parser::readSymbolProperties()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    return parseGeometrySpecification();
}


GeometrySpecification Parser::parseGeometrySpecification()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeometrySpecification obj;

    obj.name = mDs.readStringLenZeroTerm(); // @todo add to struct and Kaitai file

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably a string
    mDs.assumeData({0x30}, std::string(__func__) + " - 1");
    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 2"); // Unknown but probably a string

    const uint16_t geometryCount = mDs.readUint16();
    std::clog << "geometryCount = " << std::to_string(geometryCount) << std::endl;

    for(size_t i = 0u; i < geometryCount; ++i)
    {
        std::cout << "i of geometryCount = " << std::to_string(i) << std::endl;

        if(i > 0u)
        {
            if(mFileFormatVersion == FileFormatVersion::B)
            {
                read_type_prefix();
            }

            if(mFileFormatVersion >= FileFormatVersion::B)
            {
                readPreamble();
            }
        }

        GeometryStructure geometryStructure1 = ToGeometryStructure(mDs.readUint8());
        GeometryStructure geometryStructure2 = ToGeometryStructure(mDs.readUint8());

        if(geometryStructure1 != geometryStructure2)
        {
            throw std::runtime_error("Geometry structures should be equal!");
        }

        auto geoStruct = geometryStructure1;

        readGeometryStructure(geoStruct, &obj);

        // uint16_t foo = mDs.readUint8();
        // foo = (foo << 8) | foo;
        // geoStruct = ToGeometryStructure(foo);

        // mDs.printUnknownData(40, std::string(__func__) + " - 1");
        // readPreamble();

        if(mFileFormatVersion == FileFormatVersion::A)
        {
            mDs.printUnknownData(8, std::string(__func__) + " - 3.5");
        }
    }

    if(geometryCount == 0u)
    {
        // throw std::runtime_error("CatchMeIfYouCan");
        // mDs.printUnknownData(6, std::string(__func__) + " - 4");
    }

    // if(geometryCount == 0u)
    // {
    //     mDs.printUnknownData(10, std::string(__func__) + " - 3");

    //     {
    //         GeometryStructure geoStruct;

    //         Structure structure = read_type_prefix();
    //         readConditionalPreamble(structure);
    //         parseStructure(structure);

    //         const uint16_t followingLen1 = mDs.readUint16();

    //         for(size_t i = 0u; i < followingLen1; ++i)
    //         {
    //             std::clog << "0x" << ToHex(mDs.getCurrentOffset(), 8) << ": followingLen1 Iteration "
    //                     << std::to_string(i + 1) << "/" << std::to_string(followingLen1) << std::endl;

    //             structure = read_type_prefix();
    //             readConditionalPreamble(structure);
    //             // readPreamble();
    //             parseStructure(structure);
    //         }

    //         mDs.printUnknownData(24, "foo");

    //         structure = read_type_prefix();
    //         readConditionalPreamble(structure);
    //         parseStructure(structure);

    //         structure = read_type_prefix();
    //         readConditionalPreamble(structure);
    //         parseStructure(structure);

    //         // geoStruct = ToGeometryStructure(mDs.readUint16());

    //         // uint16_t foo = mDs.readUint8();
    //         // foo = (foo << 8) | foo;
    //         // geoStruct = ToGeometryStructure(foo);
    //         // readGeometryStructure(geoStruct, &obj);
    //     }
    // }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}