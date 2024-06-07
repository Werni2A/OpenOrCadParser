#include <algorithm>
#include <any>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <deque>
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
#include "PinShape.hpp"
#include "Stream.hpp"
#include "StreamFactory.hpp"


namespace fs = std::filesystem;


Container::Container(const fs::path& aCfbfContainer, ParserConfig aCfg) :
    mDb{}, mFileCtr{0U}, mFileErrCtr{0U}, mCtx{aCfbfContainer, "", aCfg, mDb}, mCfg{aCfg}
{
    mCtx.mFileType = getFileTypeByExtension(aCfbfContainer);

    // Extract to a unique folder in case two similar named files
    // are extracted at the same time. E.g. in parallel execution.
    std::random_device rnd;
    std::mt19937 gen(rnd());

    const std::string uuid = fmt::format("{:08x}{:08x}{:08x}{:08x}", gen(), gen(), gen(), gen());

    const fs::path extractTo = fs::temp_directory_path() / "OpenOrCadParser" / uuid;
    mCtx.mExtractedCfbfPath = extractContainer(aCfbfContainer, extractTo);

    // @todo This is a hack, since mExtractedCfbfPath is not available at construction of the context
    const fs::path logPath = extractTo / "logs" / "OpenOrCadParser.log";
    mCtx.configureLogger(logPath);

    mCtx.mLogger.debug("Using parser configuration:");
    mCtx.mLogger.debug(to_string(mCfg));
}


Container::~Container()
{
    if(!mCfg.mKeepTmpFiles)
    {
        // Remove temporary extracted files
        fs::remove_all(mCtx.mExtractedCfbfPath.parent_path());

        mCtx.mLogger.debug("Deleted CFBF container at `{}`", mCtx.mExtractedCfbfPath.string());
    }
}


void Container::parseDatabaseFileThread(std::deque<std::shared_ptr<Stream>> aStreamList)
{
    for(auto& stream : aStreamList)
    {
        bool parsedSuccessfully = true;
        try
        {
            stream->mCtx.mAttemptedParsing = true;
            stream->openFile();
            stream->read();
            stream->closeFile();
        }
        catch(...)
        {
            parsedSuccessfully = false;
            stream->exceptionHandling();
        }

        stream->mCtx.mParsedSuccessfully = parsedSuccessfully;
    }
}


// Equally distribute elements into new lists of pointers to the original elements
void distributeStreamsToThreadJobsForParsing(
    std::size_t aNumberParallelJobs,
    const std::vector<std::shared_ptr<Stream>>& aStreams, //!< Streams to distribute
    std::deque<std::shared_ptr<Stream>>& aSequentialJobList,
    std::vector<std::deque<std::shared_ptr<Stream>>>& aParallelJobLists)
{
    aSequentialJobList.clear();
    aParallelJobLists.clear();

    aParallelJobLists.resize(aNumberParallelJobs);

    std::size_t listIdx{0U};

    for(auto& stream : aStreams)
    {
        const auto streamPath = stream->mCtx.mCfbfStreamLocation.get_vector();
        // All streams located at the root-directory should be parsed
        // ahead of parallel execution
        if(streamPath.size() == 1U)
        {
            // Put in sequential list
            if(streamPath.back() == "Library")
            {
                // `Library` needs to be parsed first
                aSequentialJobList.push_front(stream);
            }
            else
            {
                aSequentialJobList.push_back(stream);
            }
        }
        else
        {
            // Put in parallel list
            aParallelJobLists.at(listIdx).push_back(stream);
            listIdx = (listIdx + 1U) % aNumberParallelJobs;
        }
    }
}


/**
 * @brief Parse the whole database file.
 */
void Container::parseDatabaseFile()
{
    mCtx.mLogger.info("Using {} threads", mCtx.mCfg.mThreadCount);

    mCtx.mLogger.info("Start parsing library located at {}", mCtx.mExtractedCfbfPath.string());

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

        mDb.mStreams.push_back(std::move(stream));
    }

    std::deque<std::shared_ptr<Stream>> sequentialJobList{};
    std::vector<std::deque<std::shared_ptr<Stream>>> parallelJobsLists{};

    distributeStreamsToThreadJobsForParsing(mCtx.mCfg.mThreadCount, mDb.mStreams, sequentialJobList, parallelJobsLists);

    // Print sequential job assignment
    {
        mCtx.mLogger.info("Assigning main thread with the following {}/{} jobs (sequential):",
            sequentialJobList.size(), mFileCtr);

        for(const auto& job : sequentialJobList)
        {
            mCtx.mLogger.debug("    {}", job->mCtx.mInputStream.string());
        }
    }

    // Print parallel job assignment
    for(std::size_t i{0U}; i < parallelJobsLists.size(); ++i)
    {
        mCtx.mLogger.info("Assigning thread {} with the following {}/{} jobs (parallel):",
            i, parallelJobsLists.at(i).size(), mFileCtr);

        const auto& parallelJobList = parallelJobsLists.at(i);

        for(const auto& job : parallelJobList)
        {
            mCtx.mLogger.debug("    {}", job->mCtx.mInputStream.string());
        }
    }

    // Run sequential jobs before parallel execution
    parseDatabaseFileThread(sequentialJobList);

    // Run parallel jobs
    {
        std::vector<std::thread> threadList;

        for(auto& threadJobs : parallelJobsLists)
        {
            if(threadJobs.size() > 0U)
            {
                threadList.push_back(std::thread{&Container::parseDatabaseFileThread, this, threadJobs});
            }
        }

        for(auto& thread : threadList)
        {
            thread.join();
        }
    }

    for(const auto& stream : mDb.mStreams)
    {
        if(!stream->mCtx.mParsedSuccessfully.value_or(false))
        {
            mFileErrCtr++;
        }
    }

    std::string errCtrStr = fmt::format("Errors in {}/{} files!", mFileErrCtr, mFileCtr);

    errCtrStr = fmt::format((mFileErrCtr == 0u) ? fg(fmt::color::green) : fg(fmt::color::crimson),
        errCtrStr);

    mCtx.mLogger.info(errCtrStr);

    // mCtx.mLogger.info(to_string(mLibrary));
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