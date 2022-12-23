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
#include "Enums/HatchStyle.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/PortType.hpp"
#include "Enums/Primitive.hpp"
#include "Enums/Rotation.hpp"
#include "Enums/Structure.hpp"
#include "Exception.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "PinShape.hpp"
#include "Primitives/Point.hpp"
#include "Primitives/PrimArc.hpp"
#include "Primitives/PrimEllipse.hpp"
#include "Primitives/PrimLine.hpp"
#include "Primitives/PrimPolygon.hpp"
#include "Primitives/PrimRect.hpp"
#include "Streams/StreamAdminData.hpp"
#include "Streams/StreamDsnStream.hpp"
#include "Streams/StreamHSObjects.hpp"
#include "Streams/StreamLibrary.hpp"
#include "Streams/StreamNetBundleMapData.hpp"
#include "Streams/StreamType.hpp"
#include "Structures/StructAlias.hpp"
#include "Structures/StructGraphicBoxInst.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"
#include "Structures/StructPartInst.hpp"
#include "Structures/StructProperties.hpp"
#include "Structures/StructSthInPages0.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPinBus.hpp"
#include "Structures/StructSymbolPinScalar.hpp"
#include "Structures/StructT0x1f.hpp"
#include "Structures/StructT0x10.hpp"
#include "Structures/TrailingProperties.hpp"


namespace fs = std::filesystem;


