#ifndef PARSERCONTEXT_HPP
#define PARSERCONTEXT_HPP


#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "DataStream.hpp"
#include "General.hpp"


namespace fs = std::filesystem;


class ParserContext
{
public:

    ParserContext(DataStream& aDs, const fs::path& aInputFile,
        const fs::path& aCurrOpenFile, const fs::path& aExtractedPath)
        : mInputFile{aInputFile}, mCurrOpenFile{aCurrOpenFile},
          mExtractedPath{aExtractedPath}, mDs{aDs}, mImgCtr{0U},
          mSkipUnknownPrim{true}, mSkipInvalidPrim{true},
          mSkipUnknownStruct{true}, mSkipInvalidStruct{true}
    { }

    fs::path mInputFile;
    fs::path mCurrOpenFile;
    fs::path mExtractedPath;

    std::reference_wrapper<DataStream> mDs;

    size_t mImgCtr; //!< Counts images per stream

    bool mSkipUnknownPrim; //!< Unknown primitives should be skipped during parsing
    bool mSkipInvalidPrim; //!< Invalid primitives should be skipped during parsing

    bool mSkipUnknownStruct; //!< Unknown structures should be skipped during parsing
    bool mSkipInvalidStruct; //!< Invalid structures should be skipped during parsing
};


#endif // PARSERCONTEXT_HPP