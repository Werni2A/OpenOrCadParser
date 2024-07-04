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

#include "ContainerContext.hpp"
// #include "Database.hpp"
#include "General.hpp"

class Database;

namespace fs = std::filesystem;

ContainerContext::ContainerContext(
    const fs::path& aInputCfbfFile, const fs::path& aExtractedCfbfPath, ParserConfig aCfg, Database& aDb)
    : mDb{aDb},
      mLogger{"tmp"}
{
    mInputCfbfFile     = aInputCfbfFile;
    mExtractedCfbfPath = aExtractedCfbfPath;
    mCfg               = aCfg;
    mFileFormatVersion = FileFormatVersion::C;
    mLogLevel          = spdlog::level::trace;

    const fs::path logPath = mExtractedCfbfPath / "logs" / "OpenOrCadParser.log";
    configureLogger(logPath);
}

ContainerContext::ContainerContext(const ContainerContext& aCtx)
    : mDb{aCtx.mDb},
      mLogger{"tmp"}
{
    mInputCfbfFile     = aCtx.mInputCfbfFile;
    mExtractedCfbfPath = aCtx.mExtractedCfbfPath;
    mCfg               = aCtx.mCfg;
    mFileFormatVersion = aCtx.mFileFormatVersion;
    mLogLevel          = aCtx.mLogLevel;
}

void ContainerContext::configureLogger(const fs::path& aLogPath)
{
    if(aLogPath.has_parent_path())
    {
        fs::create_directories(aLogPath.parent_path());
    }

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(aLogPath.string());
    mLogger        = spdlog::logger{"file logger", {file_sink}};
    mLogger.set_pattern("[%^%l%$] %v");
    mLogger.set_level(mLogLevel);

    mLogger.info("Created log file at {}", aLogPath.string());
    spdlog::info("Created log file at {}", aLogPath.string());
}