Parser::Parser(const fs::path& aFile, FileFormatVersion aFileFormatVersion) :
    mFileFormatVersion{aFileFormatVersion}, mFileCtr{0U}, mFileErrCtr{0U}, mImgCtr{0U}
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
        case Structure::Primitives:        required = false; break;
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
        mLibrary.cellsDir        = parseFile<StreamDirectoryStruct>(pathCellsDir, [this](){ return readStreamCellsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathCellsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathExportBlocksDir))
    {
        mLibrary.exportBlocksDir = parseFile<StreamDirectoryStruct>(pathExportBlocksDir, [this](){ return readStreamExportBlocksDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathExportBlocksDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathGraphicsDir))
    {
        mLibrary.graphicsDir     = parseFile<StreamDirectoryStruct>(pathGraphicsDir, [this](){ return readStreamGraphicsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathGraphicsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathPackagesDir))
    {
        mLibrary.packagesDir     = parseFile<StreamDirectoryStruct>(pathPackagesDir, [this](){ return readStreamPackagesDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathPackagesDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathPartsDir))
    {
        mLibrary.partsDir        = parseFile<StreamDirectoryStruct>(pathPartsDir, [this](){ return readStreamPartsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathPartsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsDir))
    {
        mLibrary.symbolsDir      = parseFile<StreamDirectoryStruct>(pathSymbolsDir, [this](){ return readStreamSymbolsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathSymbolsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathViewsDir))
    {
        mLibrary.viewsDir        = parseFile<StreamDirectoryStruct>(pathViewsDir, [this](){ return readStreamViewsDirectory(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathViewsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathAdminData))
    {
        mLibrary.adminData = parseFile<StreamAdminData>(pathAdminData, [this](){ return readStreamAdminData(); });
    }

    if(fs::exists(pathHSObjects))
    {
        mLibrary.hsObjects = parseFile<StreamHSObjects>(pathHSObjects, [this](){ return readStreamHSObjects(); });
    }

    if(fs::exists(pathNetBundleMapData))
    {
        mLibrary.netBundleMapData = parseFile<StreamNetBundleMapData>(pathNetBundleMapData, [this](){ return readStreamNetBundleMapData(); });
    }

    if(fs::exists(pathGraphicsTypes))
    {
        mLibrary.graphicsTypes = parseFile<std::vector<Type>>(pathGraphicsTypes, [this](){ return readStreamType(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathGraphicsTypes.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsTypes))
    {
        mLibrary.symbolsTypes = parseFile<std::vector<Type>>(pathSymbolsTypes, [this](){ return readStreamType(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathSymbolsTypes.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathLibrary))
    {
        mLibrary.library = parseFile<StreamLibrary>(pathLibrary, [this](){ return readStreamLibrary(); });
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathLibrary.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathDsnStream))
    {
        mLibrary.dsnStream = parseFile<StreamDsnStream>(pathDsnStream, [this](){ return readStreamDsnStream(); });
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsERC))
    {
        // @todo write results into mLibrary
        parseFile<bool>(pathSymbolsERC, [this](){ return readStreamERC(); });
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

            mLibrary.packages.push_back(parseFile<StreamPackage>(pathPackage, [this](){ return readStreamPackage(); }));

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

            mLibrary.symbols.push_back(parseFile<StreamSymbol>(pathSymbol, [this](){ return readStreamSymbol(); }));

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
            parseFile<bool>(schematic, [this](){ return readStreamSchematic(); });
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
            parseFile<bool>(hierarchy, [this](){ return readStreamHierarchy(); });
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
                parseFile<bool>(page, [this](){ return readStreamPage(); });
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
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Input Container: {}", mInputFile.string()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Current File:    {}", mCurrOpenFile.string()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), mDs.getCurrentOffsetStrMsg()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "\nError Message: {}\n\n", e.what()));
    }
    catch(...)
    {
        spdlog::error(fmt::format(fg(fmt::color::crimson), "--------ERROR REPORT--------"));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Unknown exception caught!\n"));
    }
}


void Parser::discard_until_preamble()
{
    const int patternSize = 4;
    std::array<uint8_t, patternSize> buffer = {0};

    const size_t startOffset = mDs.getCurrentOffset();

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

        if(mDs.isEoF())
        {
            const std::string msg = fmt::format("{}: Unexpectedly reached end-of-file!", __func__);

            spdlog::error(msg);
            throw std::runtime_error(msg);
        }
    }

    // Put back the preamble such that it can be parsed in the next step
    for(size_t i = 0u; i < preamble.size(); ++i)
    {
        mDs.putback(preamble[preamble.size() - 1 - i]);
    }

    const size_t endOffset = mDs.getCurrentOffset();

    spdlog::critical("{}: Discarded {} Byte until next preamble", __func__, endOffset - startOffset);
}

Structure Parser::auto_read_prefixes()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const size_t startOffset = mDs.getCurrentOffset();

    const auto logLevel = spdlog::get_level();
    spdlog::set_level(spdlog::level::off);

    bool failed = true;
    size_t prefixCtr = 0U;

    // Count from back to make it easier catching long preambles
    // that could succeed by mistake if the the algorithm detects
    // no errors in a shorter version.
    const size_t maxPrefixes = 10U;
    for(prefixCtr = maxPrefixes; prefixCtr >= 1U; --prefixCtr)
    {
        // Reading the prefixes might read beyond EoF in some cases,
        // because its just a prediction, we do not care. Therefore
        // reset the EoF flag.
        mDs.clear();

        failed = false;

        try
        {
            read_prefixes(prefixCtr, true);
        }
        catch(const std::exception& e)
        {
            failed = true;
        }

        mDs.setCurrentOffset(startOffset);

        // Reading the prefixes might read beyond EoF in some cases,
        // because its just a prediction, we do not care. Therefore
        // reset the EoF flag.
        mDs.clear();

        if(!failed)
        {
            break;
        }
    }

    // Reading the prefixes might read beyond EoF in some cases,
    // because its just a prediction, we do not care. Therefore
    // reset the EoF flag.
    mDs.clear();

    spdlog::set_level(logLevel);

    if(failed)
    {
        const std::string msg = fmt::format("{}: Could not find valid number of prefixes! (maximum is set to {} but could be higher)",
            __func__, maxPrefixes);

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    spdlog::info("{}: Found {} prefixes\n", __func__, prefixCtr);
    Structure structure = read_prefixes(prefixCtr);

    mDs.sanitizeNoEoF();

    return structure;
}


// Read number of prefixes, where the last one is a short prefix
Structure Parser::read_prefixes(size_t aNumber, bool aPrediction)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    if(aNumber == 0U)
    {
        throw std::invalid_argument(fmt::format("aNumber = {} but must be > 0!", aNumber));
    }

    Structure firstStruct;

    std::vector<std::pair<size_t, size_t>> offsets; //!< First = Current offset absolute; Second = Offset relative to structure

    for(size_t i = 0U; i < aNumber; ++i)
    {
        Structure currStruct;

        const size_t preambleOffset = mDs.getCurrentOffset();

        if(i == aNumber - 1)
        {
            const auto currPrefix = read_single_prefix_short();
            currStruct = currPrefix.first;
        }
        else
        {
            const auto currPrefix = read_single_prefix();
            currStruct = currPrefix.first;

            offsets.push_back(std::pair<size_t, size_t>{preambleOffset, static_cast<size_t>(currPrefix.second)});
        }

        if(i == 0U)
        {
            firstStruct = currStruct;
        }

        if(currStruct != firstStruct)
        {
            const std::string msg = fmt::format("{}: {} != {}",
                __func__, to_string(currStruct), to_string(firstStruct));

            spdlog::error(msg);
            throw std::runtime_error(msg);
        }
    }

    if(!aPrediction)
    {
        if(offsets.size() >= 2U)
        {
            for(size_t i = 0U; i < offsets.size() - 1U; i += 2)
            {
                const std::pair<size_t, size_t> start_pair = offsets[i + 1U];
                const std::pair<size_t, size_t> stop_pair  = offsets[i];

                const FutureData futureData = FutureData{start_pair.first, start_pair.second, stop_pair.first, stop_pair.second};

                std::optional<FutureData> existing = mFutureDataLst.getByStartOffset(futureData.getStartOffset());

                if(existing.has_value())
                {
                    if(existing.value().getStopOffset() != futureData.getStopOffset())
                    {
                        const std::string msg = fmt::format("{}: Future data at 0x{:08x} is either {} or {} Byte long,"
                            " having both lengths does not make any sense.",
                            __func__, existing.value().getByteLen(), futureData.getByteLen());

                        spdlog::error(msg);
                        throw std::runtime_error(msg);
                    }
                    else
                    {
                        spdlog::debug("{}: Future data exists already in list", __func__);
                    }
                }

                mFutureDataLst.push_back(futureData);

                spdlog::debug("{}: Found future data: {}", __func__, (mFutureDataLst.end() - 1)->string());
            }
        }
        else if(offsets.size() == 1U)
        {
            spdlog::debug("{}: Found single structure beginning at 0x{:08x}", __func__, offsets[0].second);
        }
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return firstStruct;
}

std::pair<Structure, uint32_t> Parser::read_single_prefix()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const Structure typeId = ToStructure(mDs.readUint8());

    const uint32_t byteOffset = mDs.readUint32();

    spdlog::debug("{:>2} = {}: Offset = {}\n", static_cast<int>(typeId), to_string(typeId), byteOffset);

    mDs.printUnknownData(4, std::string(__func__) + " - 0");
    // mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, byteOffset};
}


std::pair<Structure, uint32_t> Parser::read_single_prefix_short()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const Structure typeId = ToStructure(mDs.readUint8());

    const int16_t size = mDs.readInt16();

    spdlog::debug("{:>2} = {}: Size = {}\n", static_cast<int>(typeId), to_string(typeId), size);

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
                        int64_t newIdx = static_cast<int64_t>(idx);
                        return newIdx >= 0 ? mLibrary.library.strLst.at(newIdx) : "";
                    };

                spdlog::debug("  {}: {} <- {}", i, getStr(nameValueMapping.at(i).first), getStr(nameValueMapping.at(i).second));
            }
            catch(const std::exception& e)
            {
                const std::string msg = fmt::format("{}: Tried to access strLst out of range!\n{}", __func__, e.what());
                // spdlog::error(msg);
                // throw std::out_of_range(msg);
            }
        }
    }
    else // size < 0
    {
        // @todo Why is -1 used? The value 0 would also suffice...
        // Until now I only saw it for PinIdxMapping, Properties and SymbolDisplayProp
        spdlog::warn("{}: What does {} mean?", to_string(typeId), size); // @todo Figure out
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, size};
}


