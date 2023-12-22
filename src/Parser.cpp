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

#include "CommonBase.hpp"
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
#include "StreamFactory.hpp"


namespace fs = std::filesystem;


// @todo Add ContainerContext and ParserContext where ContainerContext is shared by all ParserContexts, maybe even derived from it?
Library* gLibrary = new Library{};
FileType gFileType = FileType::Library; // @todo move to parser context
FileFormatVersion gFileFormatVersion = FileFormatVersion::C; // @todo move to parser context


Parser::Parser(const fs::path& aCfbfContainer, ParserConfig aCfg) :
    mFileCtr{0U}, mFileErrCtr{0U}, tmpCtx{"", "", "", aCfg}, mCtx{tmpCtx}, mCfg{aCfg}
{
    gFileType      = getFileTypeByExtension(aCfbfContainer);
    mInputCfbfFile = aCfbfContainer;

    // Extract to a unique folder in case two similar named files
    // are extracted at the same time. E.g. in parallel execution.
    std::random_device rnd;
    std::mt19937 gen(rnd());

    const std::string uuid = fmt::format("{:08x}{:08x}{:08x}{:08x}", gen(), gen(), gen(), gen());

    const fs::path extractTo = fs::temp_directory_path() / "OpenOrCadParser" / uuid;
    mExtractedCfbfPath = extractContainer(aCfbfContainer, extractTo);

    spdlog::debug("Using parser configuration:");
    spdlog::debug(to_string(mCfg));
}


Parser::~Parser()
{
    if(!mCfg.mKeepTmpFiles)
    {
        // Remove temporary extracted files
        fs::remove_all(mExtractedCfbfPath.parent_path());

        spdlog::debug("Deleted CFBF container at `{}`", mExtractedCfbfPath.string());
    }
}

/**
 * @brief Parse the whole library.
 */
void Parser::parseLibrary()
{
    spdlog::info("Start parsing library located at {}", mExtractedCfbfPath.string());

    // Parse all streams in the container i.e. files in the file system
    for(const auto& dir_entry : fs::recursive_directory_iterator(mExtractedCfbfPath))
    {
        if(!dir_entry.is_regular_file())
        {
            continue;
        }

        const auto remainingFile = dir_entry.path();

        ++mFileCtr;

        std::unique_ptr<CommonBase> stream;

        ParserContext ctx{mInputCfbfFile, remainingFile, mExtractedCfbfPath, mCfg};
        mCtx = ctx;

        const auto streamLoc = getCfbfStreamLocationFromFileSytemLayout(remainingFile, mExtractedCfbfPath);
        stream = StreamFactory::build(ctx, streamLoc);

        if(nullptr == stream)
        {
            continue;
        }

        try
        {
            openFile(remainingFile);
            stream->read();
            closeFile();
        }
        catch(...)
        {
            mCtx.get().mFinishedParsing = true;
            exceptionHandling();
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
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Input Container: {}", mCtx.get().mInputCfbfFile.string()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Current File:    {}", mCtx.get().mInputStream.string()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), mCtx.get().mDs.get().getCurrentOffsetStrMsg()));
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
    return extractContainer(mInputCfbfFile, aOutDir);
}


void Parser::printContainerTree() const
{
    ContainerExtractor extractor{mInputCfbfFile};
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


void Parser::openFile(const fs::path& aInputStream)
{
    spdlog::info("Opening file: {}", aInputStream.string());

    spdlog::info("File contains {} byte.", fs::file_size(aInputStream));
}


void Parser::closeFile()
{
    spdlog::info("Closing file: {}", mCtx.get().mInputStream.string());

    mCtx.get().mDs.get().close();

    mCtx.get().mFinishedParsing = true;
}