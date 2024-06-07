#ifndef CONTAINERCONTEXT_HPP
#define CONTAINERCONTEXT_HPP


#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

// #include "Database.hpp"
#include "General.hpp"


class Container;
class Database;


namespace fs = std::filesystem;

class CfbfStreamLocation
{
public:
    CfbfStreamLocation(
        const fs::path& aStreamPath, const fs::path& aExtractedCfbfPath)
    {
        std::vector<std::string> streamLocation{};

        // The layout in the filesystem is equal
        // to the one inside the CFBF container
        fs::path relPathStreamLoc = fs::relative(aStreamPath, aExtractedCfbfPath);

        for(const auto& pathPart : relPathStreamLoc)
        {
            streamLocation.push_back(pathPart);
        }

        if(!streamLocation.empty())
        {
            // Remove file extension (`.bin`)
            streamLocation.back() = fs::path{streamLocation.back()}.replace_extension();
        }

        mStreamLocation = std::move(streamLocation);
    }

    fs::path get_relative_fs_path() const
    {
        const auto vec = get_vector();

        fs::path fsPath{};

        for(const auto& part : vec)
        {
            fsPath = fsPath / part;
        }

        return fsPath;
    }

    const std::vector<std::string>& get_vector() const
    {
        return mStreamLocation;
    }

    /**
     * Check if the CfbfStreamLocation matches the given pattern. Note
     * that the vector size needs to be equal and all given string parts
     * need to match. As a wildcard part, you can use std::nullopt
    */
    bool matches_pattern(const std::vector<std::optional<std::string>>& aPattern)
    {
        if(get_vector().size() != aPattern.size())
        {
            return false;
        }

        // Both vectors have the same length
        for(std::size_t i{0U}; i < get_vector().size(); ++i)
        {
            // Check for wildcard and skip if found
            if(!aPattern.at(i).has_value())
            {
                continue;
            }

            // Check that the location part is equal
            if(get_vector().at(i) != aPattern.at(i).value())
            {
                return false;
            }
        }

        return true;
    }

private:

    std::vector<std::string> mStreamLocation{};
};


[[maybe_unused]]
static std::string to_string(const CfbfStreamLocation& aLocation)
{
    std::string locStr{};
    for(const auto& locPart : aLocation.get_vector())
    {
        locStr += "/" + locPart;
    }

    return locStr;
}

[[maybe_unused]]
static bool operator==(const CfbfStreamLocation& aLhs, const std::vector<std::string>& aRhs)
{
    return aLhs.get_vector() == aRhs;
}

[[maybe_unused]]
static bool operator==(const std::vector<std::string>& aLhs, const CfbfStreamLocation& aRhs)
{
    return aRhs == aLhs;
}

[[maybe_unused]]
static bool operator==(const CfbfStreamLocation& aLhs, const CfbfStreamLocation& aRhs)
{
    return aLhs.get_vector() == aRhs.get_vector();
}



struct ParserConfig
{
    std::size_t mThreadCount{std::thread::hardware_concurrency()}; //!< Number of threads used for parsing streams in parallel

    bool mSkipUnknownPrim{true}; //!< Unknown primitives should be skipped during parsing
    bool mSkipInvalidPrim{true}; //!< Invalid primitives should be skipped during parsing

    bool mSkipUnknownStruct{true}; //!< Unknown structures should be skipped during parsing
    bool mSkipInvalidStruct{true}; //!< Invalid structures should be skipped during parsing

    bool mKeepTmpFiles{true}; //!< Do not delete temporary files after parser completed
};


[[maybe_unused]]
static std::string to_string(const ParserConfig& aCfg)
{
    std::string str;
    str += fmt::format("mSkipUnknownPrim   = {}\n", aCfg.mSkipUnknownPrim);
    str += fmt::format("mSkipInvalidPrim   = {}\n", aCfg.mSkipInvalidPrim);
    str += fmt::format("mSkipUnknownStruct = {}\n", aCfg.mSkipUnknownStruct);
    str += fmt::format("mSkipInvalidStruct = {}\n", aCfg.mSkipInvalidStruct);
    str += fmt::format("mKeepTmpFiles      = {}\n", aCfg.mKeepTmpFiles);

    return str;
}


class ContainerContext
{
public:

    ContainerContext(const fs::path& aInputCfbfFile,
        const fs::path& aExtractedCfbfPath, ParserConfig aCfg, Container& aContainer, Database& aDb) : mDb{aDb}, mContainer{aContainer},
            mLogger{"tmp"}
    {
        mInputCfbfFile = aInputCfbfFile;
        mExtractedCfbfPath = aExtractedCfbfPath;
        mCfg = aCfg;
        mFileFormatVersion = FileFormatVersion::C;
        mFileType = FileType::Library;
        mLogLevel = spdlog::level::trace;

        const fs::path logPath = mExtractedCfbfPath / "logs" / "OpenOrCadParser.log";
        configureLogger(logPath);
    }

    ContainerContext(const ContainerContext& aCtx) : mDb{aCtx.mDb}, mContainer{aCtx.mContainer}, mLogger{"tmp"}
    {
        mInputCfbfFile = aCtx.mInputCfbfFile;
        mExtractedCfbfPath = aCtx.mExtractedCfbfPath;
        mCfg = aCtx.mCfg;
        mFileFormatVersion = aCtx.mFileFormatVersion;
        mFileType = aCtx.mFileType;
        mLogLevel = aCtx.mLogLevel;
    }

    void configureLogger(const fs::path& aLogPath)
    {
        if(aLogPath.has_parent_path())
        {
            fs::create_directories(aLogPath.parent_path());
        }

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(aLogPath);
        mLogger = spdlog::logger{"file logger", {file_sink}};
        mLogger.set_pattern("[%^%l%$] %v");
        mLogger.set_level(mLogLevel);

        mLogger.info("Created log file at {}", aLogPath.string());
        spdlog::info("Created log file at {}", aLogPath.string());
    }

    Database& mDb;
    Container& mContainer;

    fs::path mInputCfbfFile; //!< Input CFBF container
    fs::path mExtractedCfbfPath;

    ParserConfig mCfg;

    FileFormatVersion mFileFormatVersion;
    FileType mFileType;

    spdlog::level::level_enum mLogLevel;
    spdlog::logger mLogger;
};


#endif // CONTAINERCONTEXT_HPP