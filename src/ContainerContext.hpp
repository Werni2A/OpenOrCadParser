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
        const fs::path& aExtractedCfbfPath, ParserConfig aCfg, Database& aDb);

    ContainerContext(const ContainerContext& aCtx);

    void configureLogger(const fs::path& aLogPath);

    Database& mDb;

    fs::path mInputCfbfFile; //!< Input CFBF container
    fs::path mExtractedCfbfPath;

    ParserConfig mCfg;

    FileFormatVersion mFileFormatVersion;

    spdlog::level::level_enum mLogLevel;
    spdlog::logger mLogger;
};


#endif // CONTAINERCONTEXT_HPP