uint32_t Parser::readPreamble(bool readOptionalLen)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    // Magic number specifying the beginning of a struct
    mDs.assumeData({0xff, 0xe4, 0x5c, 0x39}, std::string(__func__) + " - 0");

    const uint32_t optionalLen = readOptionalLen ? mDs.readUint32() : 0u;

    if(optionalLen != 0U)
    {
        spdlog::warn("{}: Detected optionalLen = {}", __func__, optionalLen);
    }

    mDs.printUnknownData(optionalLen, std::string(__func__) + " - 1 | Correlates to locks");

    if(optionalLen > 0u)
    {
        // @todo Looks like this correlates to setting a lock for an object.
        spdlog::debug("{}: Figure out when optionalLen is used! Currently it's 0x{:04x}", __func__, optionalLen);
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return optionalLen;
}


// Looks like some structures require a preceding preamble but not all.
// @todo Could be resolved by the trailing data structures defined in the prefix
uint32_t Parser::readConditionalPreamble(Structure structure, bool readOptionalLen)
{
    uint32_t optionalLen = 0u;

    if(requiresPreamble(structure))
    {
        optionalLen = readPreamble(readOptionalLen);
    }

    return optionalLen;
}


VariantPrimitive Parser::readPrimitive(Primitive aPrimitive)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    VariantPrimitive retPrim;

    switch(aPrimitive)
    {
        case Primitive::Rect:         retPrim = readPrimRect();         break;
        case Primitive::Line:         retPrim = readPrimLine();         break;
        case Primitive::Arc:          retPrim = readPrimArc();          break;
        case Primitive::Ellipse:      retPrim = readPrimEllipse();      break;
        case Primitive::Polygon:      retPrim = readPrimPolygon();      break;
        case Primitive::Polyline:     retPrim = readPrimPolyline();     break;
        case Primitive::CommentText:  retPrim = readPrimCommentText();  break;
        case Primitive::Bitmap:       retPrim = readPrimBitmap();       break;
        case Primitive::SymbolVector: retPrim = readPrimSymbolVector(); break;
        case Primitive::Bezier:       retPrim = readPrimBezier();       break;
        default:
            const std::string msg = fmt::format("{}: Primitive {} is not yet handled",
                __func__, to_string(aPrimitive));

            spdlog::error(msg);
            throw std::runtime_error(msg);
            break;
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return retPrim;
}


