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
#include <random>
#include <vector>

#include <fmt/color.h>
#include <fmt/core.h>
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
#include "Files/AdminData.hpp"
#include "Files/DsnStream.hpp"
#include "Files/HSObjects.hpp"
#include "Files/NetBundleMapData.hpp"
#include "Files/SymbolsLibrary.hpp"
#include "Files/Type.hpp"
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
#include "Structures/T0x1f.hpp"


namespace fs = std::filesystem;


Parser::Parser(const fs::path& aFile, FileFormatVersion aFileFormatVersion) :
    mFileCtr{0u}, mFileErrCtr{0u}, mFileFormatVersion{aFileFormatVersion}
{
    mFileType      = getFileTypeByExtension(aFile);
    mInputFile     = aFile;
    mInputFileSize = fs::file_size(aFile);

    // Extract to a unique folder in case two similar named files
    // are extracted at the same time. E.g. in parallel execution.
    std::random_device rnd;
    std::mt19937 gen(rnd());

    const std::string uuid = fmt::format("{:08x}{:08x}{:08x}{:08x}", gen(), gen(), gen(), gen());

    const fs::path extractTo = fs::temp_directory_path() / "OpenOrCadParser" / uuid;
    mExtractedPath = extractContainer(aFile, extractTo);

    // All files in the container need to be parsed, therefore add
    // them to the remaining ones.
    for(const auto& dir_entry : fs::recursive_directory_iterator(mExtractedPath))
    {
        if(dir_entry.is_regular_file() && !dir_entry.is_symlink())
        {
            mRemainingFiles.push_back(dir_entry.path());
        }
    }
}


