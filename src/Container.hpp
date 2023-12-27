#ifndef CONTAINER_HPP
#define CONTAINER_HPP


#include <any>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "ContainerContext.hpp"
#include "DataStream.hpp"
#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Library.hpp"
#include "Primitives/PrimBase.hpp"
#include "Stream.hpp"


class StreamLibrary;


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

    ContainerContext& getContext()
    {
        return mCtx;
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

    void parseLibraryThread(std::vector<std::unique_ptr<Stream>*> aStreamList);
    void parseLibrary();

    Library& getLibrary() const
    {
        return *gLibrary;
    }

    std::optional<StreamLibrary*> getStreamLibrary() const
    {
        for(auto& stream : mStreams)
        {
            if(!stream)
            {
                continue;
            }

            const std::vector<std::optional<std::string>> pattern = {"Library"};
            if(stream->mCtx.mCfbfStreamLocation.matches_pattern(pattern))
            {
                return dynamic_cast<StreamLibrary*>(stream.get());
            }
        }

        return std::nullopt;
    }

private:
    void exceptionHandling();

    size_t mFileCtr;    //!< Counts all files that were opened for parsing
    size_t mFileErrCtr; //!< Counts all files that failed somewhere

    ContainerContext mCtx;

    ParserConfig mCfg;

    // List of streams in the CFBF container
    std::vector<std::unique_ptr<Stream>> mStreams;
};


#endif // CONTAINER_HPP