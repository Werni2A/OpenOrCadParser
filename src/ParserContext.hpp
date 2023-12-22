#ifndef PARSERCONTEXT_HPP
#define PARSERCONTEXT_HPP


#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "DataStream.hpp"
#include "General.hpp"


namespace fs = std::filesystem;


[[maybe_unused]] static std::vector<std::string> getCfbfStreamLocationFromFileSytemLayout(
    fs::path aStreamPath, fs::path aExtractedCfbfPath)
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

    return streamLocation;
}


static std::unique_ptr<DataStream> globalStream{};

struct ParserConfig
{
    bool mSkipUnknownPrim{true}; //!< Unknown primitives should be skipped during parsing
    bool mSkipInvalidPrim{true}; //!< Invalid primitives should be skipped during parsing

    bool mSkipUnknownStruct{true}; //!< Unknown structures should be skipped during parsing
    bool mSkipInvalidStruct{true}; //!< Invalid structures should be skipped during parsing

    bool mKeepTmpFiles{false}; //!< Do not delete temporary files after parser completed
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


class ParserContext
{
public:

    ParserContext() = default;

    ParserContext(const fs::path& aInputCfbfFile,
        const fs::path& aInputStream, const fs::path& aExtractedCfbfPath, ParserConfig aCfg) : mDs{*globalStream}
    {
        mInputCfbfFile = aInputCfbfFile;
        mInputStream = aInputStream;
        mExtractedCfbfPath = aExtractedCfbfPath;

        mCfbfStreamLocation = getCfbfStreamLocationFromFileSytemLayout(mInputStream, mExtractedCfbfPath);

        globalStream = std::make_unique<DataStream>(mInputStream);
        mDs = *globalStream;

        mImgCtr = 0U;

        mFinishedParsing = false;

        mCfg = aCfg;
    }

    fs::path mInputCfbfFile; //!< Input CFBF container
    fs::path mInputStream; //!< Input CFBF stream as file in the file system
    fs::path mExtractedCfbfPath;

    std::vector<std::string> mCfbfStreamLocation; //!< Location of the stream inside the CFBF container

    std::reference_wrapper<DataStream> mDs;

    size_t mImgCtr; //!< Counts images per stream

    bool mFinishedParsing;

    ParserConfig mCfg;
};


#endif // PARSERCONTEXT_HPP