Parser::~Parser()
{
    // Remove temporary extracted files
    fs::remove_all(mExtractedPath.parent_path());

    if(!mRemainingFiles.empty())
    {
        std::string msg = "The following files have not been parsed:\n";

        for(size_t i = 0U; i < mRemainingFiles.size(); ++i)
        {
            msg += fmt::format("  {}\n", mRemainingFiles[i].string());
        }

        spdlog::warn(msg);
    }
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

    spdlog::info("Start parsing library {}", aLibPath);

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

    fs::path pathCIS              = pathLib / "CIS";

    fs::path pathCISSchematicStore = pathCIS / "CISSchematicStore";
    fs::path pathCISSchematicStoreStream = pathCISSchematicStore / "CISSchematicStream.bin";

    fs::path pathCISVariantStore  = pathCIS / "VariantStore";

    fs::path pathCISVariantStoreBOM = pathCISVariantStore / "BOM";
    fs::path pathCISVariantStoreBOMDataStream = pathCISVariantStoreBOM / "BOMDataStream.bin";

    fs::path pathCISVariantStoreGroups = pathCISVariantStore / "Groups";
    fs::path pathCISVariantStoreGroupsDataStream = pathCISVariantStoreGroups / "GroupsDataStream.bin";

    fs::path pathCISVariantStoreVariantNames = pathCISVariantStore / "VariantNames.bin";

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
            throw std::runtime_error(
                fmt::format("{}: Did not expect to find files here!", __func__));
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
        if(!std::filesystem::exists(pagesDir))
        {
            continue;
        }

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

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathCellsDir))
    {
        mLibrary.cellsDir        = parseFile<DirectoryStruct>(pathCellsDir, [this](){ return readCellsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathCellsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathExportBlocksDir))
    {
        mLibrary.exportBlocksDir = parseFile<DirectoryStruct>(pathExportBlocksDir, [this](){ return readExportBlocksDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathExportBlocksDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathGraphicsDir))
    {
        mLibrary.graphicsDir     = parseFile<DirectoryStruct>(pathGraphicsDir, [this](){ return readGraphicsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathGraphicsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathPackagesDir))
    {
        mLibrary.packagesDir     = parseFile<DirectoryStruct>(pathPackagesDir, [this](){ return readPackagesDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathPackagesDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathPartsDir))
    {
        mLibrary.partsDir        = parseFile<DirectoryStruct>(pathPartsDir, [this](){ return readPartsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathPartsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsDir))
    {
        mLibrary.symbolsDir      = parseFile<DirectoryStruct>(pathSymbolsDir, [this](){ return readSymbolsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathSymbolsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathViewsDir))
    {
        mLibrary.viewsDir        = parseFile<DirectoryStruct>(pathViewsDir, [this](){ return readViewsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathViewsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathAdminData))
    {
        mLibrary.adminData = parseFile<AdminData>(pathAdminData, [this](){ return readAdminData(); });
    }

    if(fs::exists(pathHSObjects))
    {
        mLibrary.hsObjects = parseFile<HSObjects>(pathHSObjects, [this](){ return readHSObjects(); });
    }

    if(fs::exists(pathNetBundleMapData))
    {
        mLibrary.netBundleMapData = parseFile<NetBundleMapData>(pathNetBundleMapData, [this](){ return readNetBundleMapData(); });
    }

    if(fs::exists(pathGraphicsTypes))
    {
        mLibrary.graphicsTypes = parseFile<std::vector<Type>>(pathGraphicsTypes, [this](){ return readType(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathGraphicsTypes.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsTypes))
    {
        mLibrary.symbolsTypes = parseFile<std::vector<Type>>(pathSymbolsTypes, [this](){ return readType(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathSymbolsTypes.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathLibrary))
    {
        mLibrary.symbolsLibrary = parseFile<SymbolsLibrary>(pathLibrary, [this](){ return readSymbolsLibrary(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathLibrary.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathDsnStream))
    {
        mLibrary.dsnStream = parseFile<DsnStream>(pathDsnStream, [this](){ return readDsnStream(); });
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsERC))
    {
        // @todo write results into mLibrary
        parseFile<bool>(pathSymbolsERC, [this](){ return parseSymbolsERC(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathSymbolsERC.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathPackages))
    {
        for(const auto& file : fs::directory_iterator(pathPackages))
        {
            const fs::path& pathPackage = file.path();

            mLibrary.packages.push_back(parseFile<Package>(pathPackage, [this](){ return readPackage(); }));

            spdlog::info("----------------------------------------------------------------------------------\n");
        }
    }
    else
    {
        spdlog::debug("Directory does not exist: {}", pathPackages.string());
    }

    if(fs::exists(pathSymbols))
    {
        for(const auto& file : fs::directory_iterator(pathSymbols))
        {
            const fs::path& pathSymbol = file.path();

            // Skip the 'ERC' and '$Types$' stream as they are additional
            // information but no symbols.
            if(pathSymbol.filename() == "ERC.bin"     ||
               pathSymbol.filename() == "$Types$.bin")
            {
                continue;
            }

            mLibrary.symbols.push_back(parseFile<Symbol>(pathSymbol, [this](){ return readSymbol(); }));

            spdlog::info("----------------------------------------------------------------------------------\n");
        }
    }
    else
    {
        spdlog::warn("Directory does not exist: {}", pathSymbols.string());
    }

    for(const auto& schematic : pathViewsSchematicsSchematic)
    {
        if(fs::exists(schematic))
        {
            // @todo write results into mLibrary
            parseFile<bool>(schematic, [this](){ return readSchematic(); });
        }
        else
        {
            spdlog::debug("File does not exist: {}", schematic.string());
        }

        spdlog::info("----------------------------------------------------------------------------------\n");
    }

    for(const auto& hierarchy : pathViewsSchematicsHierarchyHierarchy)
    {
        if(fs::exists(hierarchy))
        {
            // @todo write results into mLibrary
            parseFile<bool>(hierarchy, [this](){ return readHierarchy(); });
        }
        else
        {
            spdlog::debug("File does not exist: {}", hierarchy.string());
        }

        spdlog::info("----------------------------------------------------------------------------------\n");
    }

    for(const auto& pages : pathViewsSchematicsPagesPages)
    {
        for(const auto& page : pages)
        {
            if(fs::exists(page))
            {
                // @todo write results into mLibrary
                parseFile<bool>(page, [this](){ return readPage(); });
            }
            else
            {
                spdlog::warn("File does not exist: {}", page.string());
            }

            spdlog::info("\n----------------------------------------------------------------------------------\n");
        }
    }

    std::string errCtrStr = fmt::format("Errors in {}/{} files!", mFileErrCtr, mFileCtr);

    errCtrStr = fmt::format((mFileErrCtr == 0u) ? fg(fmt::color::green) : fg(fmt::color::crimson),
        errCtrStr);

    spdlog::info(errCtrStr);

    // spdlog::info(to_string(mLibrary));

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

        spdlog::error(fmt::format(fg(fmt::color::crimson), "--------ERROR REPORT--------"));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "File: {}", mCurrOpenFile.string()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), mDs.getCurrentOffsetStrMsg()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), ("\nError Message: {}\n\n", e.what())));
    }
    catch(...)
    {
        spdlog::error(fmt::format(fg(fmt::color::crimson), "--------ERROR REPORT--------"));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Unknown exception caught!\n"));
    }
}


// @todo return real data object
bool Parser::readPartInst()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    bool obj = false;

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}


// @todo return real data object
bool Parser::readT0x10()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    bool obj = false;

    mDs.printUnknownData(16, std::string(__func__) + " - 0");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return obj;
}


std::pair<Structure, std::any> Parser::parseStructure(Structure structure)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    spdlog::debug("Parsing ", to_string(structure));

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
            const std::string errorMsg = fmt::format("Structure with value 0x{:02x} is not implemented!",
                to_string(structure));
            throw std::invalid_argument(errorMsg);
            break;
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

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
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return typeId;
}


Structure Parser::read_type_prefix()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const Structure typeId = ToStructure(mDs.readUint8());

    // @todo In most cases this is the offset from the following typeIdRep to
    //       the beginning of the next read_type_prefix() where all the previous
    //       structures belong to this one.
    mByteOffset = mDs.readUint32();
    spdlog::debug("{} - 0 | mByteOffset = {}", __func__, mByteOffset);
    // mDs.printUnknownData(4, std::string(__func__) + " - 0");

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");

    const Structure typeIdRep = read_type_prefix_short();

    if(typeId != typeIdRep)
    {
        throw std::runtime_error("typeId (" + std::to_string(static_cast<size_t>(typeId))
                                    + ") should be equal to typeIdRep ("
                                    + std::to_string(static_cast<size_t>(typeIdRep)) + ")!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return typeId;
}


Structure Parser::read_type_prefix_short()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const Structure typeId = ToStructure(mDs.readUint8());

    // const size_t startOffset = mDs.getCurrentOffset();

    // @todo this is probably not a lenght but specifies some attribute e.g. locked/not-locked
    //       0x0b = not-locked
    //       0x1e = locked
    // @todo In DsnStream its definetly the byteLength, therefore implement some
    //       sanity check veryfing the size.
    const uint32_t byteLength = mDs.readUint32();
    if(byteLength != 0x0b && byteLength != 0x1e)
    {
        // throw std::runtime_error("Unexpected lock value 0x" + ToHex(byteLength, 2));
    }

    mDs.printUnknownData(4, std::string(__func__) + " - 0");
    // mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    const Structure typeIdRep = ToStructure(mDs.readUint8());

    const int16_t size = mDs.readInt16();
    spdlog::debug("{} - 1 | typeId = {}", __func__, to_string(typeId));
    spdlog::debug("{} - 2 | size   = {}", __func__, size);

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

                spdlog::debug("  {}: {} <- {}", i, getStr(nameValueMapping.at(i).first), getStr(nameValueMapping.at(i).second));
            }
            catch(const std::exception& e)
            {
                const std::string msg = fmt::format("Tried to access strLst out of range!\n{}", e.what());
                spdlog::error(msg);
                throw std::out_of_range(msg);
            }
        }
    }
    else // size < 0
    {
        // @todo Why is -1 used? The value 0 would also suffice...
        // Until now I only saw it for PinIdxMapping, Properties and SymbolDisplayProp
        spdlog::warn("{}: What does {} mean?", to_string(typeId), size); // @todo Figure out
    }

    // if(mDs.getCurrentOffset() != startOffset + byteLength)
    // {
    //     throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    // }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return typeId;
}


uint32_t Parser::readPreamble(bool readOptionalLen)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    // Magic number specifying the beginning of a struct
    mDs.assumeData({0xff, 0xe4, 0x5c, 0x39}, std::string(__func__) + " - 0");

    const uint32_t optionalLen = readOptionalLen ? mDs.readUint32() : 0u;

    mDs.printUnknownData(optionalLen, std::string(__func__) + " - 1 | Correlates to locks");

    if(optionalLen > 0u)
    {
        // @todo Looks like this correlates to setting a lock for an object.
        spdlog::debug("{}: Figure out when optionalLen is used! Currently it's 0x{:04x}", __func__, optionalLen);
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

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
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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
            throw std::runtime_error(fmt::format("GeometryStructure has not yet implemented value 0x{:04x}",
                to_string(geometryStructure)));
            break;
    }

    if(geometrySpecification != nullptr)
    {
        // @todo do not overwrite geometrySpecification but only push_back
        //       the entries from container
        *geometrySpecification = container;
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


// @todo Probably a wrapper for Inst (Instances)
void Parser::readSthInPages0()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


void Parser::readGraphicCommentTextInst()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    mDs.printUnknownData(34, std::string(__func__) + " - 0");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


void Parser::readWireScalar()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    uint32_t dbId = mDs.readUint32();

    spdlog::debug("dbId = {}", dbId);

    mDs.printUnknownData(4, std::string(__func__) + " - 0");

    Color wireColor = ToColor(mDs.readUint32());

    int32_t startX = mDs.readInt32();
    int32_t startY = mDs.readInt32();
    int32_t endX   = mDs.readInt32();
    int32_t endY   = mDs.readInt32();

    spdlog::debug("startX = {} | startY = {} | endX = {} | endY = {}", startX, startY, endX, endY);

    mDs.printUnknownData(1, std::string(__func__) + " - 1");

    spdlog::debug("mByteOffset = {}", mByteOffset);

    if(mByteOffset == 0x3d)
    {
        mDs.printUnknownData(2, std::string(__func__) + " - 2");
    }
    else if(mByteOffset > 0x3d)
    {
        const uint16_t len = mDs.readUint16();

        spdlog::debug("len = {}", len);

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

    spdlog::debug("wireLineWidth = {} | wireLineStyle = {}", to_string(wireLineWidth), to_string(wireLineStyle));

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


void Parser::readAlias()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    int32_t locX = mDs.readInt32();
    int32_t locY = mDs.readInt32();

    spdlog::debug("locX = {} | locY = {}", locX, locY);

    Color color = ToColor(mDs.readUint32());

    spdlog::debug("color = {}", to_string(color));

    Rotation rotation = ToRotation(mDs.readUint32()); // @todo Why is it 4 byte? Probably increase Rotation size

    spdlog::debug("rotation = {}", to_string(rotation));

    uint16_t textFontIdx = mDs.readUint16(); // @todo educated guess

    spdlog::debug("Alias fontIdx = {}", textFontIdx);

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    std::string name = mDs.readStringLenZeroTerm();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


// @todo is this a specialized instance for Rects or general for all types?
void Parser::readGraphicBoxInst()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
}


void Parser::readDevHelper()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    mDs.discardBytes(0x2a1);

    read_type_prefix();
    readPreamble();

    readGraphicBoxInst();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
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
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    SymbolVector obj;

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

    obj.locX = mDs.readInt16();
    obj.locY = mDs.readInt16();

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
    obj.name = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00, 0x02, 0x00}, std::string(__func__) + " - 2");
    // mDs.printUnknownData(12, std::string(__func__) + " - 2");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
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
    spdlog::info("Opening file: {}", aFile.string());

    auto it = std::find(mRemainingFiles.begin(), mRemainingFiles.end(), aFile);

    if(it == mRemainingFiles.end())
    {
        throw std::runtime_error("File should have been in mRemainingFiles!");
    }

    mRemainingFiles.erase(it);

    mDs = DataStream{aFile};
    if(!mDs)
    {
        throw std::runtime_error("Could not open file: " + aFile.string());
    }

    mCurrOpenFile     = aFile;
    mCurrOpenFileSize = fs::file_size(aFile);

    spdlog::info("File contains {} byte.", mCurrOpenFileSize);
}


