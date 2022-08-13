#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// @todo Add header guard to CompoundFileReader library!
#ifndef COMPOUNDFILEREADER_H
#define COMPOUNDFILEREADER_H
#include <compoundfilereader.h>
#endif // COMPOUNDFILEREADER_H

#include <utf.h>

#include <spdlog/spdlog.h>

#include "ContainerExtractor.hpp"


namespace fs = std::filesystem;


ContainerExtractor::ContainerExtractor(const fs::path& aContainer)
{
    mContainer = aContainer;

    if(!fs::exists(mContainer))
    {
        throw std::runtime_error("The following file was not found: " + mContainer.string());
    }

    if(!fs::is_regular_file(mContainer))
    {
        throw std::runtime_error("The following path does not point to a file: " + mContainer.string());
    }

    std::FILE* fp = std::fopen(mContainer.c_str(), "rb");

    if(fp == nullptr)
    {
        throw std::runtime_error("Reading file " + mContainer.string() + " failed!");
    }

    const size_t len = fs::file_size(mContainer);
    mBuffer = std::make_unique<uint8_t[]>(len);

    mBufferLen = std::fread(mBuffer.get(), 1, len, fp);
    std::fclose(fp);

    mReader = std::make_unique<CFB::CompoundFileReader>(mBuffer.get(), mBufferLen);

    // mReader->GetFileInfo();
}


void ContainerExtractor::outputFileInfo() const
{
    const CFB::COMPOUND_FILE_HDR* hdr = mReader->GetFileInfo();
    std::clog
        << "file version: " << hdr->majorVersion << "." << hdr->minorVersion << std::endl
        << "difat sector: " << hdr->numDIFATSector << std::endl
        << "directory sector: " << hdr->numDirectorySector << std::endl
        << "fat sector: " << hdr->numFATSector << std::endl
        << "mini fat sector: " << hdr->numMiniFATSector << std::endl;
}


void ContainerExtractor::outputEntryInfo(const CFB::COMPOUND_FILE_ENTRY* aEntry) const
{
    std::clog
        << "entry type: " << (mReader->IsPropertyStream(aEntry) ? "property" : (mReader->IsStream(aEntry) ? "stream" : "directory")) << std::endl
        << "color flag: " << aEntry->colorFlag << std::endl
        << "creation time: " << aEntry->creationTime << std::endl
        << "modified time: " << aEntry->modifiedTime << std::endl
        << "child ID: " << aEntry->childID << std::endl
        << "left sibling ID: " << aEntry->leftSiblingID << std::endl
        << "right sibling ID: " << aEntry->startSectorLocation << aEntry->rightSiblingID << std::endl
        << "start sector: " << aEntry->startSectorLocation << std::endl
        << "size: " << aEntry->size << std::endl;
}


std::vector<const CFB::COMPOUND_FILE_ENTRY*> ContainerExtractor::getSiblings(const CFB::COMPOUND_FILE_ENTRY* aChild) const
{
    std::vector<const CFB::COMPOUND_FILE_ENTRY*> siblings;

    if(aChild == nullptr)
    {
        return siblings;
    }

    const CFB::COMPOUND_FILE_ENTRY* currEntry = aChild;

    siblings.push_back(currEntry);

    const auto leftSiblings  = getSiblings(mReader->GetEntry(aChild->leftSiblingID));
    const auto rightSiblings = getSiblings(mReader->GetEntry(aChild->rightSiblingID));

    siblings.insert(siblings.end(), leftSiblings.begin(), leftSiblings.end());
    siblings.insert(siblings.end(), rightSiblings.begin(), rightSiblings.end());

    return siblings;
}


const CFB::COMPOUND_FILE_ENTRY* ContainerExtractor::getParent(const CFB::COMPOUND_FILE_ENTRY* aChild) const
{
    if(aChild == mReader->GetRootEntry() || aChild == nullptr)
    {
        return nullptr;
    }

    const CFB::COMPOUND_FILE_ENTRY* parent = nullptr;

    mReader->EnumFiles(mReader->GetRootEntry(), -1,
        [&, this](const CFB::COMPOUND_FILE_ENTRY* entry, const CFB::utf16string& dir, int level) -> void
        {
            if(mReader->GetEntry(entry->childID) != nullptr)
            {
                const auto& siblings = getSiblings(mReader->GetEntry(entry->childID));

                if(std::any_of(siblings.begin(), siblings.end(), [&](const CFB::COMPOUND_FILE_ENTRY* sibling){ return aChild == sibling; }))
                {
                    parent = entry;
                }
            }
        });

    return parent;
}


