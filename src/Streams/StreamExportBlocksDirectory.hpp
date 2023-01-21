#ifndef STREAMEXPORTBLOCKSDIRECTORY_HPP
#define STREAMEXPORTBLOCKSDIRECTORY_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "StreamDirectoryStruct.hpp"


class StreamExportBlocksDirectory : public StreamDirectoryStruct
{
public:

    StreamExportBlocksDirectory(DataStream& aDs) : StreamDirectoryStruct{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StreamExportBlocksDirectory& /* aObj */)
{
    // return dynamic_cast<const StreamDirectoryStruct*>(&aObj)->to_string();
    return "";
}


inline std::string StreamExportBlocksDirectory::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamExportBlocksDirectory& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STREAMEXPORTBLOCKSDIRECTORY_HPP