void Parser::closeFile()
{
    spdlog::info("Closing file: {}", mCurrOpenFile.string());

    mDs.close();

    mCurrOpenFile.clear();
    mCurrOpenFileSize = 0u;
}


PinIdxMapping Parser::readPinIdxMapping()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    PinIdxMapping obj;

    obj.unitRef = mDs.readStringLenZeroTerm();
    obj.refDes  = mDs.readStringLenZeroTerm();

    const uint16_t pinCount = mDs.readUint16();

    // @todo Add to kaitai file i = 'Order' of pin
    // See OrCAD: 'Pin Properties' -> 'Order'
    for(size_t i = 0u; i < pinCount; ++i)
    {
        obj.pinMap.push_back(mDs.readStringLenZeroTerm());

        const uint8_t separator = mDs.readUint8();

        spdlog::debug("Sep = 0x{:02x}", separator);

        // @todo maybe this is not a separator but the additional property of the pin?
        // As soon as I add a property like NET_SHORT the separator changes from 0x7f to 0xaa
        // This is probably also affected by units and convert view.
        if(separator != 0x7f && separator != 0xaa && separator != 0xff)
        {
            throw std::runtime_error(fmt::format("Separator should be 0x{:02x}, 0x{:02x} or"
                " 0x{:02x} but got 0x{:02x}!", 0x7f, 0xaa, 0xff, separator));
        }
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


SymbolPinScalar Parser::readSymbolPinScalar()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(symbolPinScalar));

    return symbolPinScalar;
}


