#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "ContainerContext.hpp"
#include "DataStream.hpp"
#include "Database.hpp"
#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Primitives/PrimBase.hpp"
#include "Stream.hpp"

namespace fs = std::filesystem;

namespace OOCP
{
// @todo rename to DatabaseParser
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

    Database getDb() const
    {
        return mDb;
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
     * @brief Get the DatabaseType from file extension.
     *
     * @note This is just a hint for the database type but
     *       note that the file extension is irrelevant.
     *
     * @param aFile File name including its extension.
     * @return DatabaseType Type of the database.
     */
    std::optional<DatabaseType> getDatabaseTypeByFileExtension(const fs::path& aFile) const;

    // ---------------------------------------------
    // -------------- Read Container ---------------
    // ---------------------------------------------

    void parseDatabaseFileThread(std::deque<std::shared_ptr<Stream>> aStreamList);
    void parseDatabaseFile();

private:
    Database mDb;

    size_t mFileCtr;    //!< Counts all files that were opened for parsing
    size_t mFileErrCtr; //!< Counts all files that failed somewhere

    ContainerContext mCtx;

    ParserConfig mCfg;
};
} // namespace OOCP

#endif // CONTAINER_HPP