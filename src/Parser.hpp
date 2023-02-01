#ifndef PARSER_HPP
#define PARSER_HPP


#include <any>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "DataStream.hpp"
#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Library.hpp"
#include "ParserContext.hpp"
#include "Primitives/PrimBase.hpp"


namespace fs = std::filesystem;


class Parser
{
public:

    Parser(const fs::path& aFile);

    ~Parser();

    size_t getFileErrCtr() const
    {
        return mFileErrCtr;
    }

    ParserContext& getContext()
    {
        return mCtx;
    }

    template <typename T>
    std::unique_ptr<T> parseFile(const fs::path &aFilePath, ParserContext& aCtx)
    {
        auto obj = std::make_unique<T>(aCtx);

        ++mFileCtr;

        try
        {
            openFile(aFilePath.string());
            obj->read();
            closeFile();
        }
        catch(...)
        {
            exceptionHandling();
        }

        spdlog::info("----------------------------------------------------------------------------------\n");

        return obj;
    }


    /**
     * @brief Extract container.
     *
     * @param aFile Path to container file.
     * @param aOutDir Output directory.
     * @return fs::path Path to the extraction directory.
     */
    fs::path extractContainer(const fs::path& aFile, const fs::path& aOutDir) const;

    /**
     * @brief Extract currently used container.
     *
     * @param aOutDir Output directory.
     * @return fs::path Path to the extraction directory.
     */
    fs::path extractContainer(const fs::path& aOutDir) const;

    /**
     * @brief Print container tree structure to console.
     */
    void printContainerTree() const;

    /**
     * @brief Get the file type from file extension.
     *
     * @param aFile File name including its extension.
     * @return FileType Type of the file.
     */
    FileType getFileTypeByExtension(const fs::path& aFile) const;

    // ---------------------------------------------
    // -------------- Read Container ---------------
    // ---------------------------------------------

    void parseLibrary();

private:
    void openFile(const fs::path& aFile);
    void closeFile();
    void exceptionHandling();

    std::vector<fs::path> mRemainingFiles; //!< Streams that have not yet been parsed

    fs::path mInputFile; //!< Input container
    fs::path mCurrOpenFile; //!< Input stream
    fs::path mExtractedPath;

    size_t mFileCtr;    //!< Counts all files that were opened for parsing
    size_t mFileErrCtr; //!< Counts all files that failed somewhere

    DataStream mDs;
    ParserContext mCtx;
};


#endif // PARSER_HPP