SymbolPinBus Parser::readSymbolPinBus()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(symbolPinBus));

    return symbolPinBus;
}


SymbolDisplayProp Parser::readSymbolDisplayProp()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    SymbolDisplayProp symbolDisplayProp;

    symbolDisplayProp.nameIdx = mDs.readUint32();

    // @todo move to left shift operator
    // @bug The required string is not this one but the value of the associated property!!!! This is just the name of the property!!
    spdlog::debug("strLst Item = {}", mLibrary.symbolsLibrary.strLst.at(symbolDisplayProp.nameIdx - 1));

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
    spdlog::debug("Unknown bits in bitmap: {}", (packedStruct >> 8u) & 0x3f); // Bit 13 downto  8
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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(symbolDisplayProp));

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
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    SymbolBBox obj;

    obj.x1 = mDs.readInt16();
    obj.y1 = mDs.readInt16();
    obj.x2 = mDs.readInt16();
    obj.y2 = mDs.readInt16();

    // @todo not sure weather this belongs to the structure or should be outside of it
    mDs.printUnknownData(4, fmt::format("{} - 0", __func__));

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


// @todo Probably specifies the 'Package Properties'
T0x1f Parser::readT0x1f()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    T0x1f obj;

    obj.name = mDs.readStringLenZeroTerm();

    std::string unknownStr0 = mDs.readStringLenZeroTerm(); // @todo figure out
    spdlog::debug("{} unknownStr0 = {}", __func__, unknownStr0);

    obj.refDes = mDs.readStringLenZeroTerm();

    std::string unknownStr1 = mDs.readStringLenZeroTerm(); // @todo figure out
    spdlog::debug("{} unknownStr1 = {}", __func__, unknownStr1);

    obj.pcbFootprint = mDs.readStringLenZeroTerm();

    // Maybe the last two bytes specify the amount of units the symbols has?
    // Also called "Section Count"
    mDs.printUnknownData(2, std::string(__func__) + " - 0 - Prob. Unit Count");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


