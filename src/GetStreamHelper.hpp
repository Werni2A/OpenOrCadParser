#ifndef GETSTREAMHELPER_HPP
#define GETSTREAMHELPER_HPP

#include <map>
#include <memory>

#include "Database.hpp"
#include "Enums/DirectoryType.hpp"
#include "Enums/StreamType.hpp"
#include "Streams/StreamDirectoryStruct.hpp"
#include "Streams/StreamLibrary.hpp"

namespace OOCP
{
static const std::map<DirectoryType, StreamType> DirectoryType2StreamType = {
    {DirectoryType::CellsDirectory,        StreamType::CellsDirectory       },
    {DirectoryType::ExportBlocksDirectory, StreamType::ExportBlocksDirectory},
    {DirectoryType::GraphicsDirectory,     StreamType::GraphicsDirectory    },
    {DirectoryType::PackagesDirectory,     StreamType::PackagesDirectory    },
    {DirectoryType::PartsDirectory,        StreamType::PartsDirectory       },
    {DirectoryType::SymbolsDirectory,      StreamType::SymbolsDirectory     },
    {DirectoryType::ViewsDirectory,        StreamType::ViewsDirectory       }
};

[[maybe_unused]]
static std::shared_ptr<StreamLibrary> getLibraryStreamFromDb(Database& aDb)
{
    for(auto& stream : aDb.mStreams)
    {
        if(stream->getStreamType() == StreamType::Library)
        {
            return std::dynamic_pointer_cast<StreamLibrary>(stream);
        }
    }

    return std::shared_ptr<StreamLibrary>{};
}

// T is of type `DirectoryStream`
template <typename T>
[[maybe_unused]]
static std::shared_ptr<T> getDirectoryStreamFromDb(Database& aDb, DirectoryType aDirType)
{
    for(auto& stream : aDb.mStreams)
    {
        if(stream->getStreamType() == DirectoryType2StreamType.at(aDirType))
        {
            return std::dynamic_pointer_cast<T>(stream);
        }
    }

    return std::shared_ptr<T>{};
}
} // namespace OOCP
#endif // GETSTREAMHELPER_HPP