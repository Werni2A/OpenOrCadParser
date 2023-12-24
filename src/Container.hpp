#ifndef CONTAINER_HPP
#define CONTAINER_HPP


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


class Container
{
public:

    Container(const fs::path& aFile, ParserConfig aCfg);

    ~Container();

    size_t getFileErrCtr() const
    {
        return mFileErrCtr;
    }

    ParserContext& getContext()
    {
        return mCtx.get();
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

    Library& getLibrary() const
    {
        return *gLibrary;
    }

private:
    void openFile(const fs::path& aFile);
    void closeFile();
    void exceptionHandling();

    size_t mFileCtr;    //!< Counts all files that were opened for parsing
    size_t mFileErrCtr; //!< Counts all files that failed somewhere

    fs::path mInputCfbfFile;
    fs::path mExtractedCfbfPath;

    ParserContext tmpCtx;
    std::reference_wrapper<ParserContext> mCtx;

    ParserConfig mCfg;
};


#endif // CONTAINER_HPP