// @todo create/update Kaitai file
GeneralProperties Parser::readGeneralProperties()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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
        throw std::runtime_error(fmt::format("Expected 00xx xxxxb but got 0x{:02x}",
            properties & 0xc0));
    }

    const uint8_t pinProperties      =  properties       & 0x07; // Get bits 2 down to 0
    const uint8_t implementationType = (properties >> 3) & 0x07; // Get bits 5 down to 3

    obj.pinNameVisible   =  static_cast<bool>((pinProperties & 0x01) >> 0); // Bit 0
    obj.pinNameRotate    =  static_cast<bool>((pinProperties & 0x02) >> 1); // Bit 1
    obj.pinNumberVisible = !static_cast<bool>((pinProperties & 0x04) >> 2); // Bit 2 - Note that this bit is inverted

    obj.implementationType = ToImplementationType(implementationType);

    mDs.printUnknownData(1, std::string(__func__) + " - 0");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


Properties Parser::readProperties()
{
    // @todo this structure contains somehow .Normal and .Convert variants

    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


Properties2 Parser::readProperties2()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    Properties2 obj;

    obj.name = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably string

    obj.refDes = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 1"); // Unknown but probably string

    obj.footprint = mDs.readStringLenZeroTerm();

    obj.sectionCount = mDs.readUint16(); // @todo has this something to do with units? Or was this just bad naming from myself?

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


GeometrySpecification Parser::readSymbolProperties()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));
    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    return parseGeometrySpecification();
}


GeometrySpecification Parser::parseGeometrySpecification()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    GeometrySpecification obj;

    obj.name = mDs.readStringLenZeroTerm(); // @todo add to struct and Kaitai file

    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 0"); // Unknown but probably a string
    mDs.assumeData({0x30}, std::string(__func__) + " - 1");
    mDs.assumeData({0x00, 0x00, 0x00}, std::string(__func__) + " - 2"); // Unknown but probably a string

    const uint16_t geometryCount = mDs.readUint16();
    spdlog::debug("geometryCount = {}", geometryCount);

    for(size_t i = 0u; i < geometryCount; ++i)
    {
        spdlog::debug("i of geometryCount = {}", i);

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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}