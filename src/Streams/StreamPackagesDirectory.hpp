#ifndef STREAMPACKAGESDIRECTORY_HPP
#define STREAMPACKAGESDIRECTORY_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "StreamDirectoryStruct.hpp"


class StreamPackagesDirectory : public StreamDirectoryStruct
{
public:

    StreamPackagesDirectory(ContainerContext& aCtx, const fs::path& aInputStream) : StreamDirectoryStruct{aCtx, aInputStream}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }
};


[[maybe_unused]]
static std::string to_string(const StreamPackagesDirectory& /* aObj */)
{
    // return dynamic_cast<const StreamDirectoryStruct*>(&aObj)->to_string();
    return "";
}


inline std::string StreamPackagesDirectory::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamPackagesDirectory& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STREAMPACKAGESDIRECTORY_HPP