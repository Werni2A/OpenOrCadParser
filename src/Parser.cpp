#include <algorithm>
#include <any>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Parser.hpp"
#include "ContainerExtractor.hpp"
#include "DataStream.hpp"

#include "Structures/Arc.hpp"
#include "Structures/Point.hpp"
#include "Structures/Ellipse.hpp"
#include "Enums/FillStyle.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Enums/GeometryStructure.hpp"
#include "Enums/HatchStyle.hpp"
#include "Structures/Line.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Structures/SymbolPinBus.hpp"
#include "Structures/SymbolPinScalar.hpp"
#include "PinShape.hpp"
#include "Structures/Polygon.hpp"
#include "Enums/PortType.hpp"
#include "Structures/Rect.hpp"
#include "Structures/Structure.hpp"

#include "Structures/T0x1f.hpp"
#include "Structures/SymbolDisplayProp.hpp"

#include "Structures/SymbolsLibrary.hpp"

#include "Enums/Rotation.hpp"
#include "Exception.hpp"


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

        std::cerr << printRed("--------ERROR REPORT--------") << newLine();
        std::cerr << printRed("File: " + mCurrOpenFile.string() + newLine());
        std::cerr << printRed(printCurrentOffset() + newLine());
        std::cerr << printRed("Error Message: " + newLine() + std::string(e.what()) + newLine());
        std::cerr << std::endl;
    }
    catch(...)
    {
        std::cerr << printRed("--------ERROR REPORT--------") << newLine();
        std::cerr << printRed("Unknown exception caugt!") << newLine();
        std::cerr << std::endl;
    }
}


