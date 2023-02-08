#ifndef STREAMGRAPHICSDIRECTORY_HPP
#define STREAMGRAPHICSDIRECTORY_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "StreamDirectoryStruct.hpp"


class StreamGraphicsDirectory : public StreamDirectoryStruct
{
public:

    StreamGraphicsDirectory(ParserContext& aCtx) : StreamDirectoryStruct{aCtx}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }
};


[[maybe_unused]]
static std::string to_string(const StreamGraphicsDirectory& /* aObj */)
{
    // return dynamic_cast<const StreamDirectoryStruct*>(&aObj)->to_string();
    return "";
}


inline std::string StreamGraphicsDirectory::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamGraphicsDirectory& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STREAMGRAPHICSDIRECTORY_HPP