void ContainerExtractor::printContainerTree() const
{
    mReader->EnumFiles(mReader->GetRootEntry(), -1,
        [&](const CFB::COMPOUND_FILE_ENTRY* entry, const CFB::utf16string& dir, int level) -> void
        {
            std::string prefix;

            for(int i = 1; i < level; ++i)
            {
                prefix += "  ";
            }

            if(mReader->IsStream(entry))
            {
                prefix = "[S]";
            }
            else if(mReader->IsPropertyStream(entry))
            {
                prefix = "[P]";
            }
            else
            {
                prefix = "[D]";
            }

            spdlog::info("{} {} ({} Byte)", prefix, UTF16ToUTF8(entry->name), entry->size);
        }
    );
}


std::string ContainerExtractor::getInternalPath(const CFB::COMPOUND_FILE_ENTRY* aEntry) const
{
    if(aEntry == nullptr)
    {
        return "";
    }

    const CFB::COMPOUND_FILE_ENTRY* currEntry = aEntry;
    std::string internalPath = UTF16ToUTF8(currEntry->name);

    while(currEntry != nullptr)
    {
        const CFB::COMPOUND_FILE_ENTRY* parent = getParent(currEntry);

        if(parent != nullptr)
        {
            internalPath = UTF16ToUTF8(parent->name) + "/" + internalPath;
        }

        currEntry = parent;
    }

    return internalPath;
}


fs::path ContainerExtractor::extract(const fs::path& aOutputDir)
{
    const fs::path baseOutputDir = aOutputDir / mContainer.filename();

    if(fs::exists(baseOutputDir))
    {
        spdlog::info("Removing existing {}", baseOutputDir.string());
        fs::remove_all(baseOutputDir);
    }

    mReader->EnumFiles(mReader->GetRootEntry(), -1,
        [&, this](const CFB::COMPOUND_FILE_ENTRY* entry, const CFB::utf16string& dir, int level) -> void
        {
            const fs::path internalPath{getInternalPath(entry)};

            const size_t contentSize = static_cast<size_t>(entry->size); //!< Data size in byte
            const size_t maxSize = 16777216u; //!< equals 2^24 = 16 MiB

            const auto isDirectory = [&, this](const CFB::COMPOUND_FILE_ENTRY* entry)
            {
                return !mReader->IsStream(entry) && !mReader->IsPropertyStream(entry);
            };

            if(!isDirectory(entry))
            {
                // Create binary file

                if(contentSize > maxSize)
                {
                    // @todo OrCAD is able to read those files. Maybe the filesize is corrupted but OrCAD just parses them
                    //       and stops when it thinks that the stream ended. But maybe there is some different information
                    //       hidden in the size value?
                    spdlog::error("Stream {} has size of {} Byte which is too large."
                        " Skipping stream! The original container might be corrupted!",
                        internalPath.string(), contentSize);

                    return;
                }

                std::unique_ptr<char[]> content = std::make_unique<char[]>(contentSize);

                mReader->ReadFile(entry, 0, content.get(), contentSize);

                const fs::path fullPath = baseOutputDir / fs::path{internalPath}.replace_extension(".bin");

                fs::create_directories(fullPath.parent_path());

                dumpBuffer(fullPath, content.get(), contentSize);
            }
            else
            {
                // Create directory

                if(contentSize > 0u)
                {
                    spdlog::error("Directory {} has size of {} Byte but should be 0 Byte!"
                        " Creating the directory anyway but the original container might be corrupted!",
                        internalPath.string(), contentSize);

                    return;
                }

                fs::path fullPath = baseOutputDir / internalPath;
                fs::create_directories(fullPath);
            }
        });

    return baseOutputDir;
}


void ContainerExtractor::dumpBuffer(const fs::path& aPath, const void* aBuffer, size_t aLen)
{
    FILE* fp = std::fopen(aPath.c_str(), "wb");

    if(fp == nullptr)
    {
        spdlog::critical("Write file error!");
        std::exit(1);
    }

    std::fwrite(aBuffer, 1, aLen, fp);
    std::fclose(fp);
}