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


class Database;


namespace fs = std::filesystem;


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
        const fs::path& aExtractedCfbfPath, ParserConfig aCfg, Database& aDb) : mDb{aDb},
            mLogger{"tmp"}
    {
        mInputCfbfFile = aInputCfbfFile;
        mExtractedCfbfPath = aExtractedCfbfPath;
        mCfg = aCfg;
        mFileFormatVersion = FileFormatVersion::C;
        mLogLevel = spdlog::level::trace;

        const fs::path logPath = mExtractedCfbfPath / "logs" / "OpenOrCadParser.log";
        configureLogger(logPath);
    }

    ContainerContext(const ContainerContext& aCtx) : mDb{aCtx.mDb}, mLogger{"tmp"}
    {
        mInputCfbfFile = aCtx.mInputCfbfFile;
        mExtractedCfbfPath = aCtx.mExtractedCfbfPath;
        mCfg = aCtx.mCfg;
        mFileFormatVersion = aCtx.mFileFormatVersion;
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

    fs::path mInputCfbfFile; //!< Input CFBF container
    fs::path mExtractedCfbfPath;

    ParserConfig mCfg;

    FileFormatVersion mFileFormatVersion;

    spdlog::level::level_enum mLogLevel;
    spdlog::logger mLogger;
};


#endif // CONTAINERCONTEXT_HPP