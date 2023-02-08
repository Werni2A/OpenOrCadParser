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
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
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
#include "Library.hpp"
#include "Parser.hpp"
#include "PinShape.hpp"
#include "Streams/StreamAdminData.hpp"
#include "Streams/StreamCellsDirectory.hpp"
#include "Streams/StreamDsnStream.hpp"
#include "Streams/StreamERC.hpp"
#include "Streams/StreamExportBlocksDirectory.hpp"
#include "Streams/StreamGraphicsDirectory.hpp"
#include "Streams/StreamHierarchy.hpp"
#include "Streams/StreamHSObjects.hpp"
#include "Streams/StreamLibrary.hpp"
#include "Streams/StreamNetBundleMapData.hpp"
#include "Streams/StreamPackage.hpp"
#include "Streams/StreamPackagesDirectory.hpp"
#include "Streams/StreamPage.hpp"
#include "Streams/StreamPartsDirectory.hpp"
#include "Streams/StreamSchematic.hpp"
#include "Streams/StreamSymbol.hpp"
#include "Streams/StreamSymbolsDirectory.hpp"
#include "Streams/StreamType.hpp"
#include "Streams/StreamViewsDirectory.hpp"


namespace fs = std::filesystem;


Library* gLibrary = new Library{};
FileType gFileType = FileType::Library; // @todo move to parser context
FileFormatVersion gFileFormatVersion = FileFormatVersion::C; // @todo move to parser context


Parser::Parser(const fs::path& aFile) :
    mFileCtr{0U}, mFileErrCtr{0U}, mDs{""}, mCtx{mDs, "", "", ""}
{
    gFileType      = getFileTypeByExtension(aFile);
    mInputFile     = aFile;

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
    if(!mCtx.mKeepTmpFiles)
    {
        // Remove temporary extracted files
        fs::remove_all(mExtractedPath.parent_path());
    }

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

/**
 * @brief Parse the whole library.
 */
void Parser::parseLibrary()
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
            // We extract embedded files into the same directory but do
            // not want to parse them as stream. Therefore skip them.
            // @todo This is just a workaround; needs some refactoring
            if(page.path().extension() != ".bin")
            {
                continue;
            }

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
        gLibrary->cellsDir = parseFile<StreamCellsDirectory>(pathCellsDir, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathCellsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathExportBlocksDir))
    {
        gLibrary->exportBlocksDir = parseFile<StreamExportBlocksDirectory>(pathExportBlocksDir, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathExportBlocksDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathGraphicsDir))
    {
        gLibrary->graphicsDir = parseFile<StreamGraphicsDirectory>(pathGraphicsDir, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathGraphicsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathPackagesDir))
    {
        gLibrary->packagesDir = parseFile<StreamPackagesDirectory>(pathPackagesDir, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathPackagesDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathPartsDir))
    {
        gLibrary->partsDir = parseFile<StreamPartsDirectory>(pathPartsDir, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathPartsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsDir))
    {
        gLibrary->symbolsDir = parseFile<StreamSymbolsDirectory>(pathSymbolsDir, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathSymbolsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathViewsDir))
    {
        gLibrary->viewsDir = parseFile<StreamViewsDirectory>(pathViewsDir, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathViewsDir.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathAdminData))
    {
        gLibrary->adminData = parseFile<StreamAdminData>(pathAdminData, mCtx);
    }

    if(fs::exists(pathHSObjects))
    {
        gLibrary->hsObjects = parseFile<StreamHSObjects>(pathHSObjects, mCtx);
    }

    if(fs::exists(pathNetBundleMapData))
    {
        gLibrary->netBundleMapData = parseFile<StreamNetBundleMapData>(pathNetBundleMapData, mCtx);
    }

    if(fs::exists(pathGraphicsTypes))
    {
        gLibrary->graphicsTypes = parseFile<StreamType>(pathGraphicsTypes, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathGraphicsTypes.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsTypes))
    {
        gLibrary->symbolsTypes = parseFile<StreamType>(pathSymbolsTypes, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathSymbolsTypes.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathLibrary))
    {
        gLibrary->library = parseFile<StreamLibrary>(pathLibrary, mCtx);
    }
    else
    {
        spdlog::debug("File does not exist: {}", pathLibrary.string());
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathDsnStream))
    {
        gLibrary->dsnStream = parseFile<StreamDsnStream>(pathDsnStream, mCtx);
    }

    spdlog::info("----------------------------------------------------------------------------------\n");

    if(fs::exists(pathSymbolsERC))
    {
        // @todo write results into mLibrary
        parseFile<StreamERC>(pathSymbolsERC, mCtx);
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

            // We extract embedded files into the same directory but do
            // not want to parse them as stream. Therefore skip them.
            // @todo This is just a workaround; needs some refactoring
            if(pathPackage.extension() != ".bin")
            {
                continue;
            }

            gLibrary->packages.push_back(parseFile<StreamPackage>(pathPackage, mCtx));

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

            // We extract embedded files into the same directory but do
            // not want to parse them as stream. Therefore skip them.
            // @todo This is just a workaround; needs some refactoring
            if(pathSymbol.extension() != ".bin")
            {
                continue;
            }

            // Skip the 'ERC', 'ERC_PHYSICAL' and '$Types$' stream as they are additional
            // information but no symbols.
            if(pathSymbol.filename() == "ERC.bin"          ||
               pathSymbol.filename() == "ERC_PHYSICAL.bin" ||
               pathSymbol.filename() == "$Types$.bin")
            {
                continue;
            }

            gLibrary->symbols.push_back(parseFile<StreamSymbol>(pathSymbol, mCtx));

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
            parseFile<StreamSchematic>(schematic, mCtx);
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
            parseFile<StreamHierarchy>(hierarchy, mCtx);
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
                parseFile<StreamPage>(page, mCtx);
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


fs::path Parser::extractContainer(const fs::path& aFile, const fs::path& aOutDir) const
{
    ContainerExtractor extractor{aFile};

    // extractor.outputFileInfo();

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

    mCtx.mDs            = mDs;
    mCtx.mInputFile     = mInputFile;
    mCtx.mCurrOpenFile  = mCurrOpenFile;
    mCtx.mExtractedPath = mExtractedPath;

    spdlog::info("File contains {} byte.", fs::file_size(aFile));
}


void Parser::closeFile()
{
    spdlog::info("Closing file: {}", mCurrOpenFile.string());

    mDs.close();

    mCurrOpenFile.clear();
}