#ifndef STREAM_HPP
#define STREAM_HPP

#include <functional>
#include <memory>
#include <optional>
#include <utility>

#include <fmt/color.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "ContainerContext.hpp"
#include "Enums/StreamType.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Record.hpp"

class Stream
{
public:
    Stream(ContainerContext& aCtx, const fs::path& aInputStream)
        : mCtx{aCtx, aInputStream}
    {
    }

    virtual ~Stream()
    {
    }

    virtual std::string to_string() const = 0;

    virtual void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) = 0;

    void openFile()
    {
        mCtx.mLogger.info("Opening file: {}", mCtx.mInputStream.string());

        mCtx.mLogger.info("File contains {} byte.", fs::file_size(mCtx.mInputStream));
    }

    void closeFile()
    {
        mCtx.mLogger.info("Closing file: {}", mCtx.mInputStream.string());

        mCtx.mDs.close();
    }

    void exceptionHandling()
    {
        try
        {
            throw;
        }
        catch(const std::exception& e)
        {
            mCtx.mLogger.error(fmt::format(fg(fmt::color::crimson), "--------ERROR REPORT--------"));
            mCtx.mLogger.error(
                fmt::format(fg(fmt::color::crimson), "Input Container: {}", mCtx.mInputCfbfFile.string()));
            mCtx.mLogger.error(fmt::format(fg(fmt::color::crimson), "Current File:    {}", mCtx.mInputStream.string()));
            mCtx.mLogger.error(fmt::format(fg(fmt::color::crimson), mCtx.mDs.getCurrentOffsetStrMsg()));
            mCtx.mLogger.error(fmt::format(fg(fmt::color::crimson), "\nError Message: {}\n\n", e.what()));
        }
        catch(...)
        {
            mCtx.mLogger.error(fmt::format(fg(fmt::color::crimson), "--------ERROR REPORT--------"));
            mCtx.mLogger.error(fmt::format(fg(fmt::color::crimson), "Unknown exception caught!\n"));
        }
    }

    virtual StreamType getStreamType() const = 0;

    // protected:
    StreamContext mCtx;
};

#endif // STREAM_HPP