void Parser::parsePage()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    // readDevHelper();
    // return;

    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::clog << print_unknown_data(21, std::string(__func__) + " - 0") << std::endl;
    readPreamble();

    std::string name = readStringBothTerm();

    std::string PageSize = readStringBothTerm();

    time_t createDateTime = static_cast<time_t>(mDs.readUint32());
    time_t modifyDateTime = static_cast<time_t>(mDs.readUint32());

    std::cout << "createDateTime = " << DateTimeToStr(createDateTime) << " | modifyDateTime = " << DateTimeToStr(modifyDateTime) << std::endl;

    std::clog << print_unknown_data(16, std::string(__func__) + " - 1") << std::endl;

    uint32_t width  = mDs.readUint32();
    uint32_t height = mDs.readUint32();

    std::cout << "width = " << std::to_string(width) << " | height = " << std::to_string(height) << std::endl;

    uint32_t pinToPin = mDs.readUint32(); //!< Pin-to-pin spacing

    std::cout << "pinToPin = " << std::to_string(pinToPin) << std::endl;

    std::clog << print_unknown_data(2, std::string(__func__) + " - 2") << std::endl;

    uint16_t horizontalCount = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint16_t verticalCount   = mDs.readUint16(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    std::cout << "horizontalCount = " << std::to_string(horizontalCount) << " | verticalCount = " << std::to_string(verticalCount) << std::endl;

    std::clog << print_unknown_data(2, std::string(__func__) + " - 3") << std::endl;

    uint32_t horizontalWidth = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint32_t verticalWidth   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference'

    std::cout << "horizontalWidth = " << std::to_string(horizontalWidth) << " | verticalWidth = " << std::to_string(verticalWidth) << std::endl;

    std::clog << print_unknown_data(48, std::string(__func__) + " - 4") << std::endl;

    uint32_t horizontalChar = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                             //             'Horizontal' -> 'Alphabetic' = 1
                                             //                             'Numeric'    = 0

    std::cout << "horizontalChar = " << std::to_string(horizontalChar) << std::endl;

    std::clog << print_unknown_data(4, std::string(__func__) + " - 5") << std::endl;

    uint32_t horizontalAscending = mDs.readUint32(); //!<       See 'Schematic Page Properties' -> 'Grid Reference' ->
                                                  //         'Horizontal'
                                                  //         Select 'ascending' (1) or 'decending' (0).

    std::cout << "horizontalAscending = " << std::to_string(horizontalAscending) << std::endl;

    uint32_t verticalChar   = mDs.readUint32(); //!< See 'Schematic Page Properties' -> 'Grid Reference' ->
                                             //       'Vertical' -> 'Alphabetic' = 1
                                             //                     'Numeric'    = 0

    std::cout << "verticalChar = " << std::to_string(verticalChar) << std::endl;

    std::clog << print_unknown_data(4, std::string(__func__) + " - 6") << std::endl;


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
    uint32_t ansiGridRefs = mDs.readUint32(); //!<       Use ANSI grid references.
                                            //   See 'Schematic Page Properties' -> 'Grid Reference'


    std::cout << "isMetric = " << std::to_string(isMetric) << std::endl;
    std::cout << "borderDisplayed = " << std::to_string(borderDisplayed) << std::endl;
    std::cout << "borderPrinted = " << std::to_string(borderPrinted) << std::endl;
    std::cout << "gridRefDisplayed = " << std::to_string(gridRefDisplayed) << std::endl;
    std::cout << "gridRefPrinted = " << std::to_string(gridRefPrinted) << std::endl;
    std::cout << "titleblockDisplayed = " << std::to_string(titleblockDisplayed) << std::endl;
    std::cout << "titleblockPrinted = " << std::to_string(titleblockPrinted) << std::endl;
    std::cout << "ansiGridRefs = " << std::to_string(ansiGridRefs) << std::endl;

    const uint16_t lenA = mDs.readUint16();

    std::cout << "lenA = " << std::to_string(lenA) << std::endl;

    for(size_t i = 0u; i < lenA; ++i)
    {
        std::clog << print_unknown_data(8, std::string(__func__) + " - a") << std::endl;
    }

    const uint16_t len0 = mDs.readUint16();

    std::cout << "len0 = " << std::to_string(len0) << std::endl;

    for(size_t i = 0u; i < len0; ++i)
    {
        std::clog << print_unknown_data(32, std::string(__func__) + " - 8") << std::endl;
    }

    // std::clog << print_unknown_data(30, std::string(__func__) + " - 7") << std::endl;


    // std::clog << print_unknown_data(38, std::string(__func__) + " - 1.5") << std::endl;

    // @todo required for CONTENT page but not for the others? This offset must be somehow
    //       dynamic
    // std::clog << print_unknown_data(14, std::string(__func__) + " - 1.6") << std::endl;





    std::clog << print_unknown_data(2, std::string(__func__) + " - 9") << std::endl;

    const uint16_t len1 = mDs.readUint16();

    std::cout << "len1 = " << std::to_string(len1) << std::endl;

    for(size_t i = 0u; i < len1; ++i)
    {
        std::string name = readStringBothTerm();
        std::clog << print_unknown_data(4, std::string(__func__) + " - 10") << std::endl;
    }

    const uint16_t len2 = mDs.readUint16();

    std::cout << "len2 = " << std::to_string(len2) << std::endl;

    for(size_t i = 0u; i < len2; ++i)
    {
        // if(i > 0u)
        // {
        //     readPreamble();
        //     std::cout << print_unknown_data(8, std::string(__func__) + " - 11") << std::endl;
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
        //     std::cout << print_unknown_data(8, std::string(__func__) + " - 11") << std::endl;
        // }

        Structure structure;

        if(i == 0u)
        {
            // @todo this is type_prefix_very_long()
            std::cout << print_unknown_data(47, std::string(__func__) + " - 11") << std::endl;
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
    // std::cout << print_unknown_data(12, std::string(__func__) + " - 12") << std::endl;

    std::cout << print_unknown_data(10, std::string(__func__) + " - 10") << std::endl;

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

    std::clog << print_unknown_data(8, std::string(__func__) + " - 0") << std::endl;

    std::string pkgName = readStringBothTerm();

    uint32_t dbId = mDs.readUint32();

    std::clog << print_unknown_data(8, std::string(__func__) + " - 1") << std::endl;

    int16_t locX = mDs.readInt16();
    int16_t locY = mDs.readInt16();

    Color color = ToColor(mDs.readUint16()); // @todo educated guess

    std::clog << print_unknown_data(2, std::string(__func__) + " - 2") << std::endl;

    uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push struct
    }

    std::clog << print_unknown_data(1, std::string(__func__) + " - 3") << std::endl;

    std::string reference = readStringBothTerm();

    std::clog << print_unknown_data(14, std::string(__func__) + " - 4") << std::endl;

    uint16_t len2 = mDs.readUint16();

    for(size_t i = 0u; i < len2; ++i)
    {
        Structure structure = read_type_prefix();
        readPreamble();
        parseStructure(structure); // @todo push struct
    }

    std::string sth1 = readStringBothTerm(); // @todo needs verification

    std::clog << print_unknown_data(2, std::string(__func__) + " - 5") << std::endl;

    // @todo implement type_prefix_very_long
    std::clog << print_unknown_data(18, std::string(__func__) + " - 6") << std::endl;
    Structure structure = read_type_prefix_long();
    readPreamble();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readT0x10()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::clog << print_unknown_data(16, std::string(__func__) + " - 0") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


TextFont Parser::readTextFont()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    TextFont textFont;

    textFont.height = mDs.readInt32();
    textFont.width  = mDs.readInt32();

    textFont.escapement = mDs.readUint16(); // @todo not sure

    std::clog << print_unknown_data(6, std::string(__func__) + " - 0") << std::endl;

    textFont.weight = mDs.readUint16();

    std::clog << print_unknown_data(2, std::string(__func__) + " - 1") << std::endl;

    textFont.italic = mDs.readUint16(); // @todo not sure

    std::clog << print_unknown_data(6, std::string(__func__) + " - 2") << std::endl;

    textFont.fontName = readStringZeroTerm();
    // Looks like OrCAD creates a fixed size buffer where the string
    // is copied into. However, when the string does not requrie the
    // full buffer size it contains still data from the previous
    // application. When the buffer is written into the file this
    // probably causes some privacy issues as the data at the end
    // of the buffer should never end up in the symbols file.
    // Some OrCAD code like this would cause the issue:
    // char buffer[60];
    // strcpy(buffer, srcStr);
    // write_data_to_file(buffer, sizeof(buffer));
    padRest(startOffset, 60);

    static size_t ctr = 0u;
    std::cout << "Index = " << std::to_string(ctr) << std::endl;
    ctr++;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << textFont << std::endl;

    return textFont;
}


std::string Parser::printCurrentOffset()
{
    unsigned offset = mDs.getCurrentOffset();
    char buffer[128];
    std::sprintf(buffer, "Offset at 0x%s", ToHex(offset, 8).c_str());
    return std::string(buffer);
};


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

    const std::array<uint8_t, patternSize> preamble = {0xff, 0xe4, 0x5c, 0x39}; // Magic number for beginning of struct

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


void Parser::discard_bytes(size_t discard)
{
    mDs.seekg(discard, std::ios_base::cur);
}


std::vector<uint8_t> Parser::read_bytes(size_t amount)
{
    std::vector<uint8_t> data;

    for(size_t i = 0u; i < amount; ++i)
    {
        data.push_back(mDs.readUint8());
    }

    return data;
}


std::string Parser::print_data(const std::vector<uint8_t>& data)
{
    const unsigned int line_width = 16u;
    const std::string hex_spacing = " ";
    const std::string str_spacing = "";

    std::string preamble;
    std::string line_hex;
    std::string line_str;

    std::string output;

    std::string line;
    for(size_t i = 0u; i < data.size(); ++i)
    {
        if(i % line_width == 0u)
        {
            preamble = "0x" + ToHex(mDs.getCurrentOffset(), 8) + ": ";
        }

        char c = static_cast<char>(data[i]);
        line_hex += ToHex(data[i], 2);
        line_str += std::isprint(c) ? c : '.';

        if((i + 1) % line_width == 0u)
        {
            output += preamble + line_hex + " | " + line_str + newLine();
            preamble.clear();
            line_hex.clear();
            line_str.clear();
        }
        else if(i == data.size() - 1)
        {
            for(size_t i = 0u; i < line_width - (data.size() % line_width); ++i)
            {
                line_hex += "  " + hex_spacing; // 2 spaces for 1 byte
                line_str += " "  + str_spacing; // 1 character for 1 byte
            }

            output += preamble + line_hex + " | " + line_str + newLine();
        }
        else
        {
            line_hex += hex_spacing;
            line_str += str_spacing;
        }
    }

    return output;
}


void Parser::assume_data(const std::vector<uint8_t>& expectedData, const std::string& comment)
{
    const std::vector<uint8_t> data = read_bytes(expectedData.size());

    size_t ctr = 0u;

    const auto checkByte = [&ctr, &expectedData](uint8_t byte)
    {
        return byte == expectedData[ctr++];
    };

    if(!std::all_of(data.cbegin(), data.cend(), checkByte))
    {
        throw std::runtime_error("Assumption failed: " + comment + newLine() + "Expected:" + newLine() + print_data(expectedData) + "but got:" + newLine() + print_data(data));
    }
}


std::string Parser::readStringBothTerm()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    uint16_t len = mDs.readUint16();

    std::string str = readStringZeroTerm();

    if(str.length() != len)
    {
        throw std::runtime_error("Zero terminated string lenght (" + std::to_string(str.length())
                                 + ") does not match the preceeding length ("
                                 + std::to_string(len) + ") definition!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    return str;
}


std::string Parser::readStringZeroTerm()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::string str;

    const size_t max_chars = 400u;

    size_t i;
    for(i = 0u; i < max_chars; ++i)
    {
        char c = mDs.get();

        if(c != '\0')
        {
            str += c;
        }
        else
        {
            break;
        }
    }

    // @todo This check was added during testing to such that the parser fails when trying to
    //       interpret random data as string. Remove this check as soon as the parser works
    //       stable because strings can actually be very long.
    if(i == max_chars)
    {
        throw std::runtime_error("Loop cancled because the string is unexpectedly large, more than "
                                 + std::to_string(max_chars) + " characters!");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << str << std::endl;

    return str;
}


void Parser::padRest(size_t startOffset, size_t blockSize)
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    size_t currOffset = mDs.getCurrentOffset();
    size_t offsetDiff = currOffset - startOffset;

    if(offsetDiff > blockSize)
    {
        throw std::runtime_error("Already parsed " + std::to_string(offsetDiff)
                                 + " bytes but should have only been "
                                 + std::to_string(blockSize) + "!");
    }

    size_t paddingSize = blockSize - offsetDiff;

    std::cout << print_unknown_data(paddingSize, std::string(__func__) + " - 0") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


Structure Parser::read_type_prefix_long()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const Structure typeId = ToStructure(mDs.readUint8());

    std::cout << print_unknown_data(2, std::string(__func__) + " - 0") << std::endl;

    assume_data({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

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
    // std::cout << print_unknown_data(4, std::string(__func__) + " - 0") << std::endl;

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

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

    std::cout << print_unknown_data(4, std::string(__func__) + " - 0") << std::endl;;
    // assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

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

    // Magic constant
    assume_data({0xff, 0xe4, 0x5c, 0x39}, std::string(__func__) + " - 0");

    const uint32_t optionalLen = readOptionalLen ? mDs.readUint32() : 0u;

    std::cout << print_unknown_data(optionalLen, std::string(__func__) + " - 1 | Correlates to locks") << std::endl;

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


std::string Parser::print_unknown_data(size_t amount, const std::string& comment)
{
    const auto data = read_bytes(amount);

    std::string out;

    if(amount > 0u)
    {
        // out = printYellow("Unknown data in " + mCurrOpenFile + ":" + newLine()
        //     + comment + newLine() + print_data(data));
        out = printYellow(comment + newLine() + print_data(data));
    }

    return out;
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

    std::cout << print_unknown_data(6, std::string(__func__) + " - 0") << std::endl;
    std::cout << print_unknown_data(4, std::string(__func__) + " - 1") << std::endl;

    const uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        GeometryStructure geometryStructure = ToGeometryStructure(mDs.readUint16());

        readGeometryStructure(geometryStructure, nullptr); // @todo write output to structure
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readGraphicCommentTextInst()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::cout << print_unknown_data(34, std::string(__func__) + " - 0") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


void Parser::readWireScalar()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    uint32_t dbId = mDs.readUint32();

    std::cout << "dbId = " << std::to_string(dbId) << std::endl;

    std::cout << print_unknown_data(4, std::string(__func__) + " - 0") << std::endl;

    Color wireColor = ToColor(mDs.readUint32());

    int32_t startX = mDs.readInt32();
    int32_t startY = mDs.readInt32();
    int32_t endX   = mDs.readInt32();
    int32_t endY   = mDs.readInt32();

    std::cout << "startX = " << std::to_string(startX) << " | "
              << "startY = " << std::to_string(startY) << " | "
              << "endX = " << std::to_string(endX) << " | "
              << "endY = " << std::to_string(endY) << " | " << std::endl;

    std::cout << print_unknown_data(1, std::string(__func__) + " - 1") << std::endl;

    std::cout << "mByteOffset = " << mByteOffset << std::endl;

    if(mByteOffset == 0x3d)
    {
        std::cout << print_unknown_data(2, std::string(__func__) + " - 2") << std::endl;
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

    std::cout << print_unknown_data(2, std::string(__func__) + " - 3") << std::endl;

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

    std::cout << print_unknown_data(2, std::string(__func__) + " - 0") << std::endl;

    std::string name = readStringBothTerm();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


// @todo is this a specialized instance for Rects or general for all types?
void Parser::readGraphicBoxInst()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    std::cout << print_unknown_data(11, std::string(__func__) + " - 0") << std::endl;

    uint32_t dbId = mDs.readUint32();

    int16_t locY = mDs.readInt16();
    int16_t locX = mDs.readInt16();

    int16_t y2 = mDs.readInt16();
    int16_t x2 = mDs.readInt16();

    int16_t x1 = mDs.readInt16();
    int16_t y1 = mDs.readInt16();

    Color color = ToColor(mDs.readUint16()); // @todo is it really not a 4 byte value?

    std::cout << print_unknown_data(5, std::string(__func__) + " - 1") << std::endl;

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

    discard_bytes(0x2a1);

    read_type_prefix();
    readPreamble();

    readGraphicBoxInst();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
}


Line Parser::readLine()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Line line;

    const uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");
    // std::cout << print_unknown_data(4, std::string(__func__) + " - 0") << std::endl;

    line.x1 = mDs.readInt32();
    line.y1 = mDs.readInt32();
    line.x2 = mDs.readInt32();
    line.y2 = mDs.readInt32();

    if(mFileFormatVersion >= FileFormatVersion::B)
    {
        line.lineStyle = ToLineStyle(mDs.readUint32());
        line.lineWidth = ToLineWidth(mDs.readUint32());
    }
    else
    {
        // Set default values
        line.lineStyle = LineStyle::Solid;
        line.lineWidth = LineWidth::Default;
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    uint32_t expectedByteLength;
    switch(mFileFormatVersion)
    {
        case FileFormatVersion::A:
            expectedByteLength = 24u; break;
        case FileFormatVersion::B:
        case FileFormatVersion::C:
            expectedByteLength = 32u; break;
        default:
            throw MissingFileFormatCheck(__func__, __LINE__, mFileFormatVersion); break;
    }

    if(byteLength != expectedByteLength)
    {
        throw FileFormatChanged("Line");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << line << std::endl;

    return line;
}


Point Parser::readPoint()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Point point;

    point.y = mDs.readUint16();
    point.x = mDs.readUint16();

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << point << std::endl;

    return point;
}


Ellipse Parser::readEllipse()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Ellipse ellipse;

    uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    ellipse.x1 = mDs.readInt32();
    ellipse.y1 = mDs.readInt32();
    ellipse.x2 = mDs.readInt32();
    ellipse.y2 = mDs.readInt32();

    if(mFileFormatVersion >= FileFormatVersion::A)
    {
        ellipse.lineStyle = ToLineStyle(mDs.readUint32());
        ellipse.lineWidth = ToLineWidth(mDs.readUint32());
    }
    else
    {
        // Set default values
        ellipse.lineStyle = LineStyle::Solid;
        ellipse.lineWidth = LineWidth::Default;
    }

    if(mFileFormatVersion >= FileFormatVersion::C)
    {
        ellipse.fillStyle  = ToFillStyle(mDs.readUint32());
        ellipse.hatchStyle = ToHatchStyle(mDs.readUint32());
    }
    else
    {
        // Set default values
        ellipse.fillStyle  = FillStyle::None;
        ellipse.hatchStyle = HatchStyle::LinesHorizontal;
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    uint32_t expectedByteLength;
    switch(mFileFormatVersion)
    {
        case FileFormatVersion::A:
            // expectedByteLength = 24u; break;
        case FileFormatVersion::B:
            expectedByteLength = 32u; break;
        case FileFormatVersion::C:
            expectedByteLength = 40u; break;
        default:
            throw MissingFileFormatCheck(__func__, __LINE__, mFileFormatVersion); break;
    }

    if(byteLength != expectedByteLength)
    {
        throw FileFormatChanged("Ellipse");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << ellipse << std::endl;

    return ellipse;
}


Polygon Parser::readPolygon()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Polygon polygon;

    const uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(mFileFormatVersion >= FileFormatVersion::B)
    {
        polygon.lineStyle = ToLineStyle(mDs.readUint32());
        polygon.lineWidth = ToLineWidth(mDs.readUint32());
    }
    else
    {
        // Set default values
        polygon.lineStyle = LineStyle::Solid;
        polygon.lineWidth = LineWidth::Default;
    }

    if(mFileFormatVersion >= FileFormatVersion::C)
    {
        polygon.fillStyle  = ToFillStyle(mDs.readUint32());
        polygon.hatchStyle = ToHatchStyle(mDs.readUint32());
    }
    else
    {
        // Default values
        polygon.fillStyle  = FillStyle::None;
        polygon.hatchStyle = HatchStyle::LinesHorizontal;
    }

    const uint16_t pointCount = mDs.readUint16();

    if(pointCount < 3u)
    {
        throw std::runtime_error("The XSD allows Polygons with < 3 points but does this make any sense? Got "
                                 + std::to_string(pointCount) + "!");
    }

    for(size_t i = 0u; i < pointCount; ++i)
    {
        polygon.points.push_back(readPoint());
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    uint32_t expectedByteLength;
    switch(mFileFormatVersion)
    {
        case FileFormatVersion::A:
            expectedByteLength = 10u + 4u * pointCount; break;
        case FileFormatVersion::B:
            expectedByteLength = 18u + 4u * pointCount; break;
        case FileFormatVersion::C:
            expectedByteLength = 26u + 4u * pointCount; break;
        default:
            throw MissingFileFormatCheck(__func__, __LINE__, mFileFormatVersion); break;
    }

    if(byteLength != expectedByteLength)
    {
        throw FileFormatChanged("Polygon");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << polygon << std::endl;

    return polygon;
}


Polyline Parser::readPolyline()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Polyline polyline;

    const uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(mFileFormatVersion >= FileFormatVersion::A)
    {
        polyline.lineStyle = ToLineStyle(mDs.readUint32());
        polyline.lineWidth = ToLineWidth(mDs.readUint32());
    }
    else
    {
        // Set default values
        polyline.lineStyle = LineStyle::Solid;
        polyline.lineWidth = LineWidth::Default;
    }

    const uint16_t pointCount = mDs.readUint16();

    if(pointCount < 2)
    {
        throw std::runtime_error("At minimum 2 'PolylinePoint's must occur but got " + std::to_string(pointCount)
                                 + " points!");
    }

    for(size_t i = 0u; i < pointCount; ++i)
    {
        polyline.points.push_back(readPoint());
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    uint32_t expectedByteLength;
    switch(mFileFormatVersion)
    {
        case FileFormatVersion::A:
            expectedByteLength = 10u + 4u * pointCount; break;
        case FileFormatVersion::B:
        case FileFormatVersion::C:
            expectedByteLength = 18u + 4u * pointCount; break;
        default:
            throw MissingFileFormatCheck(__func__, __LINE__, mFileFormatVersion); break;
    }

    if(byteLength != expectedByteLength)
    {
        throw FileFormatChanged("Polyline");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << polyline << std::endl;

    return polyline;
}



Bezier Parser::readBezier()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Bezier bezier;

    const uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(mFileFormatVersion >= FileFormatVersion::B)
    {
        bezier.lineStyle = ToLineStyle(mDs.readUint32());
        bezier.lineWidth = ToLineWidth(mDs.readUint32());
    }
    else
    {
        // Set default values
        bezier.lineStyle = LineStyle::Solid;
        bezier.lineWidth = LineWidth::Default;
    }

    const uint16_t pointCount = mDs.readUint16();

    if(pointCount < 4)
    {
        throw std::runtime_error("At minimum 4 'BezierPoint's must occur but got " + std::to_string(pointCount)
                                 + " points!");
    }
    else
    {
        // Check correct number of points.

        // Example showing the overlapping ends of the points
        // Bezier Segment 0: P0P1P2P3
        // Bezier Segment 1:       P0P1P2P3
        // Bezier Segment 2:             P0P1P2P3
        // Bezier Segment 3:                   P0P1P2P3

        // n_b = Number of Bezier segments
        // n_p = Number of BezierPoints

        //     3 * n_b + 1 = n_p
        // <=> n_b = (n_p - 1) / 3

        const auto isInteger = [](float k) -> bool
        {
            // @todo FP comparisons are a bad idea. Check for e.g. 10 maschine epsilon difference
            return std::floor(k) == k;
        };

        if(!isInteger((pointCount - 1.0) / 3.0))
        {
            throw std::runtime_error("Number of 'BezierPoint's is incorrect. Got " + std::to_string(pointCount)
                                    + " points!");
        }
    }

    for(size_t i = 0u; i < pointCount; ++i)
    {
        bezier.points.push_back(readPoint());
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    uint32_t expectedByteLength;
    switch(mFileFormatVersion)
    {
        case FileFormatVersion::A:
            expectedByteLength = 10u + 4u * pointCount; break;
        case FileFormatVersion::B:
        case FileFormatVersion::C:
            expectedByteLength = 18u + 4u * pointCount; break;
        default:
            throw MissingFileFormatCheck(__func__, __LINE__, mFileFormatVersion); break;
    }

    if(byteLength != expectedByteLength)
    {
        throw FileFormatChanged("Bezier");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << bezier << std::endl;

    return bezier;
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


Rect Parser::readRect()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Rect rect;

    const uint32_t byteLength = mDs.readUint32();

    // @todo better move this if-statement into Rect::checkByteLength(byteLength, version)
    if(byteLength != Rect::getExpectedByteLength(mFileFormatVersion))
    {
        throw FileFormatChanged("Rect");
    }

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    rect.x1 = mDs.readInt32();
    rect.y1 = mDs.readInt32();
    rect.x2 = mDs.readInt32();
    rect.y2 = mDs.readInt32();

    if(mFileFormatVersion >= FileFormatVersion::A)
    {
        rect.lineStyle = ToLineStyle(mDs.readUint32());
        rect.lineWidth = ToLineWidth(mDs.readUint32());
    }
    else
    {
        // Set default values
        rect.lineStyle = LineStyle::Solid;
        rect.lineWidth = LineWidth::Default;
    }

    if(mFileFormatVersion >= FileFormatVersion::C)
    {
        rect.fillStyle  = ToFillStyle(mDs.readUint32());
        rect.hatchStyle = ToHatchStyle(mDs.readUint32());
    }
    else
    {
        // Set default values
        rect.fillStyle  = FillStyle::None;
        rect.hatchStyle = HatchStyle::LinesHorizontal;
    }

    // @todo use for all read methods.
    checkInterpretedDataLen(__func__, startOffset, mDs.getCurrentOffset(), byteLength);

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << rect << std::endl;

    return rect;
}


SymbolVector Parser::readSymbolVector()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolVector symbolVector;

    const auto readSmallTypePrefix = [&, this]() -> GeometryStructure
        {
            uint8_t structure = mDs.readUint8();
            assume_data({0x00}, std::string(__func__) + " - 0");
            assume_data({structure}, std::string(__func__) + " - 1");

            GeometryStructure parsedStruct = ToGeometryStructure((static_cast<uint16_t>(structure) << 8) | structure);

            return parsedStruct;
        };

    // std::cout << print_unknown_data(20, std::string(__func__) + " - x");
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
    symbolVector.name = readStringBothTerm();

    assume_data({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, std::string(__func__) + " - 2");
    // std::cout << print_unknown_data(12, std::string(__func__) + " - 2") << std::endl;

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
    std::transform(extension.begin(), extension.end(), extension.begin(),
        [](unsigned char c){ return std::toupper(c); });

    FileType fileType;

    if(extension == ".OLB" ||
       extension == ".OBK")
    {
        fileType = FileType::Library;
    }
    else if(extension == ".DSN" ||
        extension == ".DBK")
    {
        fileType = FileType::Schematic;
    }
    else
    {
        throw std::runtime_error("Unknown file extension: " + extension);
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


Bitmap Parser::readBitmap()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Bitmap bitmap;

    const uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    bitmap.locX = mDs.readInt32();
    bitmap.locY = mDs.readInt32();

    bitmap.x2 = mDs.readInt32();
    bitmap.y2 = mDs.readInt32();
    bitmap.x1 = mDs.readInt32();
    bitmap.y1 = mDs.readInt32();

    bitmap.bmpWidth  = mDs.readUint32();
    bitmap.bmpHeight = mDs.readUint32();

    const uint32_t imgSize = mDs.readUint32();

    bitmap.rawImgData.clear();

    for(size_t i = 0u; i < imgSize; ++i)
    {
        bitmap.rawImgData.push_back(mDs.readUint8());
    }

    bitmap.writeBmpToFile("foo" + std::to_string(imgSize) + ".bmp");

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != 44u + imgSize)
    {
        throw FileFormatChanged("Bitmap");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << bitmap << std::endl;

    return bitmap;
}


CommentText Parser::readCommentText()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    CommentText commentText{&mLibrary};

    const uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    commentText.locX = mDs.readInt32();
    commentText.locY = mDs.readInt32();

    commentText.x2 = mDs.readInt32();
    commentText.y2 = mDs.readInt32();
    commentText.x1 = mDs.readInt32();
    commentText.y1 = mDs.readInt32();

    commentText.textFontIdx = mDs.readUint16();

    if(commentText.textFontIdx > mLibrary.symbolsLibrary.textFonts.size())
    {
        throw std::out_of_range(std::string(__func__) + ": textFontIdx is out of range! Expected " +
            std::to_string(commentText.textFontIdx) + " <= " +
            std::to_string(mLibrary.symbolsLibrary.textFonts.size()) + "!");
    }

    std::cout << print_unknown_data(2, std::string(__func__) + " - 0") << std::endl;

    commentText.name = readStringBothTerm();

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != 39u + commentText.name.size())
    {
        throw FileFormatChanged("CommentText");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << commentText << std::endl;

    return commentText;
}


PinIdxMapping Parser::readPinIdxMapping()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    PinIdxMapping pinIdxMapping;

    pinIdxMapping.unitRef = readStringBothTerm();
    pinIdxMapping.refDes  = readStringBothTerm();

    const uint16_t pinCount = mDs.readUint16();

    // @todo Add to kaitai file i = 'Order' of pin
    // See OrCAD: 'Pin Properties' -> 'Order'
    for(size_t i = 0u; i < pinCount; ++i)
    {
        pinIdxMapping.pinMap.push_back(readStringBothTerm());

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


Arc Parser::readArc()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Arc arc;

    const uint32_t byteLength = mDs.readUint32();

    assume_data({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    arc.x1 = mDs.readInt32();
    arc.y1 = mDs.readInt32();
    arc.x2 = mDs.readInt32();
    arc.y2 = mDs.readInt32();

    arc.startX = mDs.readInt32();
    arc.startY = mDs.readInt32();
    arc.endX   = mDs.readInt32();
    arc.endY   = mDs.readInt32();

    if(mFileFormatVersion >= FileFormatVersion::B)
    {
        arc.lineStyle = ToLineStyle(mDs.readUint32());
        arc.lineWidth = ToLineWidth(mDs.readUint32());
    }
    else
    {
        // Set default values
        arc.lineStyle = LineStyle::Solid;
        arc.lineWidth = LineWidth::Default;
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    uint32_t expectedByteLength;
    switch(mFileFormatVersion)
    {
        case FileFormatVersion::A:
            expectedByteLength = 40u; break;
        case FileFormatVersion::B:
        case FileFormatVersion::C:
            expectedByteLength = 48u; break;
        default:
            throw MissingFileFormatCheck(__func__, __LINE__, mFileFormatVersion); break;
    }

    if(byteLength != expectedByteLength)
    {
        throw FileFormatChanged("Arc");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << arc << std::endl;

    return arc;
}


SymbolPinScalar Parser::readSymbolPinScalar()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolPinScalar symbolPinScalar;

    symbolPinScalar.name = readStringBothTerm();

    symbolPinScalar.startX = mDs.readInt32();
    symbolPinScalar.startY = mDs.readInt32();
    symbolPinScalar.hotptX = mDs.readInt32();
    symbolPinScalar.hotptY = mDs.readInt32();

    symbolPinScalar.pinShape = ToPinShape(mDs.readUint16());

    std::clog << print_unknown_data(2, std::string(__func__) + " - 0") << std::endl;

    symbolPinScalar.portType = ToPortType(mDs.readUint32());

    std::clog << print_unknown_data(6, std::string(__func__) + " - 1") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolPinScalar << std::endl;

    return symbolPinScalar;
}


SymbolPinBus Parser::readSymbolPinBus()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    SymbolPinBus symbolPinBus;

    symbolPinBus.name = readStringBothTerm();

    symbolPinBus.startX = mDs.readInt32();
    symbolPinBus.startY = mDs.readInt32();
    symbolPinBus.hotptX = mDs.readInt32();
    symbolPinBus.hotptY = mDs.readInt32();

    symbolPinBus.pinShape = ToPinShape(mDs.readUint16());

    std::clog << print_unknown_data(2, std::string(__func__) + " - 0") << std::endl;

    symbolPinBus.portType = ToPortType(mDs.readUint32());

    std::clog << print_unknown_data(6, std::string(__func__) + " - 1") << std::endl;

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
    std::cout << print_unknown_data(2, std::string(__func__) + " - 0") << std::endl;

    assume_data({0x00}, std::string(__func__) + " - 1");

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolDisplayProp << std::endl;

    return symbolDisplayProp;
}


// @todo implement return type and return it
void Parser::readERCSymbol()
{
    std::string name = readStringBothTerm();

    // @todo Probably 'sourceLibName' which is a string but I'm not sure. Could also be the
    //       last part of the next unknown block
    std::cout << print_unknown_data(3, std::string(__func__) + " - 0") << std::endl;

    std::cout << print_unknown_data(4, std::string(__func__) + " - 1") << std::endl;

    uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        GeometryStructure geometryStructure = ToGeometryStructure(mDs.readUint16());

        readGeometryStructure(geometryStructure, nullptr); // @todo push structure
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
    std::cout << print_unknown_data(4, std::string(__func__) + " - 0") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << symbolBBox << std::endl;

    return symbolBBox;
}


// @todo Probably specifies the 'Package Properties'
T0x1f Parser::readT0x1f()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    T0x1f t0x1f;

    t0x1f.name = readStringBothTerm();

    std::string unknownStr0 = readStringBothTerm(); // @todo figure out
    std::cout << "readT0x1f unknownStr0 = " << unknownStr0 << std::endl;

    t0x1f.refDes = readStringBothTerm();

    std::string unknownStr1 = readStringBothTerm(); // @todo figure out
    std::cout << "readT0x1f unknownStr1 = " << unknownStr1 << std::endl;

    t0x1f.pcbFootprint = readStringBothTerm();

    // Maybe the last two bytes specify the amount of units the symbols has?
    // Also called "Section Count"
    std::cout << print_unknown_data(2, std::string(__func__) + " - 0 - Prob. Unit Count") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << t0x1f << std::endl;

    return t0x1f;
}


// @todo create/update Kaitai file
GeneralProperties Parser::readGeneralProperties()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    GeneralProperties generalProperties;

    // @todo move to kaitai file
    // doc: |
    //   Implementation path of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation Path'
    generalProperties.implementationPath = readStringBothTerm();

    // @todo move to kaitai file
    // doc: |
    //   Implementation of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation'
    generalProperties.implementation = readStringBothTerm();

    // @todo move to kaitai file
    // doc: |
    //   Reference descriptor for the symbol. E.g. 'R' for resistor.
    //   See OrCAD: 'Package Properties' -> 'Part Reference Prefix'
    generalProperties.refDes = readStringBothTerm();

    // @todo move to kaitai file
    // doc: |
    //   Value of the symbol. E.g. '10k' for a resistor.
    //   See OrCAD: 'Part Properties' -> 'Value'
    generalProperties.partValue = readStringBothTerm();

    const uint8_t properties = mDs.readUint8();

    // Expect that upper bits are unused => 00xx xxxxb
    if((properties & 0xc0) != 0x00)
    {
        throw std::runtime_error("Expected 00xx xxxxb but got 0x" + ToHex(properties & 0xc0, 2));
    }

    const uint8_t pinProperties      =  properties       & 0x07; // Get bits 2 down to 0
    const uint8_t implementationType = (properties >> 3) & 0x07; // Get bits 5 down to 3

    generalProperties.pinNameVisible   =  static_cast<bool>((pinProperties & 0x01) >> 0); // Bit 0
    generalProperties.pinNameRotate    =  static_cast<bool>((pinProperties & 0x02) >> 1); // Bit 1
    generalProperties.pinNumberVisible = !static_cast<bool>((pinProperties & 0x04) >> 2); // Bit 2 - Note that this bit is inverted

    generalProperties.implementationType = ToImplementationType(implementationType);

    std::cout << print_unknown_data(1, std::string(__func__) + " - 0") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << generalProperties << std::endl;

    return generalProperties;
}


Properties Parser::readProperties()
{
    // @todo this structure contains somehow .Normal and .Convert variants

    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Properties properties;

    properties.ref = readStringBothTerm();

    assume_data({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably string

    // @todo use enum for the view (normal/convert)
    const uint16_t viewNumber = mDs.readUint16(); // @todo I assume that this is the amount of views
                                               // the symbol has. Typically 1 (.Normal) or maybe
                                               // 2 with (.Normal and .Convert)
                                               // Add to properties

    switch(viewNumber)
    {
        case 1: // ".Normal"
            // Nothing to do
            break;

        case 2: // ".Convert"
            // @todo how to handle optional attributes in my structures?
            properties.convertName = readStringBothTerm(); // @todo include into Kaitai file
            break;

        default:
            throw std::runtime_error("viewNumber is " + std::to_string(viewNumber) +
                " but it was expected that this can only take the value 1 or 2!");
            break;
    }

    properties.name = readStringBothTerm();

    // This really looks like an TypePrefix! Maybe this property can be split up?
    std::cout << print_unknown_data(29, std::string(__func__) + " - 1") << std::endl;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << properties << std::endl;

    return properties;
}


Properties2 Parser::readProperties2()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    Properties2 properties2;

    properties2.name = readStringBothTerm();

    assume_data({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably string

    properties2.refDes = readStringBothTerm();

    assume_data({0x00, 0x00, 0x00}, std::string(__func__) + " - 1"); // Unknown but probably string

    properties2.footprint = readStringBothTerm();

    properties2.sectionCount = mDs.readUint16(); // @todo has this something to do with units? Or was this just bad naming from myself?

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << properties2 << std::endl;

    return properties2;
}


std::vector<Type> Parser::parseTypes()
{
    std::vector<Type> types;

    // File can be completely empty (size of 0 byte)
    while(!mDs.isEoF())
    {
        Type type;

        type.name = readStringBothTerm();
        type.componentType = ToComponentType(mDs.readUint16());

        types.push_back(type);
    }

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

    GeometrySpecification geometrySpecification;

    geometrySpecification.name = readStringBothTerm(); // @todo add to struct and Kaitai file

    assume_data({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably a string
    assume_data({0x30}, std::string(__func__) + " - 1");
    assume_data({0x00, 0x00, 0x00}, std::string(__func__) + " - 2"); // Unknown but probably a string

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

        GeometryStructure geoStruct = ToGeometryStructure(mDs.readUint16());
        readGeometryStructure(geoStruct, &geometrySpecification);

        // uint16_t foo = mDs.readUint8();
        // foo = (foo << 8) | foo;
        // geoStruct = ToGeometryStructure(foo);

        // std::cout << print_unknown_data(40, std::string(__func__) + " - 1") << std::endl;
        // readPreamble();

        if(mFileFormatVersion == FileFormatVersion::A)
        {
            std::cout << print_unknown_data(8, std::string(__func__) + " - 3.5") << std::endl;
        }
    }

    if(geometryCount == 0u)
    {
        // throw std::runtime_error("CatchMeIfYouCan");
        // std::cout << print_unknown_data(6, std::string(__func__) + " - 4") << std::endl;
    }

    // if(geometryCount == 0u)
    // {
    //     std::cout << print_unknown_data(10, std::string(__func__) + " - 3") << std::endl;

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

    //         std::cout << print_unknown_data(24, "foo") << std::endl;

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
    //         // readGeometryStructure(geoStruct, &geometrySpecification);
    //     }
    // }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << geometrySpecification << std::endl;

    return geometrySpecification;
}