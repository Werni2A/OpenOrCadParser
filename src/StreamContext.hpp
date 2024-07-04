#ifndef STREAMCONTEXT_HPP
#define STREAMCONTEXT_HPP

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "CfbfStreamLocation.hpp"
#include "ContainerContext.hpp"
#include "DataStream.hpp"
#include "General.hpp"
// #include "Stream.hpp"

class Stream;

namespace fs = std::filesystem;

class StreamContext : public ContainerContext
{
public:
    StreamContext(const ContainerContext& aCtx, const fs::path& aInputStream)
        : ContainerContext{aCtx},
          mInputStream{aInputStream},
          mCfbfStreamLocation{mInputStream, mExtractedCfbfPath},
          mDs{aInputStream, *this}
    {
        mImgCtr             = 0U;
        mAttemptedParsing   = false;
        mParsedSuccessfully = std::nullopt;

        // Create stream specific logger
        const fs::path logPath{mExtractedCfbfPath.parent_path() / "logs" / mInputCfbfFile.filename() /
                               mCfbfStreamLocation.get_relative_fs_path().concat(".log")};
        configureLogger(logPath);
    }

    fs::path mInputStream; //!< Input CFBF stream as file in the file system

    CfbfStreamLocation mCfbfStreamLocation; //!< Location of the stream inside the CFBF container

    DataStream mDs;

    size_t mImgCtr; //!< Counts images per stream

    // True, iff the parser was run on this stream. It is
    // not important wether the parser was successful or not
    bool mAttemptedParsing;

    // True, if stream was parsed successfully. If there
    // was no attempt to parse the stream yet, this variable
    // is null.
    std::optional<bool> mParsedSuccessfully;
};

#endif // STREAMCONTEXT_HPP