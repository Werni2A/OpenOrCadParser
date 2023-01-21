#ifndef STREAMPARTSDIRECTORY_HPP
#define STREAMPARTSDIRECTORY_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "StreamDirectoryStruct.hpp"


class StreamPartsDirectory : public StreamDirectoryStruct
{
public:

    StreamPartsDirectory(DataStream& aDs) : StreamDirectoryStruct{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StreamPartsDirectory& /* aObj */)
{
    // return dynamic_cast<const StreamDirectoryStruct*>(&aObj)->to_string();
    return "";
}


inline std::string StreamPartsDirectory::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamPartsDirectory& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STREAMPARTSDIRECTORY_HPP