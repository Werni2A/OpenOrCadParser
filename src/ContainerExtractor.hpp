#ifndef CONTAINEREXTRACTOR_HPP
#define CONTAINEREXTRACTOR_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#ifndef COMPOUNDFILEREADER_H
#define COMPOUNDFILEREADER_H
#include <compoundfilereader.h>
#endif // COMPOUNDFILEREADER_H

namespace fs = std::filesystem;

class ContainerExtractor
{

public:
    ContainerExtractor() = delete;

    ContainerExtractor(const fs::path& aContainer);

    /**
     * @brief Print file info.
     */
    void outputFileInfo() const;

    /**
     * @brief Print entry info.
     *
     * @param aEntry File entry.
     */
    void outputEntryInfo(const CFB::COMPOUND_FILE_ENTRY* aEntry) const;

    /**
     * @brief Get all siblings including the childs itself.
     *
     * @param aChild This needs to be the child of a parent!
     * @return std::vector<const CFB::COMPOUND_FILE_ENTRY*> All siblings including the child itself.
     * @warning This is not a generic function it can only be used for childs that are specified
     *          by their parent. I.e. foo->childID must point to aChild!
     */
    std::vector<const CFB::COMPOUND_FILE_ENTRY*> getSiblings(const CFB::COMPOUND_FILE_ENTRY* aChild) const;

    /**
     * @brief Get parent entry of child.
     * @param aChild Child entry.
     * @return const CFB::COMPOUND_FILE_ENTRY* Parent entry.
     */
    const CFB::COMPOUND_FILE_ENTRY* getParent(const CFB::COMPOUND_FILE_ENTRY* aChild) const;

    /**
     * @brief Print tree structure for container.
     */
    void printContainerTree() const;

    /**
     * @brief Get path to entry inside the container structure.
     *
     * @param aEntry Entry you want the path to.
     * @return std::string Path to entry.
     */
    std::string getInternalPath(const CFB::COMPOUND_FILE_ENTRY* aEntry) const;

    /**
     * @brief Extract CFBF container.
     *
     * @param aOutputDir Directory where the container should be extracted to.
     * @return fs::path Path to extracted container.
     */
    fs::path extract(const fs::path& aOutputDir);

    /**
     * @brief Get path of container.
     *
     * @return fs::path Container path.
     */
    fs::path getContainerPath() const
    {
        return mContainer;
    }

private:
    /**
     * @brief Write buffer to file.
     *
     * @param aPath File path.
     * @param aBuffer Consecutive memory buffer.
     * @param aLen Size of buffer in bytes.
     */
    void dumpBuffer(const fs::path& aPath, const void* aBuffer, size_t aLen);

    fs::path mContainer; //!< Path to the container file
    std::unique_ptr<CFB::CompoundFileReader> mReader;
    std::unique_ptr<uint8_t[]> mBuffer; //!< Contains the content of the container
    size_t mBufferLen;                  //!< Container length
};

#endif // CONTAINEREXTRACTOR_HPP