VariantStructure Parser::readStructure(Structure aStructure)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    VariantStructure retStruct;

    switch(aStructure)
    {
        case Structure::Alias:                  retStruct = readStructAlias();                  break;
        case Structure::ERCSymbol:              retStruct = readStructERCSymbol();              break;
        case Structure::GlobalSymbol:           retStruct = readStructGlobalSymbol();           break;
        case Structure::GraphicBoxInst:         retStruct = readStructGraphicBoxInst();         break;
        case Structure::GraphicCommentTextInst: retStruct = readStructGraphicCommentTextInst(); break;
        case Structure::OffPageSymbol:          retStruct = readStructOffPageSymbol();          break;
        case Structure::PartInst:               retStruct = readStructPartInst();               break;
        case Structure::PinIdxMapping:          retStruct = readStructPinIdxMapping();          break;
        case Structure::PinShapeSymbol:         retStruct = readStructPinShapeSymbol();         break;
        case Structure::PortSymbol:             retStruct = readStructHierarchicSymbol();       break;
        case Structure::Primitives:             retStruct = readStructPrimitives();             break;
        case Structure::Properties:             retStruct = readStructProperties();             break;
        case Structure::SthInPages0:            retStruct = readStructSthInPages0();            break;
        case Structure::SymbolDisplayProp:      retStruct = readStructSymbolDisplayProp();      break;
        case Structure::SymbolPinBus:           retStruct = readStructSymbolPinBus();           break;
        case Structure::SymbolPinScalar:        retStruct = readStructSymbolPinScalar();        break;
        case Structure::T0x1f:                  retStruct = readStructT0x1f();                  break;
        case Structure::T0x10:                  retStruct = readStructT0x10();                  break;
        case Structure::WireScalar:             retStruct = readStructWireScalar();             break;
        default:

            const std::optional<FutureData> futureData = getFutureData();

            const std::string msg = fmt::format("{}: Structure {} is not implemented!",
                __func__, to_string(aStructure));

            if(futureData.has_value())
            {
                spdlog::error(msg);
                mDs.printUnknownData(futureData.value().getByteLen(),
                    fmt::format("{}: {} is not implemented",__func__, to_string(aStructure)));
            }
            else
            {
                throw std::runtime_error(msg);
            }

            break;
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    return retStruct;
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

    mImgCtr = 0U;

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


std::optional<FutureData> Parser::getFutureData()
{
    const size_t startOffset = mDs.getCurrentOffset();

    const std::optional<FutureData> thisFuture = mFutureDataLst.getByStartOffset(startOffset);

    if(thisFuture.has_value())
    {
        spdlog::info("Found this structure in future data: 0x{:08x} -> 0x{:08x} ({} Byte)",
            thisFuture.value().getStartOffset(), thisFuture.value().getStopOffset(),
            thisFuture.value().getByteLen());
    }
    else
    {
        spdlog::warn("Did not find this structure in future data with startOffset 0x{:08x}",
            startOffset);

        spdlog::debug("Current FutureDataLst:");

        for(const auto& futureData : mFutureDataLst)
        {
            spdlog::debug(futureData.string());
        }
    }

    return thisFuture;
}


void Parser::sanitizeThisFutureSize(std::optional<FutureData> aThisFuture)
{
    const size_t stopOffset = mDs.getCurrentOffset();

    if(aThisFuture.has_value())
    {
        if(aThisFuture.value().getStopOffset() != stopOffset)
        {
            const std::string msg = fmt::format("{}: StopOffsets differ! 0x{:08x} (expected) vs. 0x{:08x} (actual)",
                __func__, aThisFuture.value().getStopOffset(), stopOffset);

            spdlog::error(msg);
            spdlog::critical("The structure may have changed due to version differences!");
            throw std::runtime_error(msg);
        }
    }
    else
    {
        spdlog::debug("{}: Could not verify structure size as future is not available.", __func__);
    }
}


std::optional<FutureData> Parser::checkTrailingFuture()
{
    const size_t stopOffset = mDs.getCurrentOffset();

    const std::optional<FutureData> nextFuture = mFutureDataLst.getByStartOffset(stopOffset);

    if(nextFuture.has_value())
    {
        spdlog::warn("Detected trailing future data at 0x{:08x}", nextFuture.value().getStartOffset());
    }

    return nextFuture;
}


void Parser::readOptionalTrailingFuture()
{
    std::optional<FutureData> future = checkTrailingFuture();

    if(future.has_value())
    {
        mDs.printUnknownData(future.value().getByteLen(), fmt::format("{}: Trailing Future Data", __func__));
    }

    sanitizeThisFutureSize(future);
}


Primitive Parser::readPrefixPrimitive()
{
    Primitive primitive1 = ToPrimitive(mDs.readUint8());
    Primitive primitive2 = ToPrimitive(mDs.readUint8());

    if(primitive1 != primitive2)
    {
        const std::string msg = fmt::format("{}: Primitives {} != {}",
            __func__, to_string(primitive1), to_string(primitive2));

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    return primitive1;
}