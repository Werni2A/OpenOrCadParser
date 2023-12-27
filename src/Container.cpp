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
#include <thread>
#include <vector>

#include <fmt/color.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "Container.hpp"
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
#include "PinShape.hpp"
#include "Stream.hpp"
#include "StreamFactory.hpp"


namespace fs = std::filesystem;


// @todo Add ContainerContext and ParserContext where ContainerContext is shared by all ParserContexts, maybe even derived from it?
Library* gLibrary = new Library{};


Container::Container(const fs::path& aCfbfContainer, ParserConfig aCfg) :
    mFileCtr{0U}, mFileErrCtr{0U}, mCtx{aCfbfContainer, "", aCfg, *this}, mCfg{aCfg}, mStreams{}
{
    mCtx.mFileType = getFileTypeByExtension(aCfbfContainer);

    // Extract to a unique folder in case two similar named files
    // are extracted at the same time. E.g. in parallel execution.
    std::random_device rnd;
    std::mt19937 gen(rnd());

    const std::string uuid = fmt::format("{:08x}{:08x}{:08x}{:08x}", gen(), gen(), gen(), gen());

    const fs::path extractTo = fs::temp_directory_path() / "OpenOrCadParser" / uuid;
    mCtx.mExtractedCfbfPath = extractContainer(aCfbfContainer, extractTo);

    spdlog::debug("Using parser configuration:");
    spdlog::debug(to_string(mCfg));
}


Container::~Container()
{
    if(!mCfg.mKeepTmpFiles)
    {
        // Remove temporary extracted files
        fs::remove_all(mCtx.mExtractedCfbfPath.parent_path());

        spdlog::debug("Deleted CFBF container at `{}`", mCtx.mExtractedCfbfPath.string());
    }
}


void Container::parseLibraryThread(std::vector<std::unique_ptr<Stream>*> aStreamList)
{
    for(auto& stream : aStreamList)
    {
        bool parsedSuccessfully = true;
        try
        {
            (*stream)->mCtx.mAttemptedParsing = true;
            (*stream)->openFile();
            (*stream)->read();
            (*stream)->closeFile();
        }
        catch(...)
        {
            parsedSuccessfully = false;
            exceptionHandling();
        }

        (*stream)->mCtx.mParsedSuccessfully = parsedSuccessfully;
    }
}


// Equally distribute elements into new lists of pointers to the original elements
template<typename ElementType>
std::vector<std::vector<ElementType*>> equallyDistributeElementsIntoLists(
    std::size_t aListCount, std::vector<ElementType>& aElements)
{
    std::vector<std::vector<ElementType*>> lists{};

    lists.resize(aListCount);

    std::size_t listIdx{0U};

    for(auto& element : aElements)
    {
        lists.at(listIdx).push_back(&element);
        listIdx = (listIdx + 1U) % aListCount;
    }

    return lists;
}

/**
 * @brief Parse the whole library.
 */
void Container::parseLibrary()
{
    spdlog::info("Using {} threads", mCtx.mCfg.mThreadCount);

    spdlog::info("Start parsing library located at {}", mCtx.mExtractedCfbfPath.string());

    // Parse all streams in the container i.e. files in the file system
    for(const auto& dir_entry : fs::recursive_directory_iterator(mCtx.mExtractedCfbfPath))
    {
        if(!dir_entry.is_regular_file())
        {
            continue;
        }

        const auto remainingFile = dir_entry.path();

        ++mFileCtr;

        auto stream = StreamFactory::build(mCtx, remainingFile);

        if(!stream)
        {
            continue;
        }

        mStreams.push_back(std::move(stream));
    }

    auto threadJobList = equallyDistributeElementsIntoLists<std::unique_ptr<Stream>>(mCtx.mCfg.mThreadCount, mStreams);

    for(std::size_t i{0U}; i < threadJobList.size(); ++i)
    {
        spdlog::info("Assigning thread {} with the following {}/{} jobs:",
            i, threadJobList.at(i).size(), mFileCtr);

        const auto& threadJobs = threadJobList.at(i);

        for(const auto& job : threadJobs)
        {
            spdlog::debug("    {}", (*job)->mCtx.mInputStream.string());
        }
    }

    std::vector<std::thread> threadList;

    for(auto& threadJobs : threadJobList)
    {
        if(threadJobs.size() > 0U)
        {
            threadList.push_back(std::thread{&Container::parseLibraryThread, this, threadJobs});
        }
    }

    for(auto& thread : threadList)
    {
        thread.join();
    }

    for(const auto& stream : mStreams)
    {
        if(!stream->mCtx.mParsedSuccessfully.value_or(false))
        {
            mFileErrCtr++;
        }
    }

    std::string errCtrStr = fmt::format("Errors in {}/{} files!", mFileErrCtr, mFileCtr);

    errCtrStr = fmt::format((mFileErrCtr == 0u) ? fg(fmt::color::green) : fg(fmt::color::crimson),
        errCtrStr);

    spdlog::info(errCtrStr);

    // spdlog::info(to_string(mLibrary));
}


void Container::exceptionHandling()
{
    try
    {
        throw;
    }
    catch(const std::exception& e)
    {
        spdlog::error(fmt::format(fg(fmt::color::crimson), "--------ERROR REPORT--------"));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Input Container: {}", mCtx.mInputCfbfFile.string()));
        // @todo
        // spdlog::error(fmt::format(fg(fmt::color::crimson), "Current File:    {}", mCtx.mInputStream.string()));
        // spdlog::error(fmt::format(fg(fmt::color::crimson), mCtx.mDs.getCurrentOffsetStrMsg()));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "\nError Message: {}\n\n", e.what()));
    }
    catch(...)
    {
        spdlog::error(fmt::format(fg(fmt::color::crimson), "--------ERROR REPORT--------"));
        spdlog::error(fmt::format(fg(fmt::color::crimson), "Unknown exception caught!\n"));
    }
}


fs::path Container::extractContainer(const fs::path& aFile, const fs::path& aOutDir) const
{
    ContainerExtractor extractor{aFile};

    // extractor.outputFileInfo();

    return extractor.extract(aOutDir);
}


fs::path Container::extractContainer(const fs::path& aOutDir) const
{
    return extractContainer(mCtx.mInputCfbfFile, aOutDir);
}


void Container::printContainerTree() const
{
    ContainerExtractor extractor{mCtx.mInputCfbfFile};
    extractor.printContainerTree();
}


FileType Container::getFileTypeByExtension(const fs::path